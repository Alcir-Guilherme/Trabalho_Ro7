cmake_minimum_required(VERSION 4.1.2)
project(Trabalho_Ro7 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Trabalho_Ro7
        Arvore.c
        HASH_PESSOAS.c
        Manipula.c
        Filmes.c
        main.c
)
