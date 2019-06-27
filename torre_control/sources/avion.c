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
		formatearMensaje(msjCliente, avion, "El avion no esta registrado o no se en estado LISTO_HANGAR/EN_VUELO");
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
	ST_NODOAVION * listaAux = NULL;

	while(TRUE){
		listaAux = *listaAviones;

		if(listaAux != NULL){
			while(listaAux != NULL){
				if(listaAux->avion.estado == AVION_DESPEGANDO || listaAux->avion.estado == AVION_EN_VUELO ||
					listaAux->avion.estado == AVION_ESPERA_ATERRIZAR || listaAux->avion.estado == AVION_ATERRIZANDO){
					listaAux->avion.combustibleActual -= 100; // Gasta 100 de combustible de cada avion
				}

				if(listaAux->avion.estado == AVION_HANGAR && listaAux->avion.combustibleActual < listaAux->avion.combustibleMaximo){
					listaAux->avion.combustibleActual += 1000; // Carga 1000 de combustible cuando esta en HANGAR
				}

				if(listaAux->avion.estado == AVION_EN_VUELO){
					if(listaAux->avion.combustibleActual < COMBUSTIBLE_ESTADO_CRITICO){ // Comprueba que el combustible del avion no este en estado Critico
						printf("El avion:\n");
						printf("\tIdentificador: %s", listaAux->avion.identificador);
						printf("\tModelo: %s", listaAux->avion.modelo);
						printf("\tCombustible actual: %d", listaAux->avion.combustibleActual);
						printf("Tiene poco combustible, se lo coloca en listaAterrizaje");


						pthread_mutex_lock(&lock);

						listaAux->avion.estado = AVION_ESPERA_ATERRIZAR; //Se coloca en AVION_ESPERA_ATERRIZAR ya que la funcion manejarEstados lo coloca en la lista

						pthread_mutex_unlock(&lock);
					}
				}
				listaAux = listaAux->siguiente;
			}
		}
//		}else{
//			printf("listaAviones esta vacia.");
//		}

		sleep(1);
	}

	return NULL;
}

void * manejarEstados(PTR_NODOAVION * listaAviones, PTR_NODOAVION * listaAterrizaje, ST_COLA * colaDespegue){
	printf("*hilo manejarEstados*\n");

	pthread_mutex_t lock;
	int opcion = 0;
	ST_NODOAVION * listaAux = NULL;

	while(TRUE){
		listaAux = *listaAviones;

		if(listaAux != NULL){
			while(listaAux != NULL){
				opcion = listaAux->avion.estado;
				switch(opcion){
				case AVION_HANGAR://chequea que tenga combustible completo para pasarlo a AVION_LISTO_HANGAR
					if(listaAux->avion.combustibleActual == listaAux->avion.combustibleMaximo){
						pthread_mutex_lock(&lock);
						listaAux->avion.estado = AVION_LISTO_HANGAR;
						pthread_mutex_unlock(&lock);
					}
					break;
				case AVION_LISTO_DESPEGAR://coloca el avion en ColaDespegue
					pthread_mutex_lock(&lock);
					colaAgregar(colaDespegue, &(listaAux->avion));
					pthread_mutex_unlock(&lock);
					break;
				case AVION_ESPERA_ATERRIZAR:
					//coloca el avion en listaAterrizaje
					pthread_mutex_lock(&lock);
					listaInsertarAlFinal(listaAterrizaje, &(listaAux->avion));
					pthread_mutex_unlock(&lock);
					break;
				default:
					break;
				}

				listaAux = listaAux->siguiente;
			}
		}

		sleep(1);
	}

	return NULL;
}

void * administrarPista(ST_PTRLISTASCOLAS * ptrlistascolas){
	printf("*hilo administrarPista*\n");

	// revisa 1ero la lista de aterrizaje en busca de alguno que tenga estado critico

	//			case AVION_DESPEGANDO:
	//					//levanta flag para que no puedan usar la pista???
	//				break;
	//			case AVION_ATERRIZANDO:
	//					//levanta flag para que no puedan usar la pista???
	//				break;
	//funcion que habilita a solo 1 avion a que utilice la pista
	//levanta flag para que no puedan usar la pista???
	return NULL;
}

