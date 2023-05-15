#pragma once
#ifndef MINISHELL_COMANDOS_H
#define MINISHELL_COMANDOS_H


#include "parser.h"
#include "procesos.h"

/**
 * Función wrapper para ejecutar cd, jobs o fg
 * @param comando comando que se quiere ejecutar
 * @param gestor  puntero al gestor, necesario en algunas de las funciones
 */
void ejecutarComando(tcommand comando, Gestor* gestor);

/**
 * Función para cambiar de directorio
 * @param dir
 * @return 0 si funcionó, 1 si dio error
 */
int cd(char* dir);

/**
 * Función para mostrar por salida estándar los procesos que se están ejecutando en segundo plano
 * @param gestor puntero al gestor del que se obtiene la lista
 */
void jobs(Gestor *gestor);


int estadoProceso(pid_t pid);



/**
 * Función para pasar un proceso a primer plano
 * @param pid id del proceso al que se quiere esperar
 * @param gestor puntero al gestor del que se obtiene la lista
 * @return 0 si funcionó, 1 si dio error
 */
int fg(pid_t pid, Gestor* gestor);

#endif //MINISHELL_COMANDOS_H
