//
// Created by Francis on 2020-07-23.
//

#ifndef PARAMETER_H
#define PARAMETER_H

#include<stdlib.h>
#include <stdio.h>

struct parameter{
    char opclasses[20];
    char mnemonics[20];
    int exec_cycles;
};

struct parameter** load_parameter(const char* filename);

#endif //PARAMETER_H
