/* torre_control
 * mensaje.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/mensaje.h"

/**
 *
 */
void liberarBuffer(){
	printf("*liberaBuffer*\n");

	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

/**
 * Limpia el parametro recibido por referencia
 * @param msjCliente
 */
void inicializarMsjCliente(char * msjCliente){
	printf("*inicializarMsjCliente*\n");

	memset(msjCliente, '\0', LONG_MSJ_CLIE);
}

/**
 * Recibe la estructura estructura del avion, datos del combustible, estado, y mensaje,
 * Concatena un mensaje con toda esa informacion en un solo string pasado por referencia.
 * @param msjServidor, avion, combustibleActAvion, combustibleMaxAvion, estadoAvion, mensaje
 */
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

/**
 * Crea punteros de variables de ST_AVION con sus correspondientes reservas de memoria,
 * Rellena esas variables con los datos de la ST_AVION recibida. Llama a concatenarMsjCliente y libera punteros.
 * @param msjServidor, avion, mensaje
 */
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

/**
 * Recibe el mensaje del Cliente, el cual separa y toma cada dato para ser guardado en
 * la variable correspondiente dentro de la estructura del avion recibida por referencia
 * @param avion, opcion, msjCliente
 */
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
