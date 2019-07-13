/* torre_control
 * consola.h
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#ifndef HEADERS_CONSOLA_H_
#define HEADERS_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/avion.h"
#include "../headers/mensaje.h"
#include "../headers/servidor.h"

enum MENUOPCION{
	REGISTRAR_AVION=1,
	INGRESAR_A_PISTA=2,
	ESTADO_AVION=3
};


void resolverPedidoCliente(int *, ST_AVION *, char *, const int *, PTR_NODOAVION *);


#endif /* HEADERS_CONSOLA_H_ */
