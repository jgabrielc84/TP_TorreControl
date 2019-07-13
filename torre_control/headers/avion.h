/* torre_control
 * avion.h
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#ifndef HEADERS_AVION_H_
#define HEADERS_AVION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define LONG_IDENTIFICADOR 8+1
#define LONG_MODELO 20+1
#define COMBUSTIBLE_ESTADO_CRITICO 10000


enum AVIONESTADO{
	AVION_HANGAR,
	AVION_LISTO_HANGAR,
	AVION_LISTO_DESPEGAR,
	AVION_DESPEGANDO,
	AVION_EN_VUELO,
	AVION_ESPERA_ATERRIZAR,
	AVION_ATERRIZANDO,
};

typedef struct{
	char identificador[LONG_IDENTIFICADOR];
	char modelo[LONG_MODELO];
	int combustibleActual;
	int combustibleMaximo;
	enum AVIONESTADO estado;
}ST_AVION;

typedef struct nodo{
	ST_AVION avion;
	struct nodo *siguiente;
}ST_NODOAVION, *PTR_NODOAVION;

typedef struct{
	PTR_NODOAVION * ptrListaAviones;
	PTR_NODOAVION * ptrListaAterrizaje;
	PTR_NODOAVION * ptrListaDespegue;
}ST_PTRLISTAS;


void inicializarAvion(ST_AVION *);

void registrarAvion(ST_AVION *, PTR_NODOAVION *, char *);

void ingresarAPista(ST_AVION *, PTR_NODOAVION *, char *);

void consultarEstadoAvion(ST_AVION *, PTR_NODOAVION *, char *);

void * gastarCombustible(PTR_NODOAVION *);

void * manejarEstados(ST_PTRLISTAS *);

void * administrarPista(ST_PTRLISTAS *);

void listaCrear(PTR_NODOAVION *);


#endif /* HEADERS_AVION_H_ */
