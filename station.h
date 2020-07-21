/****** station.h ***********************************************************
*   Description
*       Reservation station data model for Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Sun Jul 19 15:34:23 2020
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

#ifndef STATION_H
#define STATION_H

#include <stdlib.h>
#include "instruction.h"


struct station {
    char* name;
    enum opclasses type;
    bool busy;
    struct instruction* op;
    char* vj;
    char* vk;
    char* qj;
    char* qk;
};

struct slist {
    size_t size;
    size_t occupied;
    struct station *data;
};


/****** create_station_list *************************************************
*   Create a dynamic array of reservation stations
*       
*   Parameters : 
*       int initial_size 		: size of array after creation
*
*   Return : pointer to the newly allocated slist if successful
*			 NULL if memory allocation fails
*
*   Side effects : 
*           memory for an slist including initial_size new stations is 
*			allocated.
*****************************************************************************/
struct slist* create_station_list(int initial_size);


/****** add_station *********************************************************
*   Add a station to an slist
*       
*   Parameters : 
*       struct slist* list		: target slist
* 		char* name				: name of new station
*		enum opclasses type 	: execution unit type served by this station
*
*   Return : 0 if succesfull, non-zero otherwise
*
*   Side effects : 
*           if there is space on the list, a station struct is modified.
*			if the list is full, enough memory for doubling the size of the
*			list is allocated.
*****************************************************************************/
int add_station(struct slist* list, char* name, enum opclasses type);


/****** print_station *******************************************************
*   Display information about a station in a format that is compatible with
* 	 then following header :
* 	"| Name     |  Busy  |    Op   |   Vj    |    Vk   |    Qj   |    Qk   |"
*       
*   Parameters : 
*       struct station* st 		: the reservation station to display
*
*   Return : none
*
*   Side effects : 
*           a line of output is sent to the terminal
*****************************************************************************/
void print_station(struct station* st);

#endif
