/* torre_control
 * consola.c
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#include "../headers/consola.h"


void resolverPedidoClie(int * opcion,ST_AVION * avion, FILE * ptrArchivoTorreControl, char * msjCliente, const int * idCliente){
	switch(*opcion){
	case REGISTRAR_AVION:
		registrarAvion(avion, ptrArchivoTorreControl, msjCliente);
		enviarMensajeACliente(idCliente, msjCliente);
		break;
	case INGRESAR_A_PISTA:

		break;
	case ESTADO_AVION:
		consultarEstadoAvion(avion, ptrArchivoTorreControl, msjCliente);
		enviarMensajeACliente(idCliente, msjCliente);
		break;
	}
}
