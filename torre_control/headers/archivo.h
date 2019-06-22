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
#include "../headers/mensaje.h"


FILE * abrirArchivo(const char *, const char *);

void leerIpPuertoDeArchivo(FILE *, char *, int *);

int comprobarExisteAvion(FILE *, ST_AVION *);

int buscarPosVacia(FILE *);

int buscarPosAvion(FILE *, ST_AVION *);

void leerPosArchivo(FILE *, int *, ST_AVION *);


#endif /* HEADERS_ARCHIVO_H_ */
