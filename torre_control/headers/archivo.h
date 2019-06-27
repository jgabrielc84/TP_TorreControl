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

#define TRUE 1
#define FALSE 0


FILE * abrirArchivo(const char *, const char *);

void leerIpPuertoDeArchivo(FILE *, char *, int *);


#endif /* HEADERS_ARCHIVO_H_ */
