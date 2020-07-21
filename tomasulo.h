/****** tomasulo.h **********************************************************
*   Description
*       Data model and stage functions for Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Mon Jul 20 08:38:39 2020
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
#ifndef TOMASULO_H
#define TOMASULO_H

#include <stdlib.h>
#include <stdbool.h>

struct state {
    struct ilist* program;
    struct slist* stations;
    int cycle;
    int issue_width;
    int regfile_size;
    bool complete;
};

/****** issue ***************************************************************
*   Dispatch instructions to reservation stations
*       
*   Parameters : 
*       struct state* s 		: current simulation context
*       char *reg_names[]   	: array of string, the names of register Qs
*       char *reg_contents[]   	: array of string, the contents of register Qs
*
*   Return : none
*
*   Side effects : 
*           instructions, reservation stations and register Qs are modified
*****************************************************************************/
void issue(struct state* s, char* reg_names[], char* reg_contents[]);


/****** execute *************************************************************
*   For all reservation stations,
*   Wait for operands then simulate an execution cycle (decrease counter)
* 		
*       
*   Parameters : 
*       struct state* s 		: current simulation context
*
*   Return : none
*
*   Side effects : 
*           instructions are modified
*****************************************************************************/
void execute(struct state* s);


/****** writeback ***********************************************************
*   For all reservation stations,
*   wait for execution to complete then propagate results (simulate CDB)
* 		
*       
*   Parameters : 
*       struct state* s 		: current simulation context
*
*   Return : none
*
*   Side effects : 
*           instructions and reservation stations are modified
*****************************************************************************/
void writeback(struct state* s);


/****** retire ***********************************************************
*   For all instructions,
*   wait for writeback to complete then clear register Q
* 		
*       
*   Parameters : 
*       struct state* s 		: current simulation context
*       char *reg_contents[]   	: array of string, the contents of register Qs
*
*   Return : none
*
*   Side effects : 
*           instructions and register Qs are modified
*****************************************************************************/
void retire(struct state* s, char* reg_contents[]);

#endif
