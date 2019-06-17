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
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../headers/mensaje.h"


struct sockaddr_in crearServidor(const char *, const char *);

void enlazarServidor(int *, struct sockaddr_in *);

void recibirMensaje(int *, int *, char *);


#endif /* HEADERS_SERVIDOR_H_ */
