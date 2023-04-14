#include "gestorProcesos.h"


Gestor* nuevoGestor(){
    Gestor *gestor = (Gestor*)malloc(sizeof(Gestor));
    gestor->numProcesos = 0;
    gestor->proceso = (pid_t) NULL;
    gestor->procesosSegundoPlano = nuevaLista();
    return gestor;
}

