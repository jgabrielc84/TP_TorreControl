/* torre_control
 * servidor.c
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */

#include "../headers/servidor.h"


struct sockaddr_in crearServidor(const char * ip, const int * puerto){
	printf("*crearServidor*\n");
	struct sockaddr_in servidor;

	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr(ip);
	servidor.sin_port = htons(*puerto);

	return servidor;
}

void enlazarServidor(int * servidor, struct sockaddr_in * direccionServidor){
	printf("*enlazarServidor*\n");
	if(bind(*servidor, (void *) direccionServidor, sizeof(*direccionServidor)) != 0){ //asocia el socket creado con la direccion del servidor creado
		perror("Error al enlazar el servidor.\n");
		exit(EXIT_FAILURE);
	}
}

void recibirMensaje(int * bytesRecibidos, int * cliente, char * msjCliente){
	printf("*recibirMensaje*\n");
	*bytesRecibidos = 0;

	while(*bytesRecibidos == 0){
		*bytesRecibidos = recv(*cliente, msjCliente, sizeof(char)*LONG_MSJ_CLIE, 0);

		if(*bytesRecibidos <= 0){
			printf("Error al recibir mensaje.\n");
		}
	}
}
