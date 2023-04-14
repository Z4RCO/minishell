cmake_minimum_required(VERSION 3.24)
project(minishell C)

set(CMAKE_C_STANDARD 11)

add_executable(minishell main.c parser.h comandos.h comandos.c gestorProcesos.h ListaEnlazada.h ListaEnlazada.c gestorProcesos.c)
find_library(LIBPARSER parser ./)
target_link_libraries(minishell ${LIBPARSER})


set(CMAKE_C_FLAGS "-Wall -Wextra -static")