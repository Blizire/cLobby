cmake_minimum_required(VERSION 3.23.2)

project(Vision VERSION 1.0
            DESCRIPTION "Open the computers eyes"
            LANGUAGES C)

add_executable(vision ./src/main.c ./src/main.h)