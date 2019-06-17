/* torre_control
 * avion.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/avion.h"
#include "../headers/mensaje.h"


void inicializarAvion(ST_AVION * avion){
	memset(avion->modelo, '\0', LONG_MODELO);
	memset(avion->identificador, '\0', LONG_IDENTIFICADOR);
	avion->estado = AVION_HANGAR;
	avion->combustible = 0;
}
