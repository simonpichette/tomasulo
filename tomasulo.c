/****** tomasulo.c **********************************************************
*   Description
*       Simulation stage functions for Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Mon Jul 20 08:39:21 2020
*****************************************************************************
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "tomasulo.h"
#include "instruction.h"
#include "station.h"


static struct station* _find_station(struct instruction* inst, struct slist* rs);
static void _fill_station(struct station* st, struct instruction* inst, 
                          char* reg_names[], char* reg_contents[]);
static bool _ready(struct station* st);
static void _propagate_result(struct slist* stations, struct station* st);
static void _clear_station(struct station* st);


void retire(struct state* s, char* reg_contents[]) {
    // for each instruction
    // if writeback != 0 and != current cycle
    // set retired to current cycle
    // clear destination register Qi (data is now in registers)

    for(size_t i = 0; i < s->program->occupied; i++) {
        struct instruction* inst = &s->program->data[i];

        if (inst->writeback && !inst->retired && inst->writeback != s->cycle) {
            inst->retired = s->cycle;
            reg_contents[inst->rd >> 1] = "";
        }
    }
}


void writeback(struct state* s) {
    // for each station
    // if busy
    // if op->remaining == 0
        // set writeback to current cycle
        // to simulate the CDB, we must next update all stations that
        // are waiting on this one by moving the blocker from Qx to Vx
        // finally we clear the station and make it available again

    for (size_t i = 0; i < s->stations->occupied; i++) {
        struct station* st = &s->stations->data[i];
        if (st->busy) {
            if (!st->op->remaining) {
                st->op->writeback = s->cycle;
                _propagate_result(s->stations, st);
                _clear_station(st);
            }
        }
    }
}


static void _propagate_result(struct slist* stations, struct station* cdb) {
    // for each station that waits on results from cdb
    // move source from Qx to Vx

    for (size_t i = 0; i < stations->occupied; i++) {
        struct station* st = &stations->data[i];

        if (st->qj) {
            if (!strcmp(st->qj, cdb->name)) {
                st->vj = cdb->name;
                st->qj = NULL;
            }
        }

        if (st->qk) {
            if (!strcmp(st->qk, cdb->name)) {
                st->vk = cdb->name;
                st->qk = NULL;
            }
        }
    }
}


static void _clear_station(struct station* st) {
    st->vj = NULL;
    st->vk = NULL;

    // qj and qk are already clear, otherwise we could not execute
    st->busy = false;
}


void execute(struct state* s) {
    // for each station
    //  if busy
    //      if a loadstore or all operands available 
    //          if not launched this cycle and not started
    //              start execution
    //          else 
    //              decrement remaining cycles of instruction

    for(size_t i = 0; i < s->stations->occupied; i++) {
        struct station* st = &s->stations->data[i];

        if (st->busy) {
            if ((st->op->opclass == loadstore) || _ready(st)) {
                if (st->op->issue != s->cycle) {
                    if (!st->op->execute) {
                        st->op->execute = s->cycle;
                    } else {
                        st->op->remaining--;
                    }
                }
            }
        }
    }
}


static bool _ready(struct station* st) {
    if (st->vj != NULL && st->vk != NULL) {
        return true;
    }
    return false;
}


void issue(struct state* s, char* reg_names[], char* reg_contents[]) {
    struct instruction* inst;
    struct station* st;
    size_t i = 0;

    // retrieve first unissued instruction
    while(i <= s->program->occupied) {
        inst = NULL;
        st = NULL;

        if (s->program->data[i].issue == 0) {
            inst = &s->program->data[i];
            
            // try to issue instruction
            st = _find_station(inst, s->stations);
            if (!st) {
                // no station available, try next instruction
                continue;   
            }

            // station available, send instruction
            _fill_station(st, inst, reg_names, reg_contents);
            inst->issue = s->cycle;
            return;
        }
        i++;
    }
    // no instruction can be issued this cycle
    return;
}


static void _fill_station(struct station* st, struct instruction* inst, 
                            char* reg_names[], char* reg_contents[]) {
    st->busy = true;
    st->op = inst;
    reg_contents[inst->rd >>1] = st->name;

    if (inst->opclass != loadstore) {
        // if source register 1 is ready (i.e. not waiting)
        if (! strcmp(reg_contents[inst->rs1 >>1], "")) {
            st->vj = reg_names[inst->rs1 >>1];
        } else {
            // indicate stall source
            st->qj = reg_contents[inst->rs1 >>1];     
        }

        // if source register 2 is ready (i.e. not waiting)
        if (! strcmp(reg_contents[inst->rs2 >>1], "")) {
            st->vk = reg_names[inst->rs2 >>1];
        } else {
            // indicate stall source
            st->qk = reg_contents[inst->rs2 >>1];     
        }
    }
}


static struct station* _find_station(struct instruction* inst, struct slist* rs) {
    for(size_t i = 0; i < rs->occupied; i++) {
        if(rs->data[i].type == inst->opclass) {
            if(!rs->data[i].busy) {
                return &rs->data[i];
            }
        }
    }
    return NULL;
}
