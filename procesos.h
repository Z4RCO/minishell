#pragma once
#ifndef MINISHELL_PROCESOS_H
#define MINISHELL_PROCESOS_H

#include <unistd.h>
#include <stdlib.h>

#include "ListaEnlazada.h"
#include "parser.h"

typedef struct{
    int argc;
    char** argv;
    pid_t gpid;
    pid_t pid;
}Proceso;


typedef struct{
    Proceso * proceso;
    int numProcesos;
    ListaEnlazada *procesosSegundoPlano;
} Gestor;



Gestor* nuevoGestor();

void borrarGestor(Gestor * gestor);

Proceso * ejecutarProceso(tcommand comando, int* entrada, int* salida, int* error, pid_t gpid, Gestor* gestor, int plano);




#endif //MINISHELL_PROCESOS_H
