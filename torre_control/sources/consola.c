/* torre_control
 * consola.c
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#include "../headers/consola.h"


void resolverOpcionMsj(int * opcion,ST_AVION * avion, FILE * ptrArchivoTorreControl, char * msjCliente){
	switch(*opcion){
	case REGISTRAR_AVION:
		registrarAvion(avion, ptrArchivoTorreControl, msjCliente);
		break;
	case INGRESAR_A_PISTA:

		break;
	case ESTADO_AVION:
		//enviaEstadoAvion();

		break;
	default:
		printf("La opcion recibida es incorrecta");
		//send con aviso de mensaje mal recibido
		break;
	}
}
