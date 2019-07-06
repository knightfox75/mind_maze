
// Mind Maze - Funciones del Puzzle 2 - Parejas (Nucleo)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010



// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>


// Includes propietarios NDS
#include <nds.h>

// Includes NightFox's Lib
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion Puzzle02();
void Puzzle02(void) {

	// Variables
	bool loop = true;	//Control del bucle
	u8 thread = 1;		// Hilo
	u8 option = 0;		// Opcion devuelta por el menu

	// Nucleo de ejecucion
	while (loop) {
		switch (thread) {
			case 1:	// Menu de seleccion de puzzle
				option = MenuPuzzle02();
				// Si no se ha cancelado...
				if (option != 255) {
					thread = KernelPuzzle02(option);
				} else {
					loop = false;
				}
				break;
			case 2:	// Sal al menu principal
				loop = false;
				break;
			default:
				break;
		}
	}

}
