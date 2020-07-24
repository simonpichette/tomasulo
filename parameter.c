//
// Created by Francis on 2020-07-23.
//

#include "parameter.h"
#include "instruction.h"
#include "string.h"

int load_stations(const char* filename,struct slist* stations){
    int nb_stations = 0;

    FILE *param_file = fopen(filename, "r");
    if (!param_file) {
        return NULL;
    }

    //Skip de file header
    fscanf(param_file, "%*[^\n]\n");
    char station_name[20];
    char station_type[20];
    while(fscanf(param_file, "%s %s", station_name, station_type) != EOF) {

        if(strcmp("addsub",station_type) == 0){
            add_station(stations, station_name, addsub);
        }
        else if(strcmp("muldiv",station_type) == 0){
            add_station(stations, station_name, muldiv);
        }
        else{
            add_station(stations, station_name, loadstore);
        }
        nb_stations++;
    }
    fclose(param_file);
    return nb_stations;
}