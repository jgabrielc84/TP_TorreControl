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
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../headers/servidor.h"
#include "../headers/mensaje.h"
#include "../headers/consola.h"
#include "../headers/avion.h"
#include "../headers/archivo.h"

#define TRUE 1
#define FALSE 0


int main() {
	system("clear");

	//VARIABLES
	FILE * ptrArchivoConfigServ = NULL;
	int i = 0;
	int bytesRecibidos = 0;
	int opcion = 0;
	unsigned int tamanioDireccionCliente = 0;
	int puertoServidor = 0;
	char * ipServidor = malloc(sizeof(char)*LONG_IP_SERV);
	char * msjCliente = malloc(sizeof(char)*LONG_MSJ_CLIE);
	ST_AVION * avion = malloc(sizeof(ST_AVION));
	struct sockaddr_in direccionServidor;
	struct sockaddr_in direccionCliente;
	fd_set descriptorLectura;
	int socketServidor = 0;
	int socketCliente[CANT_MAX_CLIE];
	int socketNuevo = 0;
	int posVacia = FALSE;
	int hayActividad = 0;
	PTR_NODOAVION listaAviones;
	PTR_NODOAVION listaAterrizaje;
	PTR_NODOAVION listaDespegue;
	ST_PTRLISTAS ptrListasColas;
	pthread_t hiloGastarCombustible;
	pthread_t hiloManejarEstados;
	pthread_t hiloAdministrarPista;

	//INICIALIZAR VARIABLES
	memset(ipServidor, '\0', LONG_IP_SERV);
	inicializarMsjCliente(msjCliente);
	inicializarAvion(avion);
	listaCrear(&listaAviones);
	listaCrear(&listaAterrizaje);
	listaCrear(&listaDespegue);
	ptrListasColas.ptrListaAviones = &listaAviones;
	ptrListasColas.ptrListaAterrizaje = &listaAterrizaje;
	ptrListasColas.ptrListaDespegue = &listaDespegue;

	for (i = 0; i < CANT_MAX_CLIE; i++){ //Inicializa los socketCliente en 0
		socketCliente[i] = 0;
	}

	ptrArchivoConfigServ = abrirArchivo("config.txt", "r");

	leerIpPuertoDeArchivo(ptrArchivoConfigServ, ipServidor, &puertoServidor);

	fclose(ptrArchivoConfigServ);
	free(ptrArchivoConfigServ);

	direccionServidor = crearServidor(ipServidor, &puertoServidor);

	socketServidor= socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	int activado = TRUE; //Linea para poder probar el socket sin que el sistema te deje en espera por 2 min
	setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	enlazarServidor(&socketServidor, &direccionServidor);

	printf("Esperando conexion!\n\n");
	if (listen(socketServidor, 10) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&hiloGastarCombustible, NULL, (void *) gastarCombustible, &listaAviones)){ //Crea hilo que ejecuta gastarCombustible
		printf("Error al crear el hilo.\n");
		exit(EXIT_FAILURE);
	}

	sleep(1);

	if(pthread_create(&hiloManejarEstados, NULL, (void *) manejarEstados, &ptrListasColas)){ //Crea hilo que ejecuta manejarEstados
		printf("Error al crear el hilo.\n");
		exit(EXIT_FAILURE);
	}

	sleep(1);

	if(pthread_create(&hiloAdministrarPista, NULL, (void *) administrarPista, &ptrListasColas)){ //Crea hilo que ejecuta administrarPista
		printf("Error al crear el hilo.\n");
		exit(EXIT_FAILURE);
	}

	sleep(1);

	while(TRUE){
		FD_ZERO (&descriptorLectura);
		FD_SET (socketServidor, &descriptorLectura);

		for (i = 0; i < CANT_MAX_CLIE; i++){
			FD_SET (socketCliente[i], &descriptorLectura);
		}

		printf("**Espera select**\n");
		hayActividad = select (CANT_MAX_CLIE+1, &descriptorLectura, NULL, NULL, NULL);

		printf("hayActividad = %d\n", hayActividad);
		if (hayActividad < 0){
			printf("Error en select.\n");
		}

		for (i = 0; i<CANT_MAX_CLIE; i++){

			if (FD_ISSET (socketCliente[i], &descriptorLectura)){

				if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
					// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.

					parsearMensaje(avion, &opcion, msjCliente);

					resolverPedidoCliente(&opcion, avion, msjCliente, &socketCliente[i], &listaAviones);
				}else{ // Hay un error en la lectura (el cliente ha cerrado la conexión o hay error al recibir el mensaje)
					if(bytesRecibidos == 0){
						printf ("Socket cerrado.\n");

						socketCliente[i] = 0;
					}else{
						printf ("Error al recibir mensaje.\n");
					}
				}//FIN if
			}//FIN if
		}//FIN for

		printf("*Revisa socketServidor*\n"); // Se trata el socket servidor
		if (FD_ISSET (socketServidor, &descriptorLectura)){ // Un nuevo cliente solicita conexión, se mete el descriptor en socketCliente[]
			socketNuevo = accept(socketServidor, (void *) &direccionCliente, &tamanioDireccionCliente);

			i = 0;
			posVacia = FALSE;
			while(i < CANT_MAX_CLIE && posVacia == FALSE){ //Agrega el socketNuevo a una posicion vacia de socketCliente
				if( socketCliente[i] == 0 ){//comprueba que la posicion este en cero(vacia)
					socketCliente[i] = socketNuevo;
					printf("Agregado a la lista de sockets como socketCliente[%d] = %d\n" , i, socketNuevo);
					posVacia = TRUE;
				}
				i++;
			}//FIN while

			if(i < CANT_MAX_CLIE && posVacia == TRUE){ //Controla que se haya guardado el socketNuevo
				printf("Recibi una conexion en el cliente numero: %d  tamanioDireccion: %d !!\n\n", socketCliente[i], tamanioDireccionCliente);
				if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
					// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.

					parsearMensaje(avion, &opcion, msjCliente);

					resolverPedidoCliente(&opcion, avion, msjCliente, &socketCliente[i], &listaAviones);
				}else{ // Hay un error en la lectura (el cliente ha cerrado la conexión o hay error al recibir el mensaje)
					if(bytesRecibidos == 0){
						printf ("Socket cerrado.\n");

						socketCliente[i] = 0;
					}else{
						printf ("Error al recibir mensaje.\n");
					}
				}//FIN if
			}else{
				printf("Lista de sockets Cliente llena.\n");
			}//FIN if
		}//FIN if
	}//FIN while

//	pthread_exit(NULL);
	free(ipServidor);
	free(msjCliente);
	free(avion);
	return EXIT_SUCCESS;
}
