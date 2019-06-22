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

#define LONG_IDENTIFICADOR 8+1
#define LONG_MODELO 20+1


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
}ST_NODO, *PTR_NODO;

typedef struct{
	ST_NODO * frente;
	ST_NODO * fin;
}ST_COLA;


void inicializarAvion(ST_AVION *);

void registrarAvion(ST_AVION *, FILE *, char *);

void consultarEstadoAvion(ST_AVION *, FILE *, char *);

void crearCola(ST_COLA *);

void crearLista(PTR_NODO *);

ST_NODO * insertInFront(PTR_NODO *, ST_NODO *);


#endif /* HEADERS_AVION_H_ */
