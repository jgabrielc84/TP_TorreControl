/* torre_control
 * servidor.h
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */

#ifndef HEADERS_SERVIDOR_H_
#define HEADERS_SERVIDOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //sleep
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../headers/mensaje.h"

#define LONG_IP_SERV 15+1
#define CANT_MAX_CLIE 10


struct sockaddr_in crearServidor(const char *, const int *);

void enlazarServidor(int *, struct sockaddr_in *);

void enviarMensajeACliente(const int *, const char *);

int recibirMensaje(int *, char *);


#endif /* HEADERS_SERVIDOR_H_ */
