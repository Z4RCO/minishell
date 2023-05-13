#include <stdio.h>
#include "ListaEnlazada.h"
#include "stdlib.h"


Nodo* getNodo(ListaEnlazada* lista, int posicion) {
	Nodo *resultado = NULL;
	resultado = lista->punteroPrimerNodo;
	for (int i=0; i<posicion; i++) 
		resultado = resultado->siguienteNodo;

	return (resultado);
}


ListaEnlazada* nuevaLista() {
	ListaEnlazada* lista = (ListaEnlazada*)malloc(sizeof(ListaEnlazada));
	lista->punteroPrimerNodo = NULL;
	lista->n=0;
	return(lista);
}





void insertar(ListaEnlazada* lista, int posicion, void * nuevoValor) {
	Nodo *nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
	nuevoNodo->elemento = nuevoValor;


	if (lista->n==0) {
		lista->punteroPrimerNodo = nuevoNodo;
		nuevoNodo->siguienteNodo = NULL;
	}


	else {
		if (posicion == 0) {
			nuevoNodo->siguienteNodo = lista->punteroPrimerNodo;
			lista->punteroPrimerNodo = nuevoNodo;
            printf("Se inserta un elemento\n");
		}


		else if (posicion == lista->n) {
			nuevoNodo->siguienteNodo = NULL;
			getNodo(lista, lista->n - 1)->siguienteNodo = nuevoNodo;
		}


		else {
			// Obtenemos los nodos anterior y siguiente al que queremos insertar
			Nodo *anterior = getNodo(lista, posicion - 1); // Nodo que estar� antes del que queremos insertar
			Nodo *siguiente = anterior->siguienteNodo; // Nodo que estar� despu�s del que queremos insertar

			// Hacemos que cada uno apunte al lugar adecuado
			anterior->siguienteNodo = nuevoNodo;
			nuevoNodo->siguienteNodo = siguiente;
		}

	} // Fin else

	// Incrementamos el numero de elementos
	lista->n++;
}


void eliminar(ListaEnlazada* lista, int posicion) {
	Nodo *nodoAEliminar = NULL;
    printf("Se inserta un elemento\n");


    //
	if (posicion == 0) {
		nodoAEliminar = lista->punteroPrimerNodo;
		Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo;
		lista->punteroPrimerNodo = nodoSiguiente;
	}


	else {
		Nodo *nodoAnterior = getNodo(lista, posicion-1);
		nodoAEliminar = nodoAnterior->siguienteNodo;
		Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo;
		nodoAnterior->siguienteNodo = nodoSiguiente;
	}

	free(nodoAEliminar);

	lista->n--;
}


void borrarLista(ListaEnlazada* lista) {
	while (lista->n > 0) eliminar(lista, 0);
	free(lista);
}



