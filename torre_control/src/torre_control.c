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



int main() {
	system("clear");

	//VARIABLES
	FILE * ptrArchivoConfigServ = NULL;
	FILE * ptrArchivoTorreControl = NULL;
	//int idCliente = 0;
	//int servidor = 0;
	int i = 0;
	//-- // BORRAR
	int bytesRecibidos = 0;
	//fd_set bytesRecibidosFd;
	//-- // BORRAR
	int opcion = 0;
	unsigned int tamanioDireccionCliente = 0;
	int puertoServidor = 0;
	char * ipServidor = malloc(sizeof(char)*LONG_IP_SERV);
	char * msjCliente = malloc(sizeof(char)*LONG_MSJ_CLIE);
	struct sockaddr_in direccionServidor;
	struct sockaddr_in direccionCliente; //Se crea un cliente para poder identificar quien se conecta
	ST_AVION * avion = malloc(sizeof(ST_AVION));
	//-- // BORRAR
	fd_set descriptorLectura;
	int descriptorLecturaNuevo;
	int descriptorLecturaMaximo;
	int socketServidor;
	int socketCliente[CANT_MAX_CLIE];
	int socketNuevo;
	int hayActividadSelect;
	int numeroClientes;
	struct timeval timeout; // ?

	//-- // BORRAR

	//INICIALIZAR VARIABLES
	memset(ipServidor, '\0', LONG_IP_SERV);
	inicializarMsjCliente(msjCliente);
	inicializarAvion(avion);
	//Inicializa los soquets en 0
	for (int i = 0; i < CANT_MAX_CLIE; i++){
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
	if (listen(socketServidor, 100) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//socketCliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccionCliente);

	//printf("Recibi una conexion en el cliente numero %d!!\n\n", socketCliente);

	i = 0; // VER // BORRAR? o reacomodar
/*
	while(1){ //while(inicio) para que el servidor quede corriendo
		FD_ZERO (&descriptorLectura);
		FD_SET (socketServidor, &descriptorLectura);

		//int PEPE=0;
		//FD_SET (socketCliente[PEPE], &descriptorLectura);

		for (i = 0; i < CANT_MAX_CLIE; i++){
			FD_SET (socketCliente[i], &descriptorLectura);
		}
		//printf("WaitWindows 2\n");

		//-- // BORRAR
		FD_ZERO (&descriptorLectura);
		bytesRecibidos = select (CANT_MAX_CLIE+1, &descriptorLectura, NULL, NULL, &timeout);
		printf("WaitWindows 3\n");
		printf("bytesRecibidos: %d\n", bytesRecibidos);
		printf("WaitWindows 4\n");
		for (i = 0; i<CANT_MAX_CLIE; i++)
		{
		    if (FD_ISSET (socketCliente[i], &descriptorLectura))
		    {
		        //if ((Lee_Socket (socketCliente[i], (char *)&buffer, bytesRecibidos) > 0))
		        if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0)//send/recv(*servidorTorreControl, msjServidor, sizeof(char)*LONG_MSJ_SERV, 0);
		    	{
		        	// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.

		        	parsearMensaje(avion, &opcion, msjCliente);

					printf("Id: %s\n", avion->identificador); // BORRAR
					printf("Modelo: %s\n", avion->modelo); // BORRAR
					printf("combAct: %d\n", avion->combustibleActual); // BORRAR
					printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
					printf("estado: %d\n", avion->estado); // BORRAR
					printf("Opcion: %d\n", opcion); // BORRAR

					resolverPedidoClie(&opcion, avion, ptrArchivoTorreControl, msjCliente, &socketCliente[i]);
				}
		        else
		        {
		            // Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[]

		        	printf("Mensaje no recibido \n\tbytesRecibidos: %d \n", bytesRecibidos);
		        }
		    }
		}

		// Se trata el socket servidor
		if (FD_ISSET (socketServidor, &descriptorLectura))
		{
		    // Un nuevo cliente solicita conexión. Aceptarla aquí. En el ejemplo, se acepta la conexión, se mete el descriptor en socketCliente[] y se envía al cliente su posición en el array como número de cliente.
			socketCliente[i] = accept(socketServidor, (void *) &direccionCliente, &tamanioDireccionCliente);
			numeroClientes++;

		}

		//Se setea la cuenta regresiva
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

	}//FIN while(inicio)
*/
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
		hayActividadSelect = select( descriptorLecturaMaximo + 1 , &descriptorLectura , NULL , NULL , &timeout);

		if ((hayActividadSelect < 0) && (errno!=EINTR)){
			printf("select error");
		}

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(socketServidor, &descriptorLectura)){
			if((socketNuevo = accept(socketServidor, (struct sockaddr *) &direccionServidor, &tamanioDireccionCliente))){
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , socketNuevo , inet_ntoa(direccionServidor.sin_addr) , ntohs(direccionServidor.sin_port));

			msjCliente = "Bienvenido";
			//send new connection greeting message
			if( send(socketNuevo, msjCliente, strlen(msjCliente), 0) != strlen(msjCliente) ){
				perror("send");
			}

			puts("Welcome message sent successfully");

			//add new socket to array of sockets
			for (i = 0; i < CANT_MAX_CLIE; i++){
				//if position is empty
				if( socketCliente[i] == 0 ){
					socketCliente[i] = socketNuevo;
					printf("Adding to list of sockets as %d\n" , i);

					break;
				}
			}
		}

		//else its some IO operation on some other socket :)
		for (i = 0; i < CANT_MAX_CLIE; i++){
			descriptorLecturaNuevo = socketCliente[i];

			if (FD_ISSET(descriptorLecturaNuevo, &descriptorLectura)){
				//Check if it was for closing , and also read the incoming message
				if ((bytesRecibidos = read( descriptorLecturaNuevo , msjCliente, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(descriptorLecturaNuevo , (struct sockaddr*)&direccionServidor , (socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n", inet_ntoa(direccionServidor.sin_addr) , ntohs(direccionServidor.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close(descriptorLecturaNuevo);
					socketCliente[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end of the data read
					msjCliente[bytesRecibidos] = '\0';
					send(descriptorLecturaNuevo , msjCliente , strlen(msjCliente) , 0 );
				}
			}
		}
		//Se setea la cuenta regresiva
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
	}

	fclose(ptrArchivoConfigServ);
	fclose(ptrArchivoTorreControl);
	free(ptrArchivoConfigServ);
	free(ptrArchivoTorreControl);
	free(ipServidor);
	free(msjCliente);
	free(avion);
	return EXIT_SUCCESS;
}
