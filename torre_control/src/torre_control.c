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
	FILE * ptrArchivoTorreControl = NULL;
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
	//int descriptorLecturaNuevo;
	//int descriptorLecturaMaximo;
	int socketServidor = 0;
	int socketCliente[CANT_MAX_CLIE];
	int socketNuevo = 0;
	int posVacia = FALSE;
	int hayActividad = 0;

	//INICIALIZAR VARIABLES
	memset(ipServidor, '\0', LONG_IP_SERV);
	inicializarMsjCliente(msjCliente);
	inicializarAvion(avion);
	//Inicializa los soquets en 0
	for (i = 0; i < CANT_MAX_CLIE; i++){
		socketCliente[i] = 0;
	}

	ptrArchivoTorreControl = abrirArchivo("torreControl.bin", "rb+");

	ptrArchivoConfigServ = abrirArchivo("config.txt", "r");

	leerIpPuertoDeArchivo(ptrArchivoConfigServ, ipServidor, &puertoServidor);

	direccionServidor = crearServidor(ipServidor, &puertoServidor);

	socketServidor= socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	int activado = 1; //Linea para poder probar el socket sin que el sistema te deje en espera por 2 min
	setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	enlazarServidor(&socketServidor, &direccionServidor);

	printf("Esperando conexion!\n\n");
	if (listen(socketServidor, 10) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//socketCliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccionCliente);

	//printf("Recibi una conexion en el cliente numero %d!!\n\n", socketCliente);


	while(TRUE){
		FD_ZERO (&descriptorLectura);
		FD_SET (socketServidor, &descriptorLectura);

		for (i = 0; i < CANT_MAX_CLIE; i++){
			FD_SET (socketCliente[i], &descriptorLectura);
		}

		//FD_ZERO (&descriptorLectura);

		printf("**Espera select**\n");
		hayActividad = select (CANT_MAX_CLIE+1, &descriptorLectura, NULL, NULL, NULL);

		printf("hayActividad = %d\n", hayActividad);
		printf("WaitWindows 1\n");
		for (i = 0; i<CANT_MAX_CLIE; i++){
			printf("WaitWindows 2.%d\n", i);
		    if (FD_ISSET (socketCliente[i], &descriptorLectura)){
		    	printf("WaitWindows 3.%d\n", i);

		        if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
		        	printf("WaitWindows 4.%d\n", i);

		        	// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.
		        	parsearMensaje(avion, &opcion, msjCliente);

					printf("Id: %s\n", avion->identificador); // BORRAR
					printf("Modelo: %s\n", avion->modelo); // BORRAR
					printf("combAct: %d\n", avion->combustibleActual); // BORRAR
					printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
					printf("estado: %d\n", avion->estado); // BORRAR
					printf("Opcion: %d\n", opcion); // BORRAR

					resolverPedidoClie(&opcion, avion, ptrArchivoTorreControl, msjCliente, &socketCliente[i]);
				}else{
		            // Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[]

		        	printf("Mensaje no recibido \n\tbytesRecibidos: %d \n", bytesRecibidos);
		        }
		    }
		}

		// Se trata el socket servidor
		printf("*Revisa socketServidor*\n");
		if (FD_ISSET (socketServidor, &descriptorLectura)){
		    // Un nuevo cliente solicita conexión. Aceptarla aquí. En el ejemplo, se acepta la conexión, se mete el descriptor en socketCliente[] y se envía al cliente su posición en el array como número de cliente.
			socketNuevo = accept(socketServidor, (void *) &direccionCliente, &tamanioDireccionCliente);
			//numeroClientes++;

			i = 0;
			posVacia = FALSE;
			while(i < CANT_MAX_CLIE && posVacia == FALSE){ //Agrega el socketNuevo a una posicion vacia de socketCliente
			//for (i = 0; i &lt; max_clients; i++){
				//if position is empty
				if( socketCliente[i] == 0 ){
					socketCliente[i] = socketNuevo;
					printf("Agregado a la lista de sockets como socketCliente[%d] = %d\n" , i, socketNuevo);
					posVacia = TRUE;
				}
				i++;
			}

			if(i < CANT_MAX_CLIE && posVacia == TRUE){ //Controla que se haya guardado el socketNuevo
				printf("Recibi una conexion en el cliente numero: %d  tamanioDireccion: %d !!\n\n", socketCliente[i], tamanioDireccionCliente);
				if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
					// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.
					parsearMensaje(avion, &opcion, msjCliente);

					printf("Id: %s\n", avion->identificador); // BORRAR
					printf("Modelo: %s\n", avion->modelo); // BORRAR
					printf("combAct: %d\n", avion->combustibleActual); // BORRAR
					printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
					printf("estado: %d\n", avion->estado); // BORRAR
					printf("Opcion: %d\n", opcion); // BORRAR

					resolverPedidoClie(&opcion, avion, ptrArchivoTorreControl, msjCliente, &socketCliente[i]);
				}else{
					// Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[]

					printf("Mensaje no recibido \n\tbytesRecibidos: %d \n", bytesRecibidos);
				}
			}else{
				printf("Lista de sockets Cliente llena.\n");
			}
		}
	}//FIN while(inicio)
