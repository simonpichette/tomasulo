cmake_minimum_required(VERSION 2.4)

project(tomasulo)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(tomasulo main.c instruction.c station.c tomasulo.c parameter.c)
