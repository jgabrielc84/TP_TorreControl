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

#define LONG_IDENTIFICADOR 8
#define LONG_MODELO 20
#define LONG_MENSAJE 50
#define LONG_MSG_SERV 100+1

enum AVIONESTADO{
	AVION_HANGAR,
	AVION_LISTO_HANGAR,
	AVION_LISTO_DESPEGAR,
	AVION_DESPEGANDO,
	AVION_EN_VUELO,
	AVION_ESPERA_ATERRIZAR,
	AVION_ATERRIZANDO,
};

typedef struct{
	char identificador[LONG_IDENTIFICADOR+1];
	char modelo[LONG_MODELO+1];
	int combustible;
	char estado;
}ST_AVION;


int main() {
	system("clear");

	char mensajeRecibido[LONG_MSG_SERV];
	ST_MENSAJE * mensajeCliente;
	ST_AVION * avion;
	mensajeCliente = (ST_MENSAJE *)(malloc(sizeof(ST_MENSAJE)));
	avion = (ST_AVION *)(malloc(sizeof(ST_AVION)));
	memset(mensajeRecibido, '\0', (LONG_MSG_SERV));
	int cliente = 0;
	int servidor = 0;

	//Se crea un servidor con los siguientes datos:
	struct sockaddr_in direccionServidor = crearServidor("127.0.0.1", "8080");

	servidor = socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	//Linea para poder probar el socket sin que el sistema te deje en espera por 2 min(para la ENTREGA se saca)
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	//Bind: asocia el socket creado con la direccion del servidor creado al principio
	if(bind(servidor, (void *) &direccionServidor, sizeof(direccionServidor)) != 0){ //Valida el bind para saber si el sistema le da el puerto seleccionado o no
		perror("Fallo el Bind");
		return 1;
	}


	printf("Estoy escuchando!\n\n");
	listen(servidor, 100); //Se queda escuchando por si se conectan al servidor

	//-----------------------

	//while (inicio) para que el servidor quede corriendo
	while(1){
		struct sockaddr_in direccionCliente; //Se crea un cliente para poder identificar quien se conecta
		unsigned int tamanioDireccion;

		cliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccion);

		printf("Recibi una conexion en el cliente numero %d!!\n\n", cliente);

		//Envia contestacion al cliente
		send(cliente, "Se ha conectado a la Torre de Control\n", 40, 0);

		//---------------------------------------
		//Recepcion y parseo del mensaje entrante
		//---------------------------------------

		//int bytesRecibidos = recv(cliente, mensajeCliente, sizeof(ST_MENSAJE), MSG_WAITALL);
		int bytesRecibidos = recv(cliente, mensajeRecibido, sizeof(char)*(LONG_MSG_SERV), MSG_WAITALL);

		if(bytesRecibidos >= 0){
			strcpy(mensajeCliente->origen, strtok(mensajeRecibido, ";"));
			strcpy(mensajeCliente->destino, strtok(NULL, ";"));
			strcpy(mensajeCliente->mensaje, strtok(NULL, "\0"));
			//strcpy(opcion, strtok(NULL, "\0"));

			//switch(opcion){
			printf("Origen = %s\n", mensajeCliente->origen);
			printf("Destino = %s\n", mensajeCliente->destino);
			printf("mensaje = %s\n\n", mensajeCliente->mensaje);

			//ahora hay que parsear el mensaje para guardarlo en ST_AVION
			strcpy(avion->identificador, strtok(mensajeCliente->mensaje, ","));
			strcpy(avion->modelo, strtok(NULL, ","));
			avion->combustible = atoi(strtok(NULL, "\0"));

			printf("Identificador = %s\n", avion->identificador);
			printf("Modelo = %s\n", avion->modelo);
			printf("Combustible = %d\n\n", avion->combustible);

			//Envia contestacion al cliente
			char msjACliente[100];
			memset(msjACliente, '\0', 100);
			strcpy(msjACliente, "Avion: ");
			strcat(msjACliente, avion->modelo);
			strcat(msjACliente, "\tIdentificador: ");
			strcat(msjACliente, avion->identificador);
			strcat(msjACliente, "\nSe recibieron los datos satisfactoriamente\n");

			printf("Se responde al cliente: \n%s\n", msjACliente);
			send(cliente, msjACliente, 100, MSG_WAITALL);

		}else{
			perror("el cliente se ha desconectado del servidor");
			return 1;
		}
	}//FIN while (inicio)

	free(mensajeCliente);
	free(avion);
	return EXIT_SUCCESS;
}
