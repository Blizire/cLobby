cmake_minimum_required(VERSION 3.23.2)

project(Vision VERSION 1.0
            DESCRIPTION "Open the computers eyes"
            LANGUAGES C)

include_directories(include)
file(GLOB SOURCES "src/*.c")

add_executable(vision ${SOURCES})