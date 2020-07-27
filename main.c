/****** main.c **************************************************************
*   Description
*       Entry point for Tomasulo's Out-of-order execution algorithm
*       simulator.
*
*       Contains initialization, parameters, main simulation loop
*       and display functions 
*
*   Author          : Simon Pichette
*   Creation date   : Thu Jul 16 09:45:09 2020
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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "station.h"
#include "tomasulo.h"


void print_banner();
void print_scoreboard(struct ilist* program);
void print_stations(struct slist* stations);
void print_registers();
int load_program(const char* filename, struct ilist* prog);
void print_state(struct state* s, char* reg_names[]);


int main(void) {
     // arrays of strings for register names and content
    char* reg_names[] = {"F0", "F2", "F4", "F6", "F8", "F10", "F12", "F14"};
    char* reg_contents[] = {"", "", "", "", "", "", "", ""};
    char input;

    modify_exec_cycles("exec_cycles.txt");

    int nbr_add, nbr_mul, nbr_ld;

    // program loading
    struct ilist* program = create_inst_list(10);
    if (!program) {
        puts("list creation failed");
    }
    load_program("prog1.txt", program);

    printf("De base cette station comporte 3 Add, 2 Mul et 2 load \n"
           "Voulez vous (g)arder cette configugation ou voulez-vous la (m)odifier?\n");
    scanf(" %c", &input);

    switch(input) {
        case 'g' :
            // create reservation stations
            nbr_add = 3;
            nbr_mul = 2;
            nbr_ld = 2;
            break;

        case 'm':
            printf("Combien de ADDs voulez-vous?\n");
            scanf(" %i", &nbr_add);
            printf("Combien de MULs voulez-vous?\n");
            scanf(" %i", &nbr_mul);
            printf("Combien de LOADs voulez-vous?\n");
            scanf(" %i", &nbr_ld);
            break;
    }

    struct slist* stations = create_station_list(nbr_add+nbr_ld+nbr_mul+3);
    char buf[12];
    for (int i =0; i < nbr_add; i++){
        snprintf(buf, 12, "Add%d", i+1);
        add_station(stations, buf, addsub);
    }
    for (int i =0; i < nbr_mul; i++){
        snprintf(buf, 12, "Mul%d", i+1);
        add_station(stations, buf, muldiv);
    }
    for (int i =0; i < nbr_ld; i++){
        snprintf(buf, 12, "Load%d", i+1);
        add_station(stations, buf, loadstore);
    }

    // init simulation state context
    struct state context;
    context.program = program;
    context.stations = stations;
    context.issue_width = 1;
    context.regfile_size = 8;


    // run simulation
    for (context.cycle = 1; context.cycle < 100; context.cycle++) {
        retire(&context, reg_contents);
        issue(&context, reg_names, reg_contents);
        execute(&context);
        writeback(&context);

        print_state(&context, reg_contents);
        puts("(c)ontinue, (a)bort");
        scanf(" %c", &input);
        switch(input) {
            case 'c' :
                break;
            case 'a':
                return 0;
        }
        //system("clear");    // UNIX
        system("cls");    // DOS
    }
}


void print_state(struct state* s, char* reg_names[]) {
    print_banner();
    printf("Cycle : %d \n", s->cycle);
    print_scoreboard(s->program);
    print_stations(s->stations);
    print_registers(reg_names, s->regfile_size);
}


int load_program(const char* filename, struct ilist* prog) {
    char buffer[20];
    char* line; 

    FILE *source = fopen(filename, "rt");
    if (!source) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), source)) {
        line = strtok(buffer, "\n");        // remove trailing newline
        int result = add_inst(prog, line);
        if (!result) {
            //inst_details(&prog->data[prog->occupied-1]);
        } else {
            printf ("Error!!, code %d\n", result);
            return -1;
        }
    }
    return 0;
}


void print_banner() {
    puts("***********************************************************************");
    puts("*  ELE749 Out-of-order execution demo using Tomasulo's algorithm      *");
    puts("***********************************************************************");
    puts("");
}


void print_scoreboard(struct ilist* program) {
    puts("|---------------------------------------------------------------------|");
    puts("| Instruction         | Issue     | Execute   | Writeback | Retired   |");
    puts("|---------------------------------------------------------------------|");
    for (size_t i = 0; i < program->occupied; i++) {
        print_inst(&program->data[i]);
    }
    puts("|---------------------------------------------------------------------|");
    puts("");
}


void print_stations(struct slist* stations) {
    puts("|---------------------------------------------------------------------|");
    puts("| Reservation stations                                                |");
    puts("|---------------------------------------------------------------------|");
    puts("| Name     |  Busy  |    Op   |   Vj    |    Vk   |    Qj   |    Qk   |");
    puts("|---------------------------------------------------------------------|");
    for (size_t i = 0; i < stations->occupied; i++) {
        print_station(&stations->data[i]);
    }
    puts("|---------------------------------------------------------------------|");
    puts("");
}


void print_registers(char* reg_names[], size_t num) {
    puts("|-----------------------------------------------------------------------|");
    puts("| Register wait queues (Qi)                                             |");
    puts("|-----------------------------------------------------------------------|");
    puts("|   F0   |   F2   |   F4   |   F6   |   F8   |  F10   |  F12   |  F14   |");
    for (size_t i = 0; i < num; i++) {
        printf("|%7s ", reg_names[i]);
    }
    puts("|");
    puts("|-----------------------------------------------------------------------|");
}
