/*
 * servidor.c
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */

#include "../headers/servidor.h"


struct sockaddr_in crearServidor(const char * ip, const char * puerto){
	struct sockaddr_in servidor;

	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr(ip);
	servidor.sin_port = htons(atoi(puerto));

	return servidor;
}

