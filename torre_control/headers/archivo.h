/* torre_control
 * archivo.h
 *
 *  Created on: 2 jun. 2019
 *      Author: gabriel
 */

#ifndef HEADERS_ARCHIVO_H_
#define HEADERS_ARCHIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../headers/mensaje.h"


void abrirArchivoTorreControl(FILE * ptrArchivoTorreControl, ST_AVION * avion);

void abrirArchivoConfigServ(FILE *);

void leerIpPuertoDeArchivo(FILE *, char *, int *);

int comprobarExisteAvion(FILE *, ST_AVION *);


#endif /* HEADERS_ARCHIVO_H_ */
