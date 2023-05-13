#pragma once
#ifndef MINISHELL_COMANDOS_H
#define MINISHELL_COMANDOS_H


#include "parser.h"
#include "procesos.h"

int ejecutarComando(tcommand comando, Gestor* gestor);

/**
 * Función para cambiar de directorio
 * @param dir
 * @return
 */
int cd(char* dir);

//TODO ARGUMENTOS
void jobs(Gestor *gestor);

int fg(pid_t pid, Gestor* gestor);

#endif //MINISHELL_COMANDOS_H
