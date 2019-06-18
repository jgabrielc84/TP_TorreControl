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

	if((ptrArchivoTorreControl = fopen("torreControl.bin", "rb+")) == NULL){
		printf("Error al abrir archivo torreControl.bin\n");
		if((ptrArchivoTorreControl = fopen("torreControl.bin", "wb+")) == NULL){
			printf("Error al crear archivo torreControl.bin\n");
			exit(EXIT_FAILURE);
		}else{
			printf("Archivo torreControl.bin creado correctamente.\n");
			fwrite(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
			fclose(ptrArchivoTorreControl);
			printf("Vuelva a ejecutar torre de control.\n");
			exit(EXIT_SUCCESS);
		}
	}else{
		printf("Archivo de torreControl.bin abierto correctamente.\n");
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
	printf("*comprobarExisteAvion*\n");

	long int posicionInicialPtr = 0;
	int encontrado = 0;
	ST_AVION * avionArchivo = malloc(sizeof(ST_AVION));
	inicializarAvion(avionArchivo);

	posicionInicialPtr = ftell(ptrArchivoTorreControl);
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

	fseek(ptrArchivoTorreControl, posicionInicialPtr, SEEK_SET); // vuelve a poner el puntero en donde lo encontro

	return encontrado;
}

int buscarPosVacia(FILE * ptrArchivoTorreControl){
	printf("*buscarPosVacia*\n");

	long int bytesLeidos = 0;
	long int posicionPtr = 0;
	ST_AVION * avionArchivo = malloc(sizeof(ST_AVION));
	inicializarAvion(avionArchivo);

	fseek(ptrArchivoTorreControl, 0, SEEK_SET);
	fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);

	while((!feof(ptrArchivoTorreControl)) && (avionArchivo->combustibleMaximo != 0)){
		fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
	}

	if(avionArchivo->combustibleMaximo == 0){
		bytesLeidos = ftell(ptrArchivoTorreControl);
		posicionPtr = ((bytesLeidos/(sizeof(ST_AVION))) - 1);
	}else{
		posicionPtr = -1;
	}

	return posicionPtr;
}

int buscarPosAvion(FILE * ptrArchivoTorreControl, ST_AVION * avion){
	printf("*buscarPosAvion*\n");

	long int posicionInicialPtr = 0;
	long int bytesLeidos = 0;
	int posicionPtr = 0;
	ST_AVION * avionArchivo = malloc(sizeof(ST_AVION));
	inicializarAvion(avionArchivo);

	posicionInicialPtr = ftell(ptrArchivoTorreControl);
	fseek(ptrArchivoTorreControl, 0, SEEK_SET);
	fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);

	while((!feof(ptrArchivoTorreControl)) && (strcmp(avionArchivo->identificador, avion->identificador) != 0)){
		fread(avionArchivo, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
	}

	if(strcmp(avionArchivo->identificador, avion->identificador) == 0){
		bytesLeidos = ftell(ptrArchivoTorreControl);
		posicionPtr = ((bytesLeidos/(sizeof(ST_AVION))) - 1);
	}else{
		posicionPtr = -1;
	}

	fseek(ptrArchivoTorreControl, posicionInicialPtr, SEEK_SET); // vuelve a poner el puntero en donde lo encontro

	return posicionPtr;
}

void leerPosArchivo(FILE * ptrArchivoTorreControl, int * posicionAvionArchivo, ST_AVION * avion){
	printf("*leerPosArchivo*\n");

	fseek(ptrArchivoTorreControl, (sizeof(ST_AVION) * (*posicionAvionArchivo)), SEEK_SET);
	fread(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
}
