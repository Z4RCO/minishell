#include "comandos.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

void ejecutarComando(tcommand comando, Gestor* gestor) {
    if (!strcmp("cd", comando.argv[0])) {
        int check = cd(comando.argv[1]);
        if (check != 0)fprintf(stderr, "Error. La dirección proporcionada no es correcta.\n");
    }
    else if (!strcmp("jobs", comando.argv[0])) {
        jobs(gestor);
    }
    else if (!strcmp("fg", comando.argv[0])) {
        int a = -1;
        if(comando.argv[1] != NULL)a = atoi(comando.argv[1]);
        int check = fg(a, gestor);
        if (check == 1)fprintf(stderr, "Error. No hay ningún proceso en primer plano.\n");
        if (check == 2)fprintf(stderr, "Error. El pid proporcionado no es correcto.\n");
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


void jobs(Gestor *gestor) {
    Nodo* nodo = gestor->procesosSegundoPlano->punteroPrimerNodo;
    while(nodo !=  NULL){
        Proceso* proceso = nodo->elemento;
        int estaVivo = estadoProceso(proceso->pid);
        printf("Running\t\t");
        printf("%s ",proceso->linea);
        printf("\tpid: %d ",proceso->pid);
        if(estaVivo)printf("[Running]\n");
        else printf("[Dead]\n");
        nodo = nodo->siguienteNodo;
    }
}

int estadoProceso(pid_t pid){
    pid_t estado = waitpid(pid, NULL, WNOHANG);
    if(estado == 0)return 1;
    else return 0;
}

int fg(pid_t pid, Gestor *gestor) {
    Nodo* nodo = gestor->procesosSegundoPlano->punteroPrimerNodo;
    if(nodo == NULL){
        return 1;
    }
    if(pid < 0){
        Nodo* ultimoNodo = getNodo(gestor->procesosSegundoPlano, gestor->procesosSegundoPlano->n - 1);
        Proceso* proceso = (Proceso*)ultimoNodo->elemento;
        gestor->proceso = proceso;
        pid = proceso->pid;
        waitpid(pid,NULL,0);
        return 0;
    }

    int encontrado = 0;
    int i = 0;
    while(nodo != NULL && !encontrado){
        Proceso* proceso = nodo->elemento;
        if(proceso->pid == pid)encontrado = 1;
        else{
            nodo = nodo->siguienteNodo;
        }
        i++;
    }
    if(!encontrado)return 2;
    else{
        gestor->proceso = getNodo(gestor->procesosSegundoPlano, i - 1)->elemento;
        waitpid(pid, NULL, 0);
        return 0;
    }
}

