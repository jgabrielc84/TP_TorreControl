/* torre_control
 * avion.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/avion.h"
#include "../headers/mensaje.h"
#include "../headers/archivo.h"


void inicializarAvion(ST_AVION * avion){
	memset(avion->modelo, '\0', LONG_MODELO);
	memset(avion->identificador, '\0', LONG_IDENTIFICADOR);
	avion->estado = AVION_HANGAR;
	avion->combustibleActual = 0;
	avion->combustibleMaximo = 0;
}

void registrarAvion(ST_AVION * avion, FILE * ptrArchivoTorreControl, char * msjCliente){
	printf("*registrarAvion*\n");

	int existeAvion = FALSE;
	int posicionVaciaPtr = 0;

	existeAvion = comprobarExisteAvion(ptrArchivoTorreControl, avion);

	if(existeAvion == FALSE){
		posicionVaciaPtr = buscarPosVacia(ptrArchivoTorreControl);

		if(posicionVaciaPtr == -1){ //no hay posicion vacia, se escribe al final del archivo
			fseek(ptrArchivoTorreControl, 0, SEEK_END);
		}else{
			fseek(ptrArchivoTorreControl, (sizeof(ST_AVION) * posicionVaciaPtr), SEEK_SET);
		}

		fwrite(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl);

		formatearMensaje(msjCliente, avion, "Avion correctamente ingresado a torre de control");
	}else{ //existe
		formatearMensaje(msjCliente, avion, "El avion ya esta registrado en torre de control");
	}
}

void consultarEstadoAvion(ST_AVION * avion, FILE * ptrArchivoTorreControl, char * msjCliente){
	printf("*consultarEstadoAvion*\n");

	int posicionAvionArchivo = 0;

	posicionAvionArchivo = buscarPosAvion(ptrArchivoTorreControl, avion);

	if(posicionAvionArchivo >= 0){
		leerPosArchivo(ptrArchivoTorreControl, &posicionAvionArchivo, avion);

		formatearMensaje(msjCliente, avion, "El estado del avion es el siguiente:\n\n");
	}else{
		formatearMensaje(msjCliente, avion, "El avion solicitado no se encuentra en torre de control\n\n");
	}
}


void crearCola(ST_COLA * cola){
	cola->frente = NULL;
	cola->fin = NULL;
}

void crearLista(PTR_NODO * lista){
	*lista = NULL;
}

/*ST_NODO * insertInFront(PTR_NODO * lista, ST_NODO * dato){
	// creación del nodo
	ST_NODO *nodo = malloc(sizeof(ST_NODO));
	//nodo->avion = avion;  // copiar avion
	nodo->siguiente = NULL;

	//insertamos el nuevo nodo en la lista
	nodo->siguiente = *list;
	*list = nodo;
	return nodo;
}*/
