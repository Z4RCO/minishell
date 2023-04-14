#include "comandos.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int ejecutarComando(tcommand comando){
    if(!strcmp("cd",comando.argv[0])){
        int check = cd(comando.argv[1]);
        if(check < 0)fprintf(stderr,"Error. La dirección proporcionada no es correcta.\n");
    }
    else if(!strcmp("jobs",comando.argv[0])){
    }
}

int cd(char* dir){
    if(dir == NULL){
        dir = getenv("HOME");
    }
    if(chdir(dir) < 0){
        return -1;
    }
    else{
        return 0;
    }
}

//TODO Mirar jobs
void jobs(Gestor* gestor){
    for (int i = 0; i < gestor->procesosSegundoPlano->n; i++) {
        Nodo *proceso = getNodo(gestor->procesosSegundoPlano,i);
        printf("%s", (char*)proceso->elemento);
    }
}

