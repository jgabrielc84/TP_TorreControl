/* torre_control
 * archivo.c
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */


#include "../headers/archivo.h"

#define LONG_LINEALEIDA 50


void abrirArchivoTorreControl(FILE * ptrArchivoTorreControl, ST_AVION * avion){
	printf("*abrirArchivoTorreControl*\n");

	if((ptrArchivoTorreControl = fopen("torreControl.txt", "r")) == NULL){
		printf("Error al abrir archivo torreControl.txt\n");
		if((ptrArchivoTorreControl = fopen("torreControl.txt", "w")) == NULL){
			printf("Error al crear archivo torreControl.txt\n");
			exit(EXIT_FAILURE);
		}else{
			printf("Archivo torreControl.txt creado correctamente.\n");
			fwrite(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
		}
	}else{
		printf("Archivo de torreControl.txt abierto correctamente.\n");
	}
}

void abrirArchivoConfigServ(FILE * ptrArchivo){
	printf("*abrirArchivoConfigServ*\n");
	if((ptrArchivo = fopen("config.txt", "r")) == NULL){
		printf("Error al abrir archivo config.txt\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Archivo de configuracion abierto correctamente.\n\n");
	}
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

int comprobarExisteAvion(FILE * ptrArchivoTorreControl, ST_AVION * avion){
	long int posicionPtr = 0;
	int encontrado = 0;
	ST_AVION * avionArchivo = malloc(sizeof(ST_AVION));

	posicionPtr = ftell(ptrArchivoTorreControl);
	fseek(ptrArchivoTorreControl, 0, SEEK_SET);
	fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);

	while((!feof(ptrArchivoTorreControl)) && (strcmp(avionArchivo->identificador, avion->identificador) != 0)){
		fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
	}

	if(strcmp(avionArchivo->identificador, avion->identificador) == 0){
		encontrado = 1; //true
	}else{
		encontrado = 0; //false
	}

	fseek(ptrArchivoTorreControl, posicionPtr, SEEK_SET);

	return encontrado;
}
