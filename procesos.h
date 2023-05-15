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
 * @param gestor puntero al gestor de procesos para añadir el proceso a la lista
 * @param plano Establece si el comando se ejecuta en primer o segundo plano. 0 para primer plano, 0 para segundo
 * @param linea String que contiene el nombre y los argumentos del comando
 * @return Puntero con información del proceso que se está ejecutando
 */
Proceso * ejecutarProceso(tcommand comando, int* entrada, int* salida, int* error, pid_t gpid, Gestor* gestor, int plano, char* linea);




#endif //MINISHELL_PROCESOS_H
