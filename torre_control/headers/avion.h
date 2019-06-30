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
	ST_NODOAVION * frente;
	ST_NODOAVION * fin;
}ST_COLA, *PTR_COLA;

typedef struct{
	PTR_NODOAVION ptrListaAviones;
	PTR_NODOAVION ptrListaAterrizaje;
	PTR_NODOAVION ptrColaDespegueFrente;
	PTR_NODOAVION ptrColaDespegueFin;
}ST_PTRLISTASCOLAS,*PTR_PTRLISTASCOLAS;


void inicializarAvion(ST_AVION *);

void registrarAvion(ST_AVION *, PTR_NODOAVION *, char *);

void ingresarAPista(ST_AVION *, PTR_NODOAVION *, char *);

void consultarEstadoAvion(ST_AVION *, PTR_NODOAVION *, char *);

void * gastarCombustible(PTR_NODOAVION *);

void * manejarEstados(ST_PTRLISTASCOLAS *);

void * administrarPista(ST_PTRLISTASCOLAS *);

void gastarcombustiblePRUEBA(PTR_NODOAVION *);

void manejarEstadosPRUEBA(PTR_NODOAVION *, PTR_NODOAVION *, ST_COLA *);

void administrarPistaPRUEBA(PTR_NODOAVION *, PTR_NODOAVION *, ST_COLA *);

void colaCrear(ST_COLA *);

//void colaAgregar(ST_COLA *, ST_AVION *);
//
//ST_AVION * colaRemover(ST_COLA *);

void listaCrear(PTR_NODOAVION *);

//ST_NODOAVION * listaInsertarEnFrente(PTR_NODOAVION *, ST_AVION *);
//
//ST_NODOAVION * listaInsertarAlFinal(PTR_NODOAVION *, ST_AVION *);
//
//ST_NODOAVION * listaBuscarAvion(PTR_NODOAVION *, ST_AVION *);
//
//ST_AVION listaBorrarPrimero(PTR_NODOAVION *);


#endif /* HEADERS_AVION_H_ */
