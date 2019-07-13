/* torre_control
 * consola.c
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#include "../headers/consola.h"


void resolverPedidoCliente(int * opcion, ST_AVION * avion, char * msjCliente, const int * idCliente, PTR_NODOAVION * listaAviones){
	printf("*resolverPedidoCliente*\n");

	switch(*opcion){
	case REGISTRAR_AVION:
		registrarAvion(avion, listaAviones, msjCliente);
		enviarMensajeACliente(idCliente, msjCliente);
		break;
	case INGRESAR_A_PISTA:
		ingresarAPista(avion, listaAviones, msjCliente);
		enviarMensajeACliente(idCliente, msjCliente);
		break;
	case ESTADO_AVION:
		consultarEstadoAvion(avion, listaAviones, msjCliente);
		enviarMensajeACliente(idCliente, msjCliente);
		break;
	}
}
