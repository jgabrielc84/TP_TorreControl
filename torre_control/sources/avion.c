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

ST_NODOAVION * crearNodo(ST_AVION * avion){
	printf("*crearNodo*\n");

	ST_NODOAVION * avionNuevo = malloc(sizeof(ST_NODOAVION));

	inicializarAvion(&(avionNuevo->avion));

	strcpy(avionNuevo->avion.identificador, avion->identificador);
	strcpy(avionNuevo->avion.modelo, avion->modelo);
	avionNuevo->avion.combustibleActual = avion->combustibleActual;
	avionNuevo->avion.combustibleMaximo = avion->combustibleMaximo;
	avionNuevo->avion.estado = avion->estado;

	avionNuevo->siguiente = NULL;

	return avionNuevo;
}

ST_NODOAVION * listaInsertarAlFinal(PTR_NODOAVION * lista, ST_AVION * avion){
	printf("*listaInsertarAlFinal*\n");

	ST_NODOAVION * avionNuevo = crearNodo(avion);

	ST_NODOAVION * listaAux = *lista; // avanza hasta encontrar el último nodo de la lista
	while(listaAux != NULL && listaAux->siguiente != NULL){
		listaAux = listaAux->siguiente;
	}

	if(listaAux == NULL){ // contemplamos el caso de que la lista esté vacía.
		*lista = avionNuevo;
	}else{
		listaAux->siguiente = avionNuevo; //insertamos el nuevo nodo al final de la lista
	}
	return avionNuevo;
}


ST_NODOAVION * listaBuscarAvion(PTR_NODOAVION * lista, ST_AVION * avion){
	printf("*listaBuscarAvion*\n");

	ST_NODOAVION * listaAux = *lista;

	while(listaAux && (strcmp(listaAux->avion.identificador, avion->identificador) != 0)){
		listaAux = listaAux->siguiente;
	}

	return listaAux;
}

ST_NODOAVION * listainsertarSinDuplicados(PTR_NODOAVION * lista, ST_AVION * avion){ //inserta al Final de la lista en caso que no este el avion
	printf("*listaBuscarAvion*\n");

	ST_NODOAVION * nodo = listaBuscarAvion(lista, avion);

	if(nodo == NULL){
		nodo = listaInsertarAlFinal(lista, avion);
	}
	return nodo;
}

void listaCrear(PTR_NODOAVION * lista){
	printf("*crearLista*\n");

	*lista = NULL;
}

ST_NODOAVION * listaInsertarEnFrente(PTR_NODOAVION * lista, ST_AVION * avion){
	printf("*listaInsertarEnFrente*\n");

	ST_NODOAVION * nodoAvionNuevo = crearNodo(avion);

	//insertamos el nuevo nodo en la lista
	nodoAvionNuevo->siguiente = *lista;
	*lista = nodoAvionNuevo;
	return nodoAvionNuevo;
}

ST_AVION listaBorrarPrimero(PTR_NODOAVION * lista){
	printf("*listaBorrarPrimero*\n");

	ST_NODOAVION * nodoAux = *lista;
	ST_AVION avion;

	*lista = nodoAux->siguiente;

	strcpy(avion.identificador, nodoAux->avion.identificador);
	strcpy(avion.modelo, nodoAux->avion.modelo);
	avion.combustibleActual = nodoAux->avion.combustibleActual;
	avion.combustibleMaximo = nodoAux->avion.combustibleMaximo;
	avion.estado = nodoAux->avion.estado;

	free(nodoAux);

	return avion;
}

ST_AVION listaBorrarAvion(PTR_NODOAVION * lista, ST_AVION * avion){
	ST_NODOAVION * listaAux = *lista;
	ST_NODOAVION * nodoAnt = NULL;
	ST_AVION avionAux;

	while (listaAux && strcmp(listaAux->avion.identificador, avion->identificador) != 0){
		nodoAnt = listaAux;
		listaAux = listaAux->siguiente;
	}

	// se elimina el primer nodo
	if(nodoAnt == NULL && *lista != NULL){
		*lista = (*lista)->siguiente;
	}else{
		nodoAnt->siguiente = listaAux->siguiente;
	}

	strcpy(avionAux.identificador, listaAux->avion.identificador);
	strcpy(avionAux.modelo, listaAux->avion.modelo);
	avionAux.combustibleActual = listaAux->avion.combustibleActual;
	avionAux.combustibleMaximo = listaAux->avion.combustibleMaximo;
	avionAux.estado = listaAux->avion.estado;

	free(listaAux);

	return avionAux;
}

