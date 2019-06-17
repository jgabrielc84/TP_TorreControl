/* torre_control
 * servidor.c
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */

#include "../headers/servidor.h"


struct sockaddr_in crearServidor(const char * ip, const char * puerto){
	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	direccionServidor.sin_port = htons(atoi(puerto));

	return direccionServidor;
}

void enlazarServidor(int * servidor, struct sockaddr_in * direccionServidor){
	if(bind(*servidor, (void *) direccionServidor, sizeof(*direccionServidor)) != 0){ //asocia el socket creado con la direccion del servidor creado
		perror("Error al enlazar el servidor.\n");
		exit(EXIT_FAILURE);
	}
}

void recibirMensaje(int * bytesRecibidos, int * cliente, char * msjCliente){
	*bytesRecibidos = 0;

	while(*bytesRecibidos == 0){
		*bytesRecibidos = recv(*cliente, msjCliente, sizeof(char)*LONG_MSJ_CLIE, 0);

		if(*bytesRecibidos <= 0){
			printf("Error al recibir mensaje.\n");
		}
	}
}
