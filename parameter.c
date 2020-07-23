//
// Created by Francis on 2020-07-23.
//

#include "parameter.h"

struct parameter** load_parameter(const char* filename){
    int nb_lines = 0;
    char c;

    FILE *param_file = fopen(filename, "r");
    if (!param_file) {
        return NULL;
    }

    for (c = getc(param_file); c != EOF; c = getc(param_file)) {
        if (c == '\n')
            nb_lines++;
    };
    nb_lines -= 1;

    struct parameter** param_list = malloc(nb_lines * sizeof(struct parameter*));
    for(int i=0; i<nb_lines; i++){
        param_list[i] = (struct parameter*)malloc(sizeof(struct parameter));
        if (param_list[i] == NULL) {
            return NULL;
        }
    }

    int i = 0;
    rewind(param_file);
    //Skip de file header
    fscanf(param_file, "%*[^\n]\n");

    while(fscanf(param_file, "%s %s %d", param_list[i]->opclasses, param_list[i]->mnemonics,
                 &param_list[i]->exec_cycles) != EOF) {
        i++;
    }
    fclose(param_file);
    return param_list;
}