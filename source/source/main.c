/*
-------------------------------------------------

	// Mind Maze - Main();
	// Requiere DevkitARM
	// Requiere NightFox's Lib
	// Codigo por NightFox
	// http://www.nightfoxandco.com
	// Inicio 13 de Diciembre del 2010

-------------------------------------------------
*/





/*
-------------------------------------------------
	Includes
-------------------------------------------------
*/

// Includes C
#include <stdio.h>

// Includes propietarios NDS
#include <nds.h>

// Includes librerias propias
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"





/*
-------------------------------------------------
	Main() - Bloque general del programa
-------------------------------------------------
*/

int main(int argc, char **argv) {

	// Debug (Exception Handler)
	// defaultExceptionHandler();

	// Inicializa el hardware de la DS, la libreria y los datos del juego
	BootGame();

	// Verifica el archivo de guardado de datos
	CheckSavegame(argv[0]);

	// Opening del juego
	Opening();

	// Bucle infinito de ejecucion;
	u8 menu = 0;
	while (1) {

		// Segun la opcion del menu...
		menu = MainMenu();

		switch (menu) {

			// Puzzle deslizante
			case 0:
				Puzzle01();
				break;

			// Parejas
			case 1:
				Puzzle02();
				break;

			// Laberinto
			case 2:
				Puzzle03();
				break;

			// Puzzle de cubos
			case 3:
				KernelPuzzle04();
				break;

			// Creditos
			case 4:
				CreditsMain();
				break;

			// Opciones
			case 5:
				OptMenu();
				break;

			// Error trap
			default:
				break;

		}

	}

	return 0; 

}
