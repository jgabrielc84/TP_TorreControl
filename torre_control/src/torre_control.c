/*
 ============================================================================
 Name        : torre_control.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Torre de control in C, tp_aeropuerto
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../headers/servidor.h"
#include "../headers/mensaje.h"
#include "../headers/consola.h"
#include "../headers/avion.h"
#include "../headers/archivo.h"


int main() {
	system("clear");

	//VARIABLES
	FILE * ptrArchivoConfigServ = NULL;
	FILE * ptrArchivoTorreControl = NULL;
	int cliente = 0;
	int servidor = 0;
	int bytesRecibidos = 0;
	int opcion = 0;
	unsigned int tamanioDireccionCliente = 0;
	int puertoServidor = 0;
	char * ipServidor = malloc(sizeof(char)*LONG_IP_SERV);
	char * msjCliente = malloc(sizeof(char)*LONG_MSJ_CLIE);
	struct sockaddr_in direccionServidor;
	struct sockaddr_in direccionCliente; //Se crea un cliente para poder identificar quien se conecta
	ST_AVION * avion = malloc(sizeof(ST_AVION));

	//INICIALIZAR VARIABLES
	memset(ipServidor, '\0', LONG_IP_SERV);
	inicializarMsjCliente(msjCliente);
	inicializarAvion(avion);

	abrirArchivoTorreControl(ptrArchivoTorreControl, avion);

	//abrirArchivoConfigServ(ptrArchivoConfigServ);
	//-------------//El archivo se habre en el main porque falla la funcion(no encuentro el error)
	printf("*abrirArchivoConfigServ*\n");
	if((ptrArchivoConfigServ = fopen("config.txt", "r")) == NULL){
		printf("Error al abrir archivo\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Archivo de configuracion abierto correctamente.\n");
	}
	//-------------

	leerIpPuertoDeArchivo(ptrArchivoConfigServ, ipServidor, &puertoServidor);

	direccionServidor = crearServidor(ipServidor, &puertoServidor);

	servidor = socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	int activado = 1; //Linea para poder probar el socket sin que el sistema te deje en espera por 2 min(para la ENTREGA se saca?)
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	enlazarServidor(&servidor, &direccionServidor);

	printf("Esperando conexion!\n\n");
	listen(servidor, 100); //Se queda escuchando por si se conectan al servidor

	cliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccionCliente);

	printf("Recibi una conexion en el cliente numero %d!!\n\n", cliente);

	while(1){ //while(inicio) para que el servidor quede corriendo
		recibirMensaje(&bytesRecibidos, &cliente, msjCliente);
		parsearMensaje(avion, &opcion, msjCliente);

		//MensajeCliente: AABCN123|Boeing 767|180000|0|1

		printf("MensajeCliente: %s\n\n", msjCliente); // BORRAR
		printf("wait windows 6\n\n"); // BORRAR

		resolverOpcionMsj(&opcion, avion, ptrArchivoTorreControl, msjCliente);

	}//FIN while(inicio)

	free(ptrArchivoConfigServ);
	free(ptrArchivoTorreControl);
	free(ipServidor);
	free(msjCliente);
	free(avion);
	return EXIT_SUCCESS;
}
