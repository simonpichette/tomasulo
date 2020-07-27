/****** instruction.h ********************************************************
*   Description
*       Instruction data model for Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Thu Jul 16 16:38:42 2020
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
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdlib.h>
#include <stdbool.h>

enum opclasses {addsub, muldiv, loadstore};
enum opcode {ld, sw, addd, subd, muld, divd};

struct instruction {
    char* text;
    int issue;
    int execute;
    int writeback;
    int retired;
    int remaining;
    enum opcode op;
    enum opclasses opclass;
    int rs1;
    int rs2; 
    int rd;
    char* name;
};

struct ilist {
    size_t size;
    size_t occupied;
    struct instruction *data;
    bool complete;
};


/****** create_instruction_list *********************************************
*   Create a dynamic array of instructions
*       
*   Parameters : 
*       int initial_size        : size of array after creation
*
*   Return : pointer to the newly allocated ilist if successful
*            NULL if memory allocation fails
*
*   Side effects : 
*           memory for an ilist including initial_size new instructions is 
*           allocated.
*****************************************************************************/
struct ilist* create_inst_list(int initial_size);


/****** add_inst ************************************************************
*   Add an instruction to an ilist
*       
*   Parameters : 
*       struct ilist* list      : target ilist
*       char* text              : source text of the instruction, to be decoded
*
*   Return : 0 if succesfull, non-zero otherwise
*
*   Side effects : 
*           if there is space on the list, an instruction struct is modified.
*           if the list is full, enough memory for doubling the size of the
*           list is allocated.
*****************************************************************************/
int add_inst(struct ilist* list, char* text);


/****** print_isnt **********************************************************
*   Display information about an instruction in a format that is compatible with
*    then following header :
*   "| Instruction         | Issue     | Execute   | Writeback | Retired   |"
*       
*   Parameters : 
*       struct instruction* inst    : the instruction to display
*
*   Return : none
*
*   Side effects : 
*           a line of output is sent to the terminal
*****************************************************************************/
void print_inst(struct instruction* inst);


/****** inst_details ********************************************************
*   Display complete information about an instruction for debugging
*       
*   Parameters : 
*       struct instruction* inst    : the instruction to display
*
*   Return : none
*
*   Side effects : 
*           5 lines of output are sent to the terminal
*****************************************************************************/
void inst_details(struct instruction* inst);

/****** modify_exec_cycles **************************************************
* Modify the number of cycles by instructions
*
* @param filename name of the file with cycles for instructions
* @return -1 if file not loaded else 0
*****************************************************************************/
int modify_exec_cycles(const char* filename);

#endif
