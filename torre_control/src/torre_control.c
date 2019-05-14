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
#include <arpa/inet.h>
#include <sys/socket.h>

#define LONG_IDENTIFICADOR 8
#define LONG_MODELO 20

typedef struct{
	char identificador[LONG_IDENTIFICADOR+1];
	char modelo[LONG_MODELO+1];
	int combustible;
	char estado;
}ST_AVION;


int main(void) {
	//Se crea un servidor con los siguientes datos:
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(8080);

	int servidor = socket(AF_INET, SOCK_STREAM, 0); //se pide un socket que devuelve un valor

	//Linea para poder probar el socket sin que el sistema te deje en espera por 2 min
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	//Bind: asocia el socket creado con la direccion del servidor creado al principio
	if(bind(servidor, (void *) &direccionServidor, sizeof(direccionServidor)) != 0){ //Valida el bind para saber si el sistema le da el puerto seleccionado o no
		perror("Fallo el Bind");
		return 1;
	}

	printf("Estoy escuchando!\n");
	listen(servidor, 100); //Se queda escuchando por si se conectan al servidor

	//Se crea un cliente para poder identificar quien se conecta
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion;

	int cliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccion);

	printf("Recibi una conexion en %d!!\n", cliente);

	//Envia contestacion al cliente
	send(cliente, "Mensaje Recibido", 17, 0);

	for(;;);

	return EXIT_SUCCESS;
}
