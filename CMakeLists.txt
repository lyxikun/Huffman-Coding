cmake_minimum_required(VERSION 3.29)
project(Huffman_Coding C)

set(CMAKE_C_STANDARD 11)

include_directories(Inc)

link_libraries(m)

add_executable(Huffman_Coding
    Inc/huffmantable.h
        Src/main.c Src/huffmantable.c
)