void registrarAvion(ST_AVION * avion, PTR_NODOAVION * listaAviones, char * msjCliente){
	printf("*registrarAvion*\n");

	ST_NODOAVION * nodo = listaBuscarAvion(listaAviones, avion);

	if(nodo == NULL){
		listaInsertarAlFinal(listaAviones, avion);
		formatearMensaje(msjCliente, avion, "Avion correctamente ingresado a torre de control");
	}else{
		formatearMensaje(msjCliente, avion, "El avion ya esta registrado en torre de control");
	}
}

void ingresarAPista(ST_AVION * avion, PTR_NODOAVION * listaAviones, char * msjCliente){
	printf("*ingresarAPista*\n");

	int opcion = 0;

	ST_NODOAVION * nodo = listaBuscarAvion(listaAviones, avion);

	if(nodo != NULL){
		opcion = nodo->avion.estado;
	}

	switch(opcion){
	case AVION_LISTO_HANGAR:
		nodo->avion.estado = AVION_LISTO_DESPEGAR;
		formatearMensaje(msjCliente, avion, "El estado del avion paso a LISTO_DESPEGAR");
		break;
	case AVION_EN_VUELO:
		nodo->avion.estado = AVION_ESPERA_ATERRIZAR;
		formatearMensaje(msjCliente, avion, "El estado del avion paso a ESPERA_ATERRIZAR");
		break;
	default:
		formatearMensaje(msjCliente, avion, "El avion no esta registrado o no esta en estado \"LISTO / HANGAR\" o \"EN VUELO\"");
		break;
	}
}

void consultarEstadoAvion(ST_AVION * avion, PTR_NODOAVION * listaAviones, char * msjCliente){
	printf("*consultarEstadoAvion*\n");

	ST_NODOAVION * nodo = listaBuscarAvion(listaAviones, avion);

	if(nodo == NULL){
		formatearMensaje(msjCliente, avion, "El avion solicitado no se encuentra en torre de control");
	}else{
		formatearMensaje(msjCliente, &(nodo->avion), "El estado del avion es el siguiente:");
	}
}

