#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "comandos.h"
#include "parser.h"
#include "procesos.h"

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
        int pipes[line->ncommands][2];
        for(int i = 0; i < line->ncommands; i++){
            pipe(pipes[i]);
        }

        if(line->redirect_input != NULL){
            int fds;
            fds = open(line->redirect_input, O_RDONLY);
            if(fds < 0){
                fprintf(stderr, "Ha ocurrido un error al abrir el fichero de entrada. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 0);
        }
        if(line->redirect_error != NULL) {
            int fds;
            fds = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 2);
        }
        if(line->redirect_output != NULL) {
            int fds;
            fds = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 1);
        }
        ejecutarProceso(line->commands[0],line->background,0,pipes[0][1],2);
        for (int i = 1; i < line->ncommands - 1; i++){
            if(line->commands[i].filename != NULL){
                close(pipes[i][0]);
                ejecutarProceso(line->commands[i], line->background,pipes[i - 1][0],pipes[i][1],2 );
            }
            else{
                ejecutarComando(line->commands[i]);
            }





        }

        fflush(stderr);
        fflush(stdout);
        getcwd(cwd, sizeof(cwd));
        printf("msh %s> ", cwd);
    }

    return 0;
}