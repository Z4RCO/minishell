#pragma once

typedef struct Nodo {
	void* elemento;
	struct Nodo* siguienteNodo;
} Nodo;


typedef struct {
	Nodo* punteroPrimerNodo;
	int n;
} ListaEnlazada;


/**
 * Crea una lista enlazada. Reserva la memoria necesaria
 * @return Puntero a la lista
 */
ListaEnlazada *nuevaLista();

/**
 * Devuelve el nodo correspondiente a una posición
 * @param lista puntero a la lista
 * @param posicion posición del nodo
 * @return Nodo en dicha posición
 */
Nodo *getNodo (ListaEnlazada *lista, int posicion);



/**
 * insertar un nuevo elemento en la lista
 * @param lista puntero a la lista
 * @param posicion posición en la que insertar el elemento
 * @param nuevoValor Valor que se quiere insertar en la lista
 */
void insertar (ListaEnlazada *lista, int posicion, void* nuevoValor);

/**
 * Eliminar un elemento de la lista
 * @param lista puntero a la lista
 * @param posicion posición a eliminar de la lista
 */
void eliminar (ListaEnlazada *lista, int posicion);

/**
 * Borrar una lista. Libera la memoria asociada a la lista
 * @param lista puntero a la lista que se quiere borrar
 */
void borrarLista(ListaEnlazada *lista);