void * gastarCombustible(PTR_NODOAVION * listaAviones){
	printf("*hilo gastarCombustible*\n");

	pthread_mutex_t lock;
	PTR_NODOAVION listaAux = NULL;

	while(TRUE){
		pthread_mutex_lock(&lock);
		listaAux = *listaAviones;

		while(listaAux != NULL){
			if(listaAux->avion.estado == AVION_DESPEGANDO || listaAux->avion.estado == AVION_EN_VUELO ||
				listaAux->avion.estado == AVION_ESPERA_ATERRIZAR || listaAux->avion.estado == AVION_ATERRIZANDO){
				listaAux->avion.combustibleActual -= 100; // Gasta 100 de combustible de cada avion
			}

			if(listaAux->avion.estado == AVION_HANGAR && listaAux->avion.combustibleActual < listaAux->avion.combustibleMaximo){
				listaAux->avion.combustibleActual += 1000; // Carga 1000 de combustible cuando esta en HANGAR
				if(listaAux->avion.combustibleActual > listaAux->avion.combustibleMaximo){
					listaAux->avion.combustibleActual = listaAux->avion.combustibleMaximo;
				}
			}

			if(listaAux->avion.estado == AVION_EN_VUELO && listaAux->avion.combustibleActual < COMBUSTIBLE_ESTADO_CRITICO){ // Comprueba que el combustible del avion no este en estado Critico
				printf("El avion:\n");
				printf("\tIdentificador: %s", listaAux->avion.identificador);
				printf("\tModelo: %s", listaAux->avion.modelo);
				printf("\tCombustible actual: %d", listaAux->avion.combustibleActual);
				printf("Tiene poco combustible, se lo coloca en listaAterrizaje");

				listaAux->avion.estado = AVION_ESPERA_ATERRIZAR; //Se coloca en AVION_ESPERA_ATERRIZAR ya que la funcion manejarEstados lo coloca en la listaAterrizaje
			}

			listaAux = listaAux->siguiente;
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}

	return NULL;
}

void * manejarEstados(ST_PTRLISTAS * ptrListas){
	printf("*hilo manejarEstados*\n");

	pthread_mutex_t lock;
	int opcion = 0;
	PTR_NODOAVION listaAvionesAux = NULL;
	ST_NODOAVION * avionNodo = NULL;

	while(TRUE){
		pthread_mutex_lock(&lock);
		listaAvionesAux = *ptrListas->ptrListaAviones;

		while(listaAvionesAux != NULL){ // revisa todos los aviones
			opcion = listaAvionesAux->avion.estado;
			switch(opcion){
			case AVION_HANGAR: //chequea que tenga combustible completo para pasarlo a AVION_LISTO_HANGAR
				if(listaAvionesAux->avion.combustibleActual == listaAvionesAux->avion.combustibleMaximo){
					listaAvionesAux->avion.estado = AVION_LISTO_HANGAR;
				}
				break;
			case AVION_LISTO_DESPEGAR://coloca el avion en listaDespegue
				avionNodo = listaBuscarAvion(ptrListas->ptrListaDespegue, &listaAvionesAux->avion);

				if(avionNodo == NULL){
					listaInsertarAlFinal(ptrListas->ptrListaDespegue, &listaAvionesAux->avion);
				}
				break;
			case AVION_ESPERA_ATERRIZAR: //coloca el avion en listaAterrizaje
				avionNodo = listaBuscarAvion(ptrListas->ptrListaAterrizaje, &listaAvionesAux->avion);

				if(avionNodo == NULL){
					listaInsertarAlFinal(ptrListas->ptrListaAterrizaje, &listaAvionesAux->avion);
				}
				break;
			default:
				break;
			}

			listaAvionesAux = listaAvionesAux->siguiente;
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}

	return NULL;
}

void * administrarPista(ST_PTRLISTAS * ptrListas){
	printf("*hilo administrarPista*\n");

	pthread_mutex_t lock;
	PTR_NODOAVION listaAterrizajeAux = NULL;
	PTR_NODOAVION listaDespegueAux = NULL;
	ST_NODOAVION * avionNodo = NULL;
	ST_NODOAVION * avionCritico = NULL;
	ST_NODOAVION * avionCriticoMenor = NULL;
	ST_AVION avion;

	while(TRUE){
		pthread_mutex_lock(&lock);
		avionNodo = NULL;
		avionCritico = NULL;
		avionCriticoMenor = NULL;
		listaAterrizajeAux = *ptrListas->ptrListaAterrizaje;
		listaDespegueAux = *ptrListas->ptrListaDespegue;

		if(listaAterrizajeAux != NULL){
			avionCritico = listaAterrizajeAux;
			avionCriticoMenor = listaAterrizajeAux;
		}

		while(listaAterrizajeAux != NULL){ //Busca si hay aviones en ESPERA / ATERRIZAR
			if((listaAterrizajeAux)->avion.combustibleActual < COMBUSTIBLE_ESTADO_CRITICO){
				avionCritico = listaAterrizajeAux;
				if(avionCritico->avion.combustibleActual < avionCriticoMenor->avion.combustibleActual){
					avionCriticoMenor->avion.combustibleActual = avionCritico->avion.combustibleActual;
				}
			}

			listaAterrizajeAux = listaAterrizajeAux->siguiente;
		}

		if(avionCriticoMenor != NULL){ //salen aviones de listaAterrizaje
			avionNodo = listaBuscarAvion(ptrListas->ptrListaAviones, &(avionCriticoMenor->avion));
			avionNodo->avion.estado = AVION_ATERRIZANDO;
			sleep(5);
			avionNodo->avion.estado =AVION_HANGAR;
			listaBorrarAvion(ptrListas->ptrListaAterrizaje, &(avionCriticoMenor->avion));
		}else{
			if(listaDespegueAux != NULL){ //salen aviones de listaDespegue
				avion = listaBorrarPrimero(ptrListas->ptrListaDespegue);
				avionNodo = listaBuscarAvion(ptrListas->ptrListaAviones, &avion);
				avionNodo->avion.estado = AVION_DESPEGANDO;
				sleep(5);
				avionNodo->avion.estado = AVION_EN_VUELO;
			}
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}

	return NULL;
}
