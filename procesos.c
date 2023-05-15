#include "procesos.h"

#include <sys/wait.h>
#include <stdio.h>

Gestor *nuevoGestor() {
    Gestor *gestor = (Gestor *) malloc(sizeof(Gestor));
    gestor->numProcesos = 0;
    gestor->procesosSegundoPlano = nuevaLista();
    return gestor;
}

void borrarGestor(Gestor *gestor) {
    free(gestor->proceso);
    borrarLista(gestor->procesosSegundoPlano);
    free(gestor);
}

Proceso *ejecutarProceso(tcommand comando, int *entrada, int *salida, int* error, pid_t gpid, Gestor* gestor, int plano, char* linea) {
    pid_t pid = fork();
    if(pid < 0){
        perror("Se ha producido un error haciendo el fork.\n");
        exit(-1);
    }
    if (pid == 0) {
        if(gpid != -1){
            setgid(gpid);
        }
        else{
            setgid(getpid());
        }
        if (entrada != NULL) {
            dup2(entrada[0], STDIN_FILENO);
            close(entrada[0]);
            close(entrada[1]);
        }
        if (salida != NULL) {
            dup2(salida[1], STDOUT_FILENO);
            close(salida[0]);
            close(salida[1]);
        }
        if (error != NULL) {
            dup2(error[1], STDERR_FILENO);
            close(error[0]);
            close(error[1]);
        }

        execvp(comando.filename, comando.argv);

        perror("Se ha producido un error ejecutando un mandato\n");
        exit(1);
    } else {
        Proceso* p = (Proceso*)malloc(sizeof(Proceso));
        p->pid = pid;
        p->linea = linea;
        if(gpid != -1){
            p->gpid = gpid;
        }
        else p->gpid = pid;

        if(plano == 0){
            free(gestor->proceso);
            gestor->proceso = p;
            waitpid(pid, NULL, 0);
            free(gestor->proceso);
            gestor->proceso = NULL;
        }
        else{
            insertar(gestor->procesosSegundoPlano,gestor->procesosSegundoPlano->n, p);
        }
    }
}