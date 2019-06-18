/* torre_control
 * mensaje.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/mensaje.h"


void liberaBuffer(){
	printf("*liberaBuffer*\n");

	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

void inicializarMsjCliente(char * msjCliente){
	printf("*inicializarMsjCliente*\n");

	memset(msjCliente, '\0', LONG_MSJ_CLIE);
}

void concatenarMsjCliente(char * msjServidor, const ST_AVION * avion,const char * combustibleActAvion, const char * combustibleMaxAvion, const char * estadoAvion, const char * mensaje){
	printf("*concatenarMsjCliente*\n");

	strcpy(msjServidor, avion->identificador);
	strcat(msjServidor, "|");
	strcat(msjServidor, avion->modelo);
	strcat(msjServidor, "|");
	strcat(msjServidor, combustibleActAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, combustibleMaxAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, estadoAvion);
	strcat(msjServidor, "|");
	strcat(msjServidor, mensaje);
}

void formatearMensaje(char * msjCliente, const ST_AVION * avion, const char * mensaje){
	printf("*formatearMensaje*\n");

	char * combustibleActAvion = malloc(sizeof(char)*LONG_COMBUSTIBLEAVION); // Crear variables
	char * combustibleMaxAvion = malloc(sizeof(char)*LONG_COMBUSTIBLEAVION);
	char * estadoAvion = malloc(sizeof(char)*LONG_ESTADOAVION);

	memset(combustibleActAvion, '\0', LONG_COMBUSTIBLEAVION); // Inicializar variables
	memset(combustibleMaxAvion, '\0', LONG_COMBUSTIBLEAVION);
	memset(estadoAvion, '\0', LONG_ESTADOAVION);
	inicializarMsjCliente(msjCliente);

	sprintf(combustibleActAvion, "%d", avion->combustibleActual); // Copiar valores a char
	sprintf(combustibleMaxAvion, "%d", avion->combustibleMaximo);
	sprintf(estadoAvion, "%d", avion->estado);

	concatenarMsjCliente(msjCliente, avion, combustibleActAvion, combustibleMaxAvion, estadoAvion, mensaje);

	free(combustibleActAvion); //Liberar punteros
	free(combustibleMaxAvion);
	free(estadoAvion);
}

void parsearMensaje(ST_AVION * avion, int * opcion,const char * msjCliente){
	printf("*parsearMensaje*\n");

	char * linea = malloc(sizeof(char)*LONG_MSJ_CLIE);

	memset(linea, '\0', LONG_MSJ_CLIE);
	strcpy(linea, msjCliente);

	strcpy(avion->identificador, strtok(linea, "|"));
	strcpy(avion->modelo, strtok(NULL, "|"));
	avion->combustibleActual = atoi(strtok(NULL, "|"));
	avion->combustibleMaximo = atoi(strtok(NULL, "|"));
	avion->estado = atoi(strtok(NULL, "|"));
	*opcion = atoi(strtok(NULL, "\0"));

	free(linea);
}
