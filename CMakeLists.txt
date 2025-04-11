cmake_minimum_required(VERSION 3.10)

project(CVPC)

include_directories(include/)

add_executable(CVPC 
    src/main.cpp
    src/lexer.cpp)