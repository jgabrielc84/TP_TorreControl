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


int main() {
	system("clear");

	//VARIABLES
	int cliente = 0;
	int servidor = 0;
	int bytesRecibidos = 0;
	int opcion = -1;
	unsigned int tamanioDireccionCliente = 0;
	char * msjCliente = malloc(sizeof(char)*LONG_MSJ_CLIE);
	struct sockaddr_in direccionServidor;
	struct sockaddr_in direccionCliente; //Se crea un cliente para poder identificar quien se conecta
	ST_AVION * avion = malloc(sizeof(ST_AVION));

	//INICIALIZAR VARIABLES
	inicializarMsjCliente(msjCliente);
	memset(msjCliente, '\0', (LONG_MSJ_CLIE));

	direccionServidor = crearServidor("127.0.0.1", "8080");

	servidor = socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	int activado = 1; //Linea para poder probar el socket sin que el sistema te deje en espera por 2 min(para la ENTREGA se saca?)
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	enlazarServidor(&servidor, &direccionServidor);

	printf("Esperando conexion!\n\n");
	listen(servidor, 100); //Se queda escuchando por si se conectan al servidor

	cliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccionCliente);

	printf("Recibi una conexion en el cliente numero %d!!\n\n", cliente);

	//send(cliente, "Se ha conectado a la Torre de Control\n", 40, 0);

	while(1){ //while (inicio) para que el servidor quede corriendo
		recibirMensaje(&bytesRecibidos, &cliente, msjCliente);
		parsearMensaje(avion, &opcion, msjCliente);

		printf("MensajeCliente: %s\n\n", msjCliente); // BORRAR
		printf("wait windows 6\n\n"); // BORRAR

		resolverOpcionMsj(&opcion);
	}//FIN while (inicio)

	free(msjCliente);
	free(avion);
	return EXIT_SUCCESS;
}