/*
	int addrlen = sizeof(direccionServidor);

	while(1){
		FD_ZERO(&descriptorLectura); //clear the socket set

		FD_SET(socketServidor, &descriptorLectura); //add master socket to set
		descriptorLecturaMaximo = socketServidor; //cantidad maxima de descriptorLectura dentro de fd_set


		for( i = 0 ; i < CANT_MAX_CLIE ; i++){ //add child sockets to set
			descriptorLecturaNuevo = socketCliente[i]; //socket descriptor

			if(descriptorLecturaNuevo > 0){ //if valid socket descriptor then add to read list
				FD_SET(descriptorLecturaNuevo , &descriptorLectura);
			}

			if(descriptorLecturaNuevo > descriptorLecturaMaximo){ //highest file descriptor number, need it for the select function
				descriptorLecturaMaximo = descriptorLecturaNuevo;
			}
		}
		//wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
		hayActividadSelect = select( descriptorLecturaMaximo + 1 , &descriptorLectura , NULL , NULL , NULL);

		if ((hayActividadSelect < 0) && (errno!=EINTR)){
			printf("select error");
		}

		if (FD_ISSET(socketServidor, &descriptorLectura)){ //If something happened on the master socket , then its an incoming connection
			if((socketNuevo = accept(socketServidor, (struct sockaddr *) &direccionServidor, (socklen_t*)&addrlen)) < 0){
				perror("Error accept");
				exit(EXIT_FAILURE);
			}
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , socketNuevo , inet_ntoa(direccionServidor.sin_addr) , ntohs(direccionServidor.sin_port));

			msjCliente = "Bienvenido\n";

			if( send(socketNuevo, msjCliente, strlen(msjCliente), 0) != strlen(msjCliente) ){ //send new connection greeting message
				perror("send");
			}

			puts("Welcome message sent successfully");
			printf("WaitWindows 1\n");

			//add new socket to array of sockets
			for (i = 0; i < CANT_MAX_CLIE; i++){
				//if position is empty
				if( socketCliente[i] == 0 ){
					socketCliente[i] = socketNuevo;
					printf("Agregado a la lista de sockets como socketCliente[%d] = %d\n" , i, socketNuevo);

					break;
				}
			}
		}
		printf("WaitWindows 2\n");

		//else its some IO operation on some other socket :)
		for (i = 0; i < CANT_MAX_CLIE; i++){
			descriptorLecturaNuevo = socketCliente[i];
			printf("WaitWindows 3.%d\n", i);

			if (FD_ISSET(descriptorLecturaNuevo, &descriptorLectura)){
				printf("WaitWindows 4.%d\n", i);

				//Check if it was for closing , and also read the incoming message
				if ((bytesRecibidos = recv(descriptorLecturaNuevo, msjCliente, sizeof(char)*LONG_MSJ_CLIE, 0)) == 0){ //read( descriptorLecturaNuevo , msjCliente, 1024)) == 0){
					printf("WaitWindows 5.%d\n", i);
					//Somebody disconnected , get his details and print
					printf("Cliente: %d salio de su sesion.\n", descriptorLecturaNuevo);

					//getpeername(descriptorLecturaNuevo , (struct sockaddr*)&direccionServidor , (socklen_t*)&addrlen);
					//printf("Host disconnected , ip %s , port %d \n", inet_ntoa(direccionServidor.sin_addr) , ntohs(direccionServidor.sin_port));

					//Close the socket and mark as 0 in list for reuse
					//close(descriptorLecturaNuevo);
					//socketCliente[i] = 0;
				}else{ //Echo back the message that came in
					//set the string terminating NULL byte on the end of the data read
					printf("Mensaje Recibido de cliente %d: %s",descriptorLectura, msjCliente);

					msjCliente[bytesRecibidos] = '\0';
					//msjCliente = "Mensaje recibido.";
					send(descriptorLecturaNuevo , msjCliente , strlen(msjCliente) , 0 );
				}
			}
		}
		//Se setea la cuenta regresiva
		//timeout.tv_sec = 3;
		//timeout.tv_usec = 0;
	}
*/

	fclose(ptrArchivoConfigServ);
	fclose(ptrArchivoTorreControl);
	free(ptrArchivoConfigServ);
	free(ptrArchivoTorreControl);
	free(ipServidor);
	free(msjCliente);
	free(avion);
	return EXIT_SUCCESS;
}
