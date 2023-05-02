#pragma once
#ifndef MINISHELL_PROCESOS_H
#define MINISHELL_PROCESOS_H

#include <unistd.h>
#include <stdlib.h>

#include "ListaEnlazada.h"
#include "parser.h"

typedef struct{
    pid_t pid;
    int entrada;
    int salida;
    int error;
}Proceso;


typedef struct{
    Proceso * proceso;
    int numProcesos;
    ListaEnlazada *procesosSegundoPlano;
} Gestor;



Gestor* nuevoGestor();

void borrarGestor(Gestor * gestor);

Proceso * ejecutarProceso(tcommand comando, int* entrada, int* salida, int error);




#endif //MINISHELL_PROCESOS_H
