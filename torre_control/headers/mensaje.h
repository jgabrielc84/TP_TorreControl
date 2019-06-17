/* torre_control
 * mensaje.h
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#ifndef HEADERS_MENSAJE_H_
#define HEADERS_MENSAJE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/avion.h"

#define LONG_MSJ_CLIE 100+1
#define LONG_COMBUSTIBLEAVION 10+1
#define LONG_ESTADOAVION 1+1
#define LONG_OPCIONAVION 1+1

void liberaBuffer();

void inicializarMsjCliente(char *);

void formatearMensaje(char *, const ST_AVION *, const char *);

void parsearMensaje(ST_AVION *, int *,const char *);

#endif /* HEADERS_MENSAJE_H_ */
