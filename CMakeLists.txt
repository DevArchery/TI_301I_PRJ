cmake_minimum_required(VERSION 3.10)
project(TI301I_PRJ C)

set(CMAKE_C_STANDARD 99)

add_executable(TI301I_PRJ
        main.c
        utils.c
        data.c
        hasse.c)
