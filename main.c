#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#include "comandos.h"
#include "parser.h"
#include "procesos.h"

void signalHandler(int sig);

Gestor *gestorProcesos;

int main(void) {


    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);

    int **redireccion = (int **) malloc(3 * sizeof(int *));
    for (int i = 0; i < 3; ++i) {
        redireccion[i] = NULL;
    }

    char buf[1024];
    tline *line = NULL;
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("msh %s> ", cwd);
    gestorProcesos = nuevoGestor();
    Gestor* aux = gestorProcesos;
    while (fgets(buf, 1024, stdin)) {
        line = tokenize(buf);
        if (line == NULL || line->ncommands <= 0) {
            fflush(stderr);
            fflush(stdout);


            getcwd(cwd, sizeof(cwd));
            printf("msh %s> ", cwd);
            continue;
        }
        char *linea = (char *) malloc(1024 * sizeof(char));
        int j = 0;
        while (buf[j] != '\n') {
            linea[j] = buf[j];
            j++;
        }

        if (line->redirect_input != NULL) {
            redireccion[0] = (int *) malloc(2 * sizeof(int));
            pipe(redireccion[0]);
            int fds;
            fds = open(line->redirect_input, O_RDONLY);
            if (fds < 0) {
                fprintf(stderr,"%s. Error. El fichero no existe.\n", line->redirect_input);
                getcwd(cwd, sizeof(cwd));
                printf("msh %s> ", cwd);
                continue;
            }
            dup2(fds, redireccion[0][0]);
            close(fds);
        }
        if (line->redirect_error != NULL) {
            int fds;
            redireccion[2] = (int *) malloc(2 * sizeof(int));
            pipe(redireccion[2]);
            fds = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,"%s. Error. El fichero no existe.\n", line->redirect_input);
                getcwd(cwd, sizeof(cwd));
                printf("msh %s> ", cwd);
                continue;
            }
            dup2(fds, redireccion[2][1]);
            close(fds);
        }
        if (line->redirect_output != NULL) {
            int fds;
            redireccion[1] = (int *) malloc(2 * sizeof(int));
            pipe(redireccion[1]);
            fds = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IXUSR);
            if (fds < 0) {
                fprintf(stderr,"%s. Error. El fichero no existe.\n", line->redirect_input);
                getcwd(cwd, sizeof(cwd));
                printf("msh %s> ", cwd);
                continue;
            }
            dup2(fds, redireccion[1][1]);
            close(fds);
        }
        if (line->ncommands == 1) {
            Proceso *p = (Proceso *) malloc(sizeof(Proceso));
            if (line->commands[0].filename != NULL) {
                pid_t pid = ejecutarProceso(line->commands[0], redireccion[0], redireccion[1], redireccion[2], -1);
                p->pid = pid;
                p->gpid = pid;
                p->linea = linea;
                if (!line->background) {
                    p->pid = pid;
                    gestorProcesos->proceso = p;
                    waitpid(pid, NULL, 0);
                    free(gestorProcesos->proceso);
                } else {
                    insertar(gestorProcesos->procesosSegundoPlano, gestorProcesos->procesosSegundoPlano->n, p);
                }

            } else {
                ejecutarComando(line->commands[0], gestorProcesos);
            }

            for (int i = 0; i < 3; ++i) {
                if (redireccion[i] != NULL) {
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
            int error = 0;
            for (int i = 0; i < line->ncommands; i++) {
                if (line->commands[i].filename == NULL) {
                    error = 1;
                }
            }
            if (error) {
                fprintf(stderr, "Error. Alguno de los mandatos introducidos no es correcto.\n");
                fflush(stderr);
                fflush(stdout);

                getcwd(cwd, sizeof(cwd));
                printf("msh %s> ", cwd);
                continue;
            }

            int pipes[line->ncommands][2];
            for (int i = 0; i < line->ncommands; i++) {
                pipe(pipes[i]);
            }

            pid_t pid;
            Proceso *p =(Proceso*) malloc(line->ncommands * sizeof(Proceso));
            for (int i = 0; i < line->ncommands; i++) {
                if (i == 0) {
                    pid = ejecutarProceso(line->commands[i], redireccion[0], pipes[i], redireccion[2], -1);
                    p[i].pid = pid;
                    p[i].gpid = pid;
                    p[i].linea = linea;
                } else if (i == line->ncommands - 1) {
                    close(pipes[i - 1][1]);
                    pid = ejecutarProceso(line->commands[i], pipes[i - 1], redireccion[1], redireccion[2], p->gpid);
                    if (!line->background) {
                        gestorProcesos->proceso = p;
                        p[i].pid = pid;
                    } else {
                        p[i].pid = pid;
                        insertar(gestorProcesos->procesosSegundoPlano, gestorProcesos->procesosSegundoPlano->n, p);
                    }

                } else {
                    close(pipes[i - 1][1]);
                    p[0].pid = pid;
                    ejecutarProceso(line->commands[i], pipes[i - 1], pipes[i], redireccion[2], p->gpid);
                }
            }
            if(!line->background){
                for(int i = 0; i < line->ncommands; i++){
                    waitpid(gestorProcesos->proceso[i].pid, NULL, 0);
                }
            }

        }

        for (int i = 0; i < 3; ++i) {
            if (redireccion[i] != NULL) {
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
    Nodo *nodo = gestorProcesos->procesosSegundoPlano->punteroPrimerNodo;
    while (nodo != NULL) {
        Proceso *proceso = (Proceso *) nodo->elemento;
        free(proceso->linea);
        nodo = nodo->siguienteNodo;
    }
    borrarGestor(gestorProcesos);
    free(redireccion);

    return 0;
}

void signalHandler(int sig) {
    if (gestorProcesos->proceso != NULL) {
        killpg(gestorProcesos->proceso[0].gpid, sig);
    }
}