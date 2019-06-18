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

void concatenarMsjCliente(char * msjServidor, const ST_AVION * avion,const char * combustibleAvion, const char * estadoAvion, const char * mensaje){
	strcpy(msjServidor, avion->identificador);
	strcat(msjServidor, "|");
	strcat(msjServidor, avion->modelo);
	strcat(msjServidor, "|");
	strcat(msjServidor, combustibleAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, estadoAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, mensaje);
}

void formatearMensaje(char * msjCliente, const ST_AVION * avion, const char * mensaje){
	char * combustibleAvion = malloc(sizeof(char)*LONG_COMBUSTIBLEAVION);
	char * estadoAvion = malloc(sizeof(char)*LONG_ESTADOAVION);

	memset(combustibleAvion, '\0', LONG_COMBUSTIBLEAVION);
	memset(estadoAvion, '\0', LONG_ESTADOAVION);
	inicializarMsjCliente(msjCliente);

	sprintf(combustibleAvion, "%d", avion->combustibleActual);
	sprintf(estadoAvion, "%d", avion->estado);

	concatenarMsjCliente(msjCliente, avion, combustibleAvion, estadoAvion, mensaje);
}

void parsearMensaje(ST_AVION * avion, int * opcion,const char * msjCliente){
	printf("*parsearMensaje*\n");
	char * linea = malloc(sizeof(char)*LONG_MSJ_CLIE);
	strcpy(linea, msjCliente);

	strcpy(avion->identificador, strtok(linea, "|"));
	strcpy(avion->modelo, strtok(NULL, "|"));
	avion->combustibleActual = atoi(strtok(NULL, "|"));
	avion->combustibleMaximo = atoi(strtok(NULL, "|"));
	avion->estado = atoi(strtok(NULL, "|"));
	*opcion = atoi(strtok(NULL, "\0"));
}
