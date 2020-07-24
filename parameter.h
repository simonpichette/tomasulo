//
// Created by Francis on 2020-07-23.
//

#ifndef PARAMETER_H
#define PARAMETER_H

#include<stdlib.h>
#include <stdio.h>
#include "station.h"

struct parameter{
    char name[20];
    char class[20];
};

int load_stations(const char* filename,struct slist* stations);

#endif //PARAMETER_H
