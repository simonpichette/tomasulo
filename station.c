/****** station.c ***********************************************************
*   Description
*       Reservation station management for Tomasulo's algorithm simulator
*
*   Author          : Simon Pichette
*   Creation date   : Sun Jul 19 15:38:29 2020
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
#include <stdbool.h>
#include <stdio.h>
#include "station.h"

static void _grow(struct slist* list);
static struct station _init_station(char* name, enum opclasses type);


struct slist* create_station_list(int initial_size) {
	struct slist* list = malloc(sizeof(struct slist));
    if (list == NULL) {
        return NULL;
    }

    list->size = initial_size;
    list->occupied = 0;
    list->data = malloc(initial_size * sizeof(struct station));
    if (list->data == NULL) {
        return NULL;
    }
    return list;
}


int add_station(struct slist* list, char* name, enum opclasses type) {
    if (list->occupied == list->size) {
        _grow(list);
    }

    if (list->occupied == list->size) {
        // _grow failed, aborting
        return -1;
    }

    list->data[list->occupied] = _init_station(name, type);
    list->occupied++;
    return 0;
}


static void _grow(struct slist* list) {
    // attempts to grow a list to double capacity, preserving data using realloc

    struct station* newlist = realloc(list->data, 
                            (list->size << 1) * sizeof(struct station));
    if (newlist == NULL) {
        // realloc failed, list is unchanged and still valid
        return;         
    }

    list->size <<= 1;
    list->data = newlist;
    return;
}


static struct station _init_station(char* name, enum opclasses type) {
    // this struct initialization method requires C99
    struct station s = (struct station){0};

    s.name = malloc(sizeof(name));
    if (! s.name) {
        return s;
    }
    strcpy(s.name, name);

    s.type = type;
    return s;
}


void print_station(struct station* st) {
	char* busy = (st->busy == true) ? "yes" : "no";
	char* op = (st->busy == true) ? st->op->name : "";
	char* vj = (st->vj != NULL) ? st->vj : "";
	char* vk = (st->vk != NULL) ? st->vk : "";
	char* qj = (st->qj != NULL) ? st->qj : "";
	char* qk = (st->qk != NULL) ? st->qk : "";

	printf("|%9s |%7s |%8s |%8s |%8s |%8s |%8s |\n", 
			st->name, busy, op, vj, vk, qj, qk);
}
