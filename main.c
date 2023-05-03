#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "comandos.h"
#include "parser.h"
#include "procesos.h"

void signalHandler();


int main(void) {
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);

    int entradaFD = dup(STDIN_FILENO);
    int salidaFD = dup(STDOUT_FILENO);
    int errorFD = dup(STDERR_FILENO);
    char buf[1024];
    tline *line = NULL;
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("msh %s> ", cwd);
    Gestor *gestorProcesos = nuevoGestor();
    while (fgets(buf, 1024, stdin)) {

        line = tokenize(buf);
        if (line == NULL || line->ncommands <= 0) {
            fflush(stderr);
            fflush(stdout);


            getcwd(cwd, sizeof(cwd));
            printf("msh %s> ", cwd);
            continue;
        }

        if (line->redirect_input != NULL) {
            int fds;
            fds = open(line->redirect_input, O_RDONLY);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de entrada. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 0);
        }
        if (line->redirect_error != NULL) {
            int fds;
            fds = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 2);
        }
        if (line->redirect_output != NULL) {
            int fds;
            fds = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, 1);
        }

        if (line->ncommands == 1) {
            if (line->commands[0].filename != NULL) {
                ejecutarProceso(line->commands[0], NULL, NULL, 2);
            } else {
                ejecutarComando(line->commands[0]);
            }
            fflush(stderr);
            fflush(stdout);
            getcwd(cwd, sizeof(cwd));
            printf("msh %s> ", cwd);
            continue;
        } else {
            int pipes[line->ncommands][2];
            for (int i = 0; i < line->ncommands; i++) {
                pipe(pipes[i]);
            }


            for (int i = 0; i < line->ncommands; i++) {
                if (i == 0) {
                    if (line->commands[i].filename != NULL) {
                        ejecutarProceso(line->commands[i], NULL, pipes[i], 2);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                } else if (i == line->ncommands - 1) {
                    if (line->commands[i].filename != NULL) {
                        ejecutarProceso(line->commands[i], pipes[i - 1], NULL, 2);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                } else {
                    if (line->commands[i].filename != NULL) {
                        ejecutarProceso(line->commands[i], pipes[i - 1], pipes[i], 2);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                }
            }


            for (int i = 0; i < line->ncommands; i++) {
                for (int j = 0; i < 2; i++) {
                    close(pipes[i][j]);
                }
            }
        }


        fflush(stderr);
        fflush(stdout);

        dup2(entradaFD, STDIN_FILENO);
        dup2(salidaFD, STDOUT_FILENO);
        dup2(errorFD, STDERR_FILENO);

        getcwd(cwd, sizeof(cwd));
        fprintf(stdout, "msh %s> ", cwd);
    }

    return 0;
}

void signalHandler() {
}

// find ./ -name *.txt | grep fichero

//TODO acabar redirecciones
//TODO Hacer procesos en segundo plano
//TODO Acabar señales
//TODO Hacer gestión de errores
//TODO Hacer jobs y fg
//TODO Arreglar Tuberías