/* torre_control
 * avion.c
 *
 *  Created on: 15 may. 2019
 *      Author: gabriel
 */

#include "../headers/avion.h"
#include "../headers/mensaje.h"
#include "../headers/archivo.h"


void inicializarAvion(ST_AVION * avion){
	memset(avion->modelo, '\0', LONG_MODELO);
	memset(avion->identificador, '\0', LONG_IDENTIFICADOR);
	avion->estado = AVION_HANGAR;
	avion->combustibleActual = 0;
	avion->combustibleMaximo = 0;
}

void registrarAvion(ST_AVION * avion, FILE * ptrArchivoTorreControl, char * msjCliente){
	printf("*registrarAvion*\n");

	int existe = comprobarExisteAvion(ptrArchivoTorreControl, avion);

	if(!existe){ //no encontrado
		fwrite(avion, sizeof(ST_AVION), 1, ptrArchivoTorreControl);
		formatearMensaje(msjCliente, avion, "Avion correctamente ingresado a torre de control");
	}else{ //encontrado
		formatearMensaje(msjCliente, avion, "El avion ya se encuentra en torre de control");
	}
}
