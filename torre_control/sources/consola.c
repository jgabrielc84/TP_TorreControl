/* torre_control
 * consola.c
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#include "../headers/consola.h"


void resolverOpcionMsj(int * opcion){
	switch(*opcion){
	case MENU_REGISTRAR_AVION:
		//registrarAvion();
		break;
	case MENU_PEDIR_PISTA:

		break;
	case MENU_ESTADO_AVION:
		//enviaEstadoAvion();

		break;
	default:
		printf("La opcion recibida es incorrecta");
		//send con aviso de mensaje mal recibido
		break;
	}
}
