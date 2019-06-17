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
	MENU_SALIR_SISTEMA,
	MENU_REGISTRAR_AVION,
	MENU_PEDIR_PISTA,
	MENU_ESTADO_AVION
};


void resolverOpcionMsj(int *);


#endif /* HEADERS_CONSOLA_H_ */
