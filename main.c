#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "comandos.h"
#include "parser.h"
#include "procesos.h"

void signalHandler(int signal);

Gestor *gestorProcesos;

int main(void) {
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);

    int** redireccion = (int**)malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; ++i) {
        redireccion[i] = NULL;
    }

    char buf[1024];
    tline *line = NULL;
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("msh %s> ", cwd);
    gestorProcesos = nuevoGestor();
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
            redireccion[0] = (int*)malloc(2 * sizeof(int));
            pipe(redireccion[0]);
            int fds;
            fds = open(line->redirect_input, O_RDONLY);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de entrada. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, redireccion[0][1]);
            close(fds);
        }
        if (line->redirect_error != NULL) {
            redireccion[2] = (int*)malloc(2 * sizeof(int));
            pipe(redireccion[2]);
            int fds;
            fds = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, redireccion[2][0]);
            close(fds);
        }
        if (line->redirect_output != NULL) {
            int fds;
            redireccion[1] = (int*)malloc(2 * sizeof(int));
            pipe(redireccion[1]);
            fds = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,
                        "Ha ocurrido un error al abrir el fichero de error. Revisa que el nombre sea correccto\n");
                exit(-1);
            }
            dup2(fds, redireccion[1][0]);
            close(fds);
        }
        if (line->ncommands == 1) {
            if (line->commands[0].filename != NULL) {
                ejecutarProceso(line->commands[0], redireccion[0], redireccion[1], redireccion[2], -1);
            } else {
                ejecutarComando(line->commands[0]);
            }

            for (int i = 0; i < 3; ++i) {
                if(redireccion[i] != NULL){
                    close(redireccion[i][1]);
                    close(redireccion[i][0]);
                    free(redireccion[i]);
                    redireccion[i] = NULL;
                }
            }

            fflush(stderr);
            fflush(stdout);

            getcwd(cwd, sizeof(cwd));
            printf("msh %s> ", cwd);
            continue;
        } else {
            int pipes[line->ncommands][2];
            Proceso * p;
            for (int i = 0; i < line->ncommands; i++) {
                pipe(pipes[i]);
            }


            for (int i = 0; i < line->ncommands; i++) {
                if (i == 0) {
                    if (line->commands[i].filename != NULL) {
                        p = ejecutarProceso(line->commands[i], redireccion[0], pipes[i], redireccion[2], -1);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                } else if (i == line->ncommands - 1) {
                    close(pipes[i - 1][1]);
                    if (line->commands[i].filename != NULL) {
                        ejecutarProceso(line->commands[i], pipes[i - 1], redireccion[1], redireccion[2], p->gpid);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                } else {
                    close(pipes[i - 1][1]);
                    if (line->commands[i].filename != NULL) {
                        ejecutarProceso(line->commands[i], pipes[i - 1], pipes[i], redireccion[2], p->gpid);
                    } else {
                        ejecutarComando(line->commands[i]);
                    }
                }

            }
        }

        for (int i = 0; i < 3; ++i) {
            if(redireccion[i] != NULL){
                close(redireccion[i][1]);
                close(redireccion[i][0]);
                free(redireccion[i]);
                redireccion[i] = NULL;
            }
        }

        fflush(stderr);
        fflush(stdout);



        getcwd(cwd, sizeof(cwd));
        printf("msh %s> ", cwd);
    }

    borrarGestor(gestorProcesos);
    free(redireccion);

    return 0;
}

void signalHandler(int signal) {
    if(gestorProcesos->proceso->gpid != -1){
        killpg(gestorProcesos->proceso->gpid, signal);
    }
}

// echo hola > fichero.txt

//TODO acabar redirecciones
//TODO Hacer procesos en segundo plano
//TODO Acabar señales
//TODO Hacer gestión de errores
//TODO Hacer jobs y fg