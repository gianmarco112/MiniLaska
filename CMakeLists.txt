cmake_minimum_required(VERSION 3.17)
project(MiniLaska C)

set(CMAKE_C_STANDARD 11)

include_directories(Laska)

add_executable(MiniLaska

        Laska/MiniLaska.c
        Laska/MiniLaska.h)
