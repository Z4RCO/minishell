#include <stdio.h>
#include "ListaEnlazada.h"
#include "stdlib.h"


Nodo *getNodo(ListaEnlazada *lista, int posicion) {
    Nodo *resultado = NULL;
    resultado = lista->punteroPrimerNodo;
    for (int i = 0; i < posicion; i++)
        resultado = resultado->siguienteNodo;

    return (resultado);
}


ListaEnlazada *nuevaLista() {
    ListaEnlazada *lista = (ListaEnlazada *) malloc(sizeof(ListaEnlazada));
    lista->punteroPrimerNodo = NULL;
    lista->n = 0;
    return (lista);
}


void insertar(ListaEnlazada *lista, int posicion, void *nuevoValor) {
    Nodo *nuevoNodo = (Nodo *) malloc(sizeof(Nodo));
    nuevoNodo->elemento = nuevoValor;


    if (lista->n == 0) {
        lista->punteroPrimerNodo = nuevoNodo;
        nuevoNodo->siguienteNodo = NULL;
    } else {
        if (posicion == 0) {
            nuevoNodo->siguienteNodo = lista->punteroPrimerNodo;
            lista->punteroPrimerNodo = nuevoNodo;

        } else if (posicion == lista->n) {

            nuevoNodo->siguienteNodo = NULL;
            getNodo(lista, lista->n - 1)->siguienteNodo = nuevoNodo;
        } else {
            Nodo *anterior = getNodo(lista, posicion - 1);
            Nodo *siguiente = anterior->siguienteNodo;

            anterior->siguienteNodo = nuevoNodo;
            nuevoNodo->siguienteNodo = siguiente;
        }

    }
    lista->n++;
}


void eliminar(ListaEnlazada *lista, int posicion) {
    Nodo *nodoAEliminar = NULL;


    if (posicion == 0) {
        nodoAEliminar = lista->punteroPrimerNodo;
        Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo;
        lista->punteroPrimerNodo = nodoSiguiente;
    } else {
        Nodo *nodoAnterior = getNodo(lista, posicion - 1);
        nodoAEliminar = nodoAnterior->siguienteNodo;
        Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo;
        nodoAnterior->siguienteNodo = nodoSiguiente;
    }

    free(nodoAEliminar);

    lista->n--;
}


void borrarLista(ListaEnlazada *lista) {
    while (lista->n > 0) eliminar(lista, 0);
    free(lista);
}



