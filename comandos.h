#pragma once
#ifndef MINISHELL_COMANDOS_H
#define MINISHELL_COMANDOS_H


#include "parser.h"
#include "procesos.h"

int ejecutarComando(tcommand comando);

/**
 * Función para cambiar de directorio
 * @param dir
 * @return
 */
int cd(char* dir);

//TODO ARGUMENTOS
void jobs(Gestor *gestor);

void fg();

#endif //MINISHELL_COMANDOS_H
