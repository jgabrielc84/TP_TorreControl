/* torre_control
 * consola.c
 *
 *  Created on: 9 jun. 2019
 *      Author: gabriel
 */

#include "../headers/consola.h"


void resolverPedidoCliente(int * opcion, ST_AVION * avion, char * msjCliente, const int * idCliente,
						PTR_NODOAVION * listaAviones){
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

/*void resolverSelect(){

	FD_ZERO (&descriptorLectura);
	FD_SET (socketServidor, &descriptorLectura);

	for (i = 0; i < CANT_MAX_CLIE; i++){
		FD_SET (socketCliente[i], &descriptorLectura);
	}

	printf("**Espera select**\n");
	hayActividad = select (CANT_MAX_CLIE+1, &descriptorLectura, NULL, NULL, NULL);

	printf("hayActividad = %d\n", hayActividad);
	printf("WaitWindows 1\n");
	for (i = 0; i<CANT_MAX_CLIE; i++){
		printf("WaitWindows 2.%d\n", i);
		if (FD_ISSET (socketCliente[i], &descriptorLectura)){
			printf("WaitWindows 3.%d\n", i);

			if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
				printf("WaitWindows 4.%d\n", i);

				// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.
				parsearMensaje(avion, &opcion, msjCliente);

				printf("Id: %s\n", avion->identificador); // BORRAR
				printf("Modelo: %s\n", avion->modelo); // BORRAR
				printf("combAct: %d\n", avion->combustibleActual); // BORRAR
				printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
				printf("estado: %d\n", avion->estado); // BORRAR
				printf("Opcion: %d\n", opcion); // BORRAR

				resolverPedidoCliente(&opcion, avion, msjCliente, &socketCliente[i], &listaAviones, &listaAterrizaje, &colaDespegue);
			}else{
				// Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[]

				printf("Mensaje no recibido \n\tbytesRecibidos: %d \n", bytesRecibidos);
			}
		}
	}

	// Se trata el socket servidor
	printf("*Revisa socketServidor*\n");
	if (FD_ISSET (socketServidor, &descriptorLectura)){
		// Un nuevo cliente solicita conexión. Aceptarla aquí. En el ejemplo, se acepta la conexión, se mete el descriptor en socketCliente[] y se envía al cliente su posición en el array como número de cliente.
		socketNuevo = accept(socketServidor, (void *) &direccionCliente, &tamanioDireccionCliente);
		//numeroClientes++;

		i = 0;
		posVacia = FALSE;
		while(i < CANT_MAX_CLIE && posVacia == FALSE){ //Agrega el socketNuevo a una posicion vacia de socketCliente
		//for (i = 0; i < max_clients; i++){
			//if position is empty
			if( socketCliente[i] == 0 ){
				socketCliente[i] = socketNuevo;
				printf("Agregado a la lista de sockets como socketCliente[%d] = %d\n" , i, socketNuevo);
				posVacia = TRUE;
			}
			i++;
		}

		if(i < CANT_MAX_CLIE && posVacia == TRUE){ //Controla que se haya guardado el socketNuevo
			printf("Recibi una conexion en el cliente numero: %d  tamanioDireccion: %d !!\n\n", socketCliente[i], tamanioDireccionCliente);
			if((bytesRecibidos = recibirMensaje(&(socketCliente[i]), msjCliente)) > 0){
				// Se ha leido un dato del cliente correctamente. Hacer aquí el tratamiento para ese mensaje. En el ejemplo, se lee y se escribe en pantalla.
				parsearMensaje(avion, &opcion, msjCliente);

				printf("Id: %s\n", avion->identificador); // BORRAR
				printf("Modelo: %s\n", avion->modelo); // BORRAR
				printf("combAct: %d\n", avion->combustibleActual); // BORRAR
				printf("combMax: %d\n", avion->combustibleMaximo); // BORRAR
				printf("estado: %d\n", avion->estado); // BORRAR
				printf("Opcion: %d\n", opcion); // BORRAR

				resolverPedidoCliente(&opcion, avion, msjCliente, &socketCliente[i], &listaAviones, &listaAterrizaje, &colaDespegue);
			}else{
				// Hay un error en la lectura. Posiblemente el cliente ha cerrado la conexión. Hacer aquí el tratamiento. En el ejemplo, se cierra el socket y se elimina del array de socketCliente[]

				printf("Mensaje no recibido \n\tbytesRecibidos: %d \n", bytesRecibidos);
			}
		}else{
			printf("Lista de sockets Cliente llena.\n");
		}
	}
	FD_ZERO (&descriptorLectura);
	FD_SET (socketServidor, &descriptorLectura);

	for (i = 0; i < CANT_MAX_CLIE; i++){
		FD_SET (socketCliente[i], &descriptorLectura);
	}

	//Se setea la cuenta regresiva
	//timeout.tv_sec = 3;
	//timeout.tv_usec = 0;
}
*/
