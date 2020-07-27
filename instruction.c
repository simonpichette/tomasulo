/****** instruction.c ********************************************************
*   Description
*       Functions for the management of instructions and instruction lists
*       used by Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Thu Jul 16 16:38:47 2020
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "instruction.h"

static void _grow(struct ilist* list);
static int _decode(struct ilist* list, int pos, char* text);
static int _process_loadstore(struct instruction* inst, char* elem, char* text);
static int _assign_register(int* regid, char* elem);
static int _copy_inst_string(struct instruction* inst, char* text);
static int _process_arithmetic(struct instruction* inst, char* elem, char* text);

// array of strings for instruction mnemonics
// must be ordered the same as enum opcode for
// opcode field assignment to work correctly
const char* mnemonics[] = {"ld", "sw", "addd", "subd", "muld", "divd"};

// array of ints for execution time
// also ordered the same as enum opcode
int exec_cycles[100] = {};

int modify_exec_cycles(const char* filename){
        char buffer[20];
        char* line;

        FILE *source = fopen(filename, "rt");
        if (!source) {
            return -1;
        }

        int exec_index = 0;
        while (fgets(buffer, sizeof(buffer), source)) {
            line =  strtok(buffer, "\n");        // remove trailing newline
            exec_cycles[exec_index] = atoi(line);
            exec_index++;
        }
        return 0;
}

void inst_details(struct instruction* inst) {
    printf("Text   : %s    name  : %s\n", inst->text, inst->name);
    printf("issue  : %d  execute : %d  writeback : %d  retired : %d  remaining : %d\n",
        inst->issue, inst->execute, inst->writeback, inst->retired, inst->remaining);
    printf("opcode : %d  opclass : %d\n", inst->op, inst->opclass);
    printf("    rd : %d,     rs1 : %d,       rs2 : %d\n\n", inst->rd, 
        inst->rs1, inst->rs2);
}


void print_inst(struct instruction* inst) {
    printf("|%20s |%10d |%10d |%10d |%10d |\n",
        inst->text, inst->issue, inst->execute, inst->writeback, inst->retired);
}


struct ilist* create_inst_list(int initial_size) {
    struct ilist* list = malloc(sizeof(struct ilist));
    if (list == NULL) {
        return NULL;
    }

    list->size = initial_size;
    list->occupied = 0;
    list->data = malloc(initial_size * sizeof(struct instruction));
    if (list->data == NULL) {
        return NULL;
    }
    return list;
}


int add_inst(struct ilist* list, char* text) {
    if (list->occupied == list->size) {
        _grow(list);
    }

    if (list->occupied == list->size) {
        // _grow failed, aborting
        return -1;
    }

    // this struct initialization method requires C99
    list->data[list->occupied] = (struct instruction){0};

    int retval = _decode(list, list->occupied, text);
    if (!retval) {
        list->occupied++;
    }
    return retval;
}


static void _grow(struct ilist* list) {
    struct instruction* newlist = realloc(list->data, 
                            (list->size << 1) * sizeof(struct instruction));
    if (newlist == NULL) {
        // realloc failed, list is unchanged and still valid
        return;         
    }

    list->size <<= 1;
    list->data = newlist;
    return;
}


static int _decode(struct ilist* list, int pos, char* text) { 
    struct instruction* inst = &list->data[pos];

    char* elem = malloc(strlen(text) + 1);
    if (!elem) { return -1; }
    strcpy(elem, text);

    elem = strtok(elem, " ,()");
    if (elem != NULL) {
        for (int i = 0; i < 6; i++) {
            if (!strcmp(elem, mnemonics[i])) {
                inst->op = i;
                inst->name = (char *) mnemonics[i];
                inst->remaining = exec_cycles[i];
                switch (i) {
                    case ld:
                    case sw:
                        return _process_loadstore(inst, elem, text);
                    case addd:
                    case subd:
                        inst->opclass = addsub;
                        return _process_arithmetic(inst, elem, text);
                    case muld:
                    case divd:
                        inst->opclass = muldiv;
                        return _process_arithmetic(inst, elem, text);
                }
            }
        }
    }
    return -10;
}


static int _process_loadstore(struct instruction* inst, char* elem, char* text) {
    inst->opclass = loadstore;
    if (_assign_register(&inst->rd, elem) != 0) { return -2; }
    if (_copy_inst_string(inst, text) != 0)     { return -3; }

    // No other information is required to simulate loads and store
    return 0;
}


static int _process_arithmetic(struct instruction* inst, char* elem, char* text) {
    if (_assign_register(&inst->rd, elem)  != 0) { return -4; }
    if (_assign_register(&inst->rs1, elem) != 0) { return -5; }
    if (_assign_register(&inst->rs2, elem) != 0) { return -6; }
    if (_copy_inst_string(inst, text) != 0) { return -7; }
    return 0;
}


static int _assign_register(int* regid, char* elem) {
    elem = strtok(NULL, " ,()");
    if (elem[0] == 'F') {
        *regid = atoi(elem + 1);
        return 0;
    } else {
        return -8;
    } 
}


static int _copy_inst_string(struct instruction* inst, char* text) {
    inst->text = malloc(strlen(text) + 1);
    if(inst->text) {
        strcpy(inst->text, text);
        return 0;
    } else {
        return -9;
    }
}
