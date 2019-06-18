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
	/*default:
		printf("La opcion recibida es incorrecta");
		//send con aviso de mensaje mal recibido
		break;*/
	}
	//------------
	/*fseek(ptrArchivoTorreControl, 0, SEEK_SET); // BORRAR
	fread(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl); // BORRAR
	printf("Id: %s\n", avion->identificador); // BORRAR
	printf("Modelo: %s\n", avion->modelo); // BORRAR
	printf("combAct: %d\n", avion->combustibleActual); // BORRAR
	printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
	printf("estado: %d\n", avion->estado); // BORRAR
	printf("Opcion: %d\n", *opcion); // BORRAR*/

	//------------
}
