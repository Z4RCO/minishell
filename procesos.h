#pragma once
#ifndef MINISHELL_PROCESOS_H
#define MINISHELL_PROCESOS_H

#include <unistd.h>
#include <stdlib.h>

#include "ListaEnlazada.h"
#include "parser.h"

typedef struct{
    char* linea;
    pid_t gpid;
    pid_t pid;
}Proceso;


typedef struct{
    Proceso * proceso;
    int numProcesos;
    ListaEnlazada *procesosSegundoPlano;
} Gestor;


/**
 * Crear un nuevo gestor. Reserva la memoria necesaria
 * @return Puntero al gestor
 */
Gestor* nuevoGestor();

/**
 * Borrar un gestor. Libera toda la memoria asociada al gestor
 * @param gestor
 */
void borrarGestor(Gestor * gestor);

/**
 * Ejecutar un comando mediante un proceso duplicado.\n
 * Permite ejecutar el proceso con redirección de entrada, salida y error.\n
 * Permite ejecución en primer y segundo plano.\n
 * Permite establecer el group id del proceso\n
 * @param comando Comando que se quiere ejecutar
 * @param entrada pipe para la redirección de entrada. Si es NULL no hay redirección
 * @param salida pipe para la redirección de salida. Si es NULL no hay redirección
 * @param error pipe para la redirección de error. Si es NULL no hay redirección
 * @param gpid id del grupo
 * @return pid del proceso hijo
 */
pid_t ejecutarProceso(tcommand comando, int* entrada, int* salida, int* error, pid_t gpid);




#endif //MINISHELL_PROCESOS_H
