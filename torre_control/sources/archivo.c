/* torre_control
 * archivo.c
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */


#include "../headers/archivo.h"

#define LONG_LINEALEIDA 50


FILE * abrirArchivo(const char * nombreArchivo, const char * modoApertura){
	printf("*abrirArchivo %s*\n", nombreArchivo);

	FILE * ptrArchivo = NULL;

	if((ptrArchivo = fopen(nombreArchivo, modoApertura)) == NULL){
		printf("Error al abrir archivo %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}else{
		printf("Archivo %s abierto correctamente.\n\n", nombreArchivo);
	}

	return ptrArchivo;
}

void leerIpPuertoDeArchivo(FILE * ptrArchivo, char * ip, int * puerto){
	printf("*leerIpPuerto*\n");

	char * lineaLeida = malloc((sizeof(char))*LONG_LINEALEIDA);
	memset(lineaLeida, '\0', LONG_LINEALEIDA); //Se inicializa la variable

	fgets(lineaLeida, LONG_LINEALEIDA, ptrArchivo);

	strtok(lineaLeida, "=");
	strcpy(ip, strtok(NULL, " "));
	*puerto = atoi(strtok(NULL, ";"));

	free(lineaLeida);
}
