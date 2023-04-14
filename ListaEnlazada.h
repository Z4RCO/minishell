#pragma once

typedef struct Nodo {
	void* elemento;
	struct Nodo* siguienteNodo;
} Nodo;


typedef struct {
	Nodo* punteroPrimerNodo;
	int n;
} ListaEnlazada;


Nodo *getNodo (ListaEnlazada *lista, int posicion);


ListaEnlazada *nuevaLista();


void insertar (ListaEnlazada *lista, int posicion, void* nuevoValor);


void eliminar (ListaEnlazada *lista, int posicion);


void borrarLista(ListaEnlazada *lista);








