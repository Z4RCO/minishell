#include "procesos.h"

#include <sys/wait.h>
#include <stdio.h>

Gestor* nuevoGestor(){
    Gestor *gestor = (Gestor*)malloc(sizeof(Gestor));
    gestor->numProcesos = 0;
    gestor->proceso = (Proceso *)malloc(sizeof(Proceso*));
    gestor->procesosSegundoPlano = nuevaLista();
    return gestor;
}

void borrarGestor(Gestor * gestor){
    free(gestor->proceso);
    borrarLista(gestor->procesosSegundoPlano);
}

Proceso * ejecutarProceso(tcommand comando, int plano, int entrada, int salida, int error){
    pid_t pid = fork();
    if(pid == 0){

        execvp(comando.filename, comando.argv);
        fprintf(stderr, "Se ha producido un error ejecutando un mandato\n");
        exit(-1);
    }
    else{
        //TODO gestión de errores
        if(plano ==0){      //Primer plano
            int status;
            wait(&status);
        }
        else{
            Proceso *proceso = (Proceso *)malloc(sizeof(Proceso *));
            proceso->pid = pid;
            printf("Segundo plano\n");
            return proceso;
        }


    }
}