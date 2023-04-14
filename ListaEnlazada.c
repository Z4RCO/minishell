#include "ListaEnlazada.h"
#include "assert.h"
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
	assert(lista != NULL & lista->n >= 0); // La lista existe y su tama�o tiene sentido
	assert(posicion >= 0 && posicion <= lista->n); 

	// Creamos el nuevo nodo con el nuevo valor
	Nodo *nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
	nuevoNodo->elemento = nuevoValor;

	// Si la lista estaba vacia...
	if (lista->n==0) {
		lista->punteroPrimerNodo = nuevoNodo;
		nuevoNodo->siguienteNodo = NULL;
	}

	// Si la lista no est� vac�a, distinguimos tres casos
	else {
		
		// Si estamos insertando al principio, el siguiente del insertado
		// es el que antes era el primero (apuntado por "punteroPrimerNodo")
		if (posicion == 0) {
			nuevoNodo->siguienteNodo = lista->punteroPrimerNodo;
			lista->punteroPrimerNodo = nuevoNodo;
		}

		// Si estamos a�adiendo al final de la lista
		else if (posicion == lista->n) {
			nuevoNodo->siguienteNodo = NULL;
			getNodo(lista, lista->n - 1)->siguienteNodo = nuevoNodo;
		}

		// En cualquier otro caso, hay un nodo
		// anterior y otro posterior
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

	// Si el nodo que queremos eliminar es el primero
	if (posicion == 0) {
		nodoAEliminar = lista->punteroPrimerNodo;
		Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo; // Obtenemos el siguiente, o NULL si no lo hay
		lista->punteroPrimerNodo = nodoSiguiente; // Apuntamos al que antes era el segundo nodo, o NULL si solo hab�a un nodo en la lista
	}

	// Si el nodo que queremos eliminar no es el primero
	// Nota que en este caso n>=2 pues si fuera n==1 ya se hubiera ejecutado el anterior caso
	// Nota tambi�n que este caso incluye el hecho de que el nodo a eliminar pueda ser el �ltimo
	else {
		Nodo *nodoAnterior = getNodo(lista, posicion-1); // Nodo anterior al que queremos eliminar
		nodoAEliminar = nodoAnterior->siguienteNodo; // Nodo que queremos eliminar
		Nodo *nodoSiguiente = nodoAEliminar->siguienteNodo; // Nodo siguiente al que queremos eliminar
		nodoAnterior->siguienteNodo = nodoSiguiente; // Hacemos que el anterior apunte al siguiente
	}

	// Liberamos la memoria del nodo
	free(nodoAEliminar);

	// Actualizamos n
	lista->n--;
}


void borrarLista(ListaEnlazada* lista) {
	while (lista->n > 0) eliminar(lista, 0);
	free(lista);
}