void gastarcombustiblePRUEBA(PTR_NODOAVION * listaAviones){
	printf("*gastarcombustiblePRUEBA*\n");

	ST_NODOAVION * listaAux = NULL;

	while(listaAux != NULL){
		if(listaAux->avion.estado == AVION_DESPEGANDO || listaAux->avion.estado == AVION_EN_VUELO ||
			listaAux->avion.estado == AVION_ESPERA_ATERRIZAR || listaAux->avion.estado == AVION_ATERRIZANDO){
			listaAux->avion.combustibleActual -= 100; // Gasta 100 de combustible de cada avion
		}

		if(listaAux->avion.estado == AVION_HANGAR && listaAux->avion.combustibleActual < listaAux->avion.combustibleMaximo){
			listaAux->avion.combustibleActual += 1000; // Carga 1000 de combustible cuando esta en HANGAR
		}

		if(listaAux->avion.estado == AVION_EN_VUELO && listaAux->avion.combustibleActual < COMBUSTIBLE_ESTADO_CRITICO){ // Comprueba que el combustible del avion no este en estado Critico
			printf("El avion:\n");
			printf("\tIdentificador: %s", listaAux->avion.identificador);
			printf("\tModelo: %s", listaAux->avion.modelo);
			printf("\tCombustible actual: %d", listaAux->avion.combustibleActual);
			printf("Tiene poco combustible, se lo coloca en listaAterrizaje");

			listaAux->avion.estado = AVION_ESPERA_ATERRIZAR; //Se coloca en AVION_ESPERA_ATERRIZAR ya que la funcion manejarEstados lo coloca en la lista
		}
		listaAux = listaAux->siguiente;
	}
}

void manejarEstadosPRUEBA(PTR_NODOAVION * listaAviones, PTR_NODOAVION * listaAterrizaje, ST_COLA * colaDespegue){
	printf("*manejarEstadosPRUEBA*\n");

	int opcion = 0;
	ST_NODOAVION * listaAux = NULL;

	listaAux = *listaAviones;

	while(listaAux != NULL){
		opcion = listaAux->avion.estado;
		switch(opcion){
		case AVION_HANGAR://chequea que tenga combustible completo para pasarlo a AVION_LISTO_HANGAR
			if(listaAux->avion.combustibleActual == listaAux->avion.combustibleMaximo){
				listaAux->avion.estado = AVION_LISTO_HANGAR;
			}
			break;
		case AVION_LISTO_DESPEGAR://coloca el avion en ColaDespegue
			colaAgregar(colaDespegue, &(listaAux->avion));
			break;
		case AVION_ESPERA_ATERRIZAR:
			//coloca el avion en listaAterrizaje
			listaInsertarAlFinal(listaAterrizaje, &(listaAux->avion));
			break;
		default:
			break;
		}

		listaAux = listaAux->siguiente;
	}
}

void administrarPistaPRUEBA(){

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

void colaCrear(ST_COLA * cola){
	printf("*colaCrear*\n");

	cola->frente = NULL;
	cola->fin = NULL;
}

void colaAgregar(ST_COLA * cola, ST_AVION * avion){
	printf("*colaAgregar*\n");

	ST_NODOAVION * nodoAvionNuevo = crearNodo(avion);

	if(cola->frente == NULL){
		cola->frente = nodoAvionNuevo;
	}else{
		cola->fin->siguiente = nodoAvionNuevo;
	}

	cola->fin = nodoAvionNuevo;
}

ST_AVION * colaRemover(ST_COLA * cola){
	printf("*colaRemover*\n");

	ST_NODOAVION *nodoAux = cola->frente;
	ST_AVION * avionAux = malloc(sizeof(ST_AVION));

	strcpy(avionAux->identificador, nodoAux->avion.identificador);
	strcpy(avionAux->modelo, nodoAux->avion.modelo);
	avionAux->combustibleActual = nodoAux->avion.combustibleActual;
	avionAux->combustibleMaximo = nodoAux->avion.combustibleMaximo;
	avionAux->estado = nodoAux->avion.estado;

	cola->frente = nodoAux->siguiente;

	if(cola->frente == NULL){
		cola->fin = NULL;
	}

	free(nodoAux);

	return avionAux;
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

	while(listaAux && strcmp(listaAux->avion.identificador, avion->identificador) != 0){
		listaAux = listaAux->siguiente;
	}

	return listaAux;
}

ST_AVION * listaBorrarPrimero(PTR_NODOAVION * lista){
	printf("*listaBorrarPrimero*\n");

	ST_NODOAVION * nodoAux = *lista;
	ST_AVION * avion = malloc(sizeof(ST_AVION));

	*lista = nodoAux->siguiente;

	strcpy(avion->identificador, nodoAux->avion.identificador);
	strcpy(avion->modelo, nodoAux->avion.modelo);
	avion->combustibleActual = nodoAux->avion.combustibleActual;
	avion->combustibleMaximo = nodoAux->avion.combustibleMaximo;
	avion->estado = nodoAux->avion.estado;

	free(nodoAux);

	return avion;
}
