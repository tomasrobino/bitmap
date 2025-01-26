cmake_minimum_required(VERSION 3.30)
project(bitmap C)

set(CMAKE_C_STANDARD 11)

add_executable(bitmap main.c)
configure_file(example.bmp example.bmp COPYONLY)