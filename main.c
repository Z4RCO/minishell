#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "comandos.h"
#include "parser.h"
#include "gestorProcesos.h"

int main(void) {
    char buf[1024];
    tline *line;
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("msh %s> ", cwd);
    Gestor *gestorProcesos = nuevoGestor();
    while (fgets(buf, 1024, stdin)) {

        line = tokenize(buf);
        if (line == NULL || line->ncommands < 1) {
            continue;
        }
        for (int i = 0; i < line->ncommands; i++){
            if(line->commands[i].filename != NULL){

            }
            else{
                ejecutarComando(line->commands[i]);
            }





        }


            getcwd(cwd, sizeof(cwd));
        printf("msh %s> ", cwd);
    }

    return 0;
}