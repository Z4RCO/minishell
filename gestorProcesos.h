#pragma once
#ifndef MINISHELL_GESTORPROCESOS_H
#define MINISHELL_GESTORPROCESOS_H

#include <unistd.h>
#include <stdlib.h>

#include "ListaEnlazada.h"

typedef struct{
    pid_t proceso;
    int numProcesos;
    ListaEnlazada *procesosSegundoPlano;
} Gestor;

Gestor* nuevoGestor();




#endif //MINISHELL_GESTORPROCESOS_H
