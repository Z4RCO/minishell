#include "comandos.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

int ejecutarComando(tcommand comando, Gestor* gestor) {
    if (!strcmp("cd", comando.argv[0])) {
        int check = cd(comando.argv[1]);
        if (check < 0)fprintf(stderr, "Error. La dirección proporcionada no es correcta.\n");
    }
    else if (!strcmp("jobs", comando.argv[0])) {
        jobs(gestor);
    }
    else if (!strcmp("fg", comando.argv[0])) {
        int a = 0;
        if(comando.argv[1] != NULL)a = atoi(comando.argv[1]);
        int check = fg(a, gestor);
        if (check < 0)fprintf(stderr, "Error. El pid proporcionado no es correcto.\n");
    }
    else{
        fprintf(stderr, "%s: No se encuentra el mandato.\n", comando.argv[0]);
    }

}

int cd(char *dir) {
    if (dir == NULL) {
        dir = getenv("HOME");
    }
    if (chdir(dir) < 0) {
        return -1;
    } else {
        return 0;
    }
}

//TODO Mirar jobs
void jobs(Gestor *gestor) {
    for (int i = 0; i < gestor->procesosSegundoPlano->n; i++) {
        Nodo *nodo = getNodo(gestor->procesosSegundoPlano, i);
        Proceso* proceso = nodo->elemento;
        printf("Running ");
        for (int j = 0; j < proceso->argc; ++j) {
            printf("%s ",proceso->argv[i]);
        }
        printf("pid: %d\n",proceso->pid);
    }
}

//TODO Acabar fg
int fg(pid_t pid, Gestor *gestor) {
    Nodo* nodo = gestor->procesosSegundoPlano->punteroPrimerNodo;
    Proceso* proceso = nodo->elemento;
    while( pid != proceso->pid){
        if(nodo == NULL){
            return 1;
        }

        proceso = nodo->elemento;
        nodo = nodo->siguienteNodo;
    }
    waitpid(pid, NULL, 0);
    return 0;
}

