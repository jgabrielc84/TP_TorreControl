/* torre_control
 * mensaje.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/mensaje.h"


void liberaBuffer(){
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

void inicializarMsjCliente(char * msjCliente){
	memset(msjCliente, '\0', LONG_MSJ_CLIE);
}

void concatenarMsjServidor(char * msjServidor, const ST_AVION * avion,const char * combustibleAvion, const char * estadoAvion, const char * opcionAvion){
	strcpy(msjServidor, avion->identificador);
	strcat(msjServidor, "|");
	strcat(msjServidor, avion->modelo);
	strcat(msjServidor, "|");
	strcat(msjServidor, combustibleAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, estadoAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, opcionAvion);
}

void formatearMensaje(char * msjServidor, const ST_AVION * avion, const int * opcion){
	char * combustibleAvion = malloc(sizeof(char)*LONG_COMBUSTIBLEAVION);
	char * estadoAvion = malloc(sizeof(char)*LONG_ESTADOAVION);
	char * opcionAvion = malloc(sizeof(char)*LONG_OPCIONAVION);
	memset(combustibleAvion, '\0', LONG_COMBUSTIBLEAVION);
	memset(estadoAvion, '\0', LONG_ESTADOAVION);
	memset(opcionAvion, '\0', LONG_OPCIONAVION);
	inicializarMsjCliente(msjServidor);

	sprintf(combustibleAvion, "%d", avion->combustible);
	sprintf(estadoAvion, "%d", avion->estado);
	sprintf(opcionAvion, "%d", *opcion);

	concatenarMsjServidor(msjServidor, avion, combustibleAvion, estadoAvion, opcionAvion);
}

void parsearMensaje(ST_AVION * avion, int * opcion,const char * msjCliente){
	char * linea = malloc(sizeof(char)*LONG_MSJ_CLIE);
	strcpy(linea, msjCliente);

	strcpy(avion->identificador, strtok(linea, "|"));
	strcpy(avion->modelo, strtok(NULL, "|"));
	avion->combustible = atoi(strtok(NULL, "|"));
	avion->estado = atoi(strtok(NULL, "|"));
	*opcion = atoi(strtok(NULL, "\0"));
}
