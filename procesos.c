#include "procesos.h"

#include <sys/wait.h>
#include <stdio.h>

Gestor *nuevoGestor() {
    Gestor *gestor = (Gestor *) malloc(sizeof(Gestor));
    gestor->numProcesos = 0;
    gestor->proceso = (Proceso *) malloc(sizeof(Proceso *));
    gestor->procesosSegundoPlano = nuevaLista();
    return gestor;
}

void borrarGestor(Gestor *gestor) {
    free(gestor->proceso);
    borrarLista(gestor->procesosSegundoPlano);
    free(gestor);
}

Proceso *ejecutarProceso(tcommand comando, int *entrada, int *salida, int error) {
    pid_t pid = fork();
    if (pid == 0) {
        if (entrada != NULL) {
            close(entrada[1]);
            dup2(entrada[0], STDIN_FILENO);
        }
        if(salida != NULL){
            close(salida[0]);
            dup2(salida[1], STDOUT_FILENO);
        }
        dup2(error, STDERR_FILENO);

        execvp(comando.filename, comando.argv);

        fprintf(stderr, "Se ha producido un error ejecutando un mandato\n");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}