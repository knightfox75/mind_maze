
// Mind Maze - Valores por defecto de las variables globales
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



// Funcion LoadGlobalDefaults();
void LoadGlobalDefaults(void) {

	// Variables
	u8 n = 0;

	///////////////
	// Puzzle 01 //
	///////////////

	// Textos de los puzzles
	sprintf(P01TEXT[0].name, "Buho");
	sprintf(P01TEXT[0].author, "Tonimax");

	sprintf(P01TEXT[1].name, "Subreal");
	sprintf(P01TEXT[1].author, "Tonimax");

	sprintf(P01TEXT[2].name, "Luna");
	sprintf(P01TEXT[2].author, "Tonimax");

	sprintf(P01TEXT[3].name, "Cute!");
	sprintf(P01TEXT[3].author, "SueKeruna");

	sprintf(P01TEXT[4].name, "Lost End");
	sprintf(P01TEXT[4].author, "SueKeruna");

	sprintf(P01TEXT[5].name, "Kyle");
	sprintf(P01TEXT[5].author, "SueKeruna");

	sprintf(P01TEXT[6].name, "I hear voices");
	sprintf(P01TEXT[6].author, "SueKeruna");

	sprintf(P01TEXT[7].name, "Don't leave me");
	sprintf(P01TEXT[7].author, "SueKeruna");

	sprintf(P01TEXT[8].name, "Alehop!");
	sprintf(P01TEXT[8].author, "Miyucchi");

	sprintf(P01TEXT[9].name, "Maiko");
	sprintf(P01TEXT[9].author, "Miyucchi");

	sprintf(P01TEXT[10].name, "Sheriff girl");
	sprintf(P01TEXT[10].author, "Keyframe_79");



	///////////////
	// Puzzle 02 //
	///////////////

	////////////////////////////////////////
	// Informacion de cada mazo de cartas //
	////////////////////////////////////////

	// Girls
	sprintf(PUZZLE02INFO[0].name, "Girls");			// Normbre del mazo
	sprintf(PUZZLE02INFO[0].author, "SueKeruna");	// Autor
	PUZZLE02INFO[0].cards = 11;						// Numero de cartas en el mazo
	PUZZLE02INFO[0].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[0].start_y[0] = 24;
	PUZZLE02INFO[0].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[0].start_y[1] = 136;
			// Control de marcadores //
	PUZZLE02INFO[0].delay = 4;				// Retardo entre frames
	PUZZLE02INFO[0].timer = 90;				// Retardo entre animaciones
	PUZZLE02INFO[0].frames = 8;				// Ultimo frame de la animacion
	PUZZLE02INFO[0].mark_x[0][0] = 24;		// Posicion X marcador 0	(Diestros)
	PUZZLE02INFO[0].mark_y[0][0] = 72;		// Posicion X marcador 0
	PUZZLE02INFO[0].mark_x[1][0] = 72;		// Posicion X marcador 1
	PUZZLE02INFO[0].mark_y[1][0] = 32;		// Posicion X marcador 1
	PUZZLE02INFO[0].mark_x[2][0] = 128;		// Posicion X marcador 2
	PUZZLE02INFO[0].mark_y[2][0] = 16;		// Posicion X marcador 2
	PUZZLE02INFO[0].mark_x[3][0] = 184;		// Posicion X marcador 3
	PUZZLE02INFO[0].mark_y[3][0] = 24;		// Posicion X marcador 3
	PUZZLE02INFO[0].mark_x[0][1] = 200;		// Posicion X marcador 0	(Zurdos)
	PUZZLE02INFO[0].mark_y[0][1] = 80;		// Posicion X marcador 0
	PUZZLE02INFO[0].mark_x[1][1] = 152;		// Posicion X marcador 1
	PUZZLE02INFO[0].mark_y[1][1] = 120;		// Posicion X marcador 1
	PUZZLE02INFO[0].mark_x[2][1] = 96;		// Posicion X marcador 2
	PUZZLE02INFO[0].mark_y[2][1] = 136;		// Posicion X marcador 2
	PUZZLE02INFO[0].mark_x[3][1] = 40;		// Posicion X marcador 3
	PUZZLE02INFO[0].mark_y[3][1] = 128;		// Posicion X marcador 3


	// Corazones
	sprintf(PUZZLE02INFO[1].name, "Hearts");		// Normbre del mazo
	sprintf(PUZZLE02INFO[1].author, "NightFox");	// Autor
	PUZZLE02INFO[1].cards = 12;						// Numero de cartas en el mazo
	PUZZLE02INFO[1].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[1].start_y[0] = 16;
	PUZZLE02INFO[1].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[1].start_y[1] = 128;


	// Diamantes
	sprintf(PUZZLE02INFO[2].name, "Diamonds");		// Normbre del mazo
	sprintf(PUZZLE02INFO[2].author, "NightFox");	// Autor
	PUZZLE02INFO[2].cards = 12;						// Numero de cartas en el mazo
	PUZZLE02INFO[2].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[2].start_y[0] = 16;
	PUZZLE02INFO[2].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[2].start_y[1] = 128;


	// Picas
	sprintf(PUZZLE02INFO[3].name, "Spades");		// Normbre del mazo
	sprintf(PUZZLE02INFO[3].author, "NightFox");	// Autor
	PUZZLE02INFO[3].cards = 12;						// Numero de cartas en el mazo
	PUZZLE02INFO[3].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[3].start_y[0] = 16;
	PUZZLE02INFO[3].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[3].start_y[1] = 128;


	// Treboles
	sprintf(PUZZLE02INFO[4].name, "Clubs");			// Normbre del mazo
	sprintf(PUZZLE02INFO[4].author, "NightFox");	// Autor
	PUZZLE02INFO[4].cards = 12;						// Numero de cartas en el mazo
	PUZZLE02INFO[4].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[4].start_y[0] = 16;
	PUZZLE02INFO[4].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[4].start_y[1] = 128;


	// Inicializaciones comunes a Corazones, Diamantes, Picas, Treboles
	for (n = 1; n <= 4; n ++) {
		PUZZLE02INFO[n].delay = 4;				// Retardo entre frames
		PUZZLE02INFO[n].timer = 90;				// Retardo entre animaciones
		PUZZLE02INFO[n].frames = 8;				// Ultimo frame de la animacion
		PUZZLE02INFO[n].mark_x[0][0] = 48;		// Posicion X marcador 0	(Diestros)
		PUZZLE02INFO[n].mark_y[0][0] = 16;		// Posicion X marcador 0
		PUZZLE02INFO[n].mark_x[1][0] = 24;		// Posicion X marcador 1
		PUZZLE02INFO[n].mark_y[1][0] = 56;		// Posicion X marcador 1
		PUZZLE02INFO[n].mark_x[2][0] = 16;		// Posicion X marcador 2
		PUZZLE02INFO[n].mark_y[2][0] = 96;		// Posicion X marcador 2
		PUZZLE02INFO[n].mark_x[3][0] = 32;		// Posicion X marcador 3
		PUZZLE02INFO[n].mark_y[3][0] = 136;		// Posicion X marcador 3
		PUZZLE02INFO[n].mark_x[0][1] = 176;		// Posicion X marcador 0	(Zurdos)
		PUZZLE02INFO[n].mark_y[0][1] = 144;		// Posicion X marcador 0
		PUZZLE02INFO[n].mark_x[1][1] = 200;		// Posicion X marcador 1
		PUZZLE02INFO[n].mark_y[1][1] = 104;		// Posicion X marcador 1
		PUZZLE02INFO[n].mark_x[2][1] = 208;		// Posicion X marcador 2
		PUZZLE02INFO[n].mark_y[2][1] = 64;		// Posicion X marcador 2
		PUZZLE02INFO[n].mark_x[3][1] = 192;		// Posicion X marcador 3
		PUZZLE02INFO[n].mark_y[3][1] = 24;		// Posicion X marcador 3
	}


	// Zodiaco
	sprintf(PUZZLE02INFO[5].name, "Zodiac");		// Normbre del mazo
	sprintf(PUZZLE02INFO[5].author, "NightFox");	// Autor
	PUZZLE02INFO[5].cards = 14;						// Numero de cartas en el mazo
	PUZZLE02INFO[5].start_x[0] = 176;				// Posicion de las cartas en el tapete (diestro)
	PUZZLE02INFO[5].start_y[0] = 16;
	PUZZLE02INFO[5].start_x[1] = 16;				// Posicion de las cartas en el tapete (zurdo)
	PUZZLE02INFO[5].start_y[1] = 128;
			// Control de marcadores //
	PUZZLE02INFO[5].delay = 4;				// Retardo entre frames
	PUZZLE02INFO[5].timer = 90;				// Retardo entre animaciones
	PUZZLE02INFO[5].frames = 8;				// Ultimo frame de la animacion
	PUZZLE02INFO[5].mark_x[0][0] = 192;		// Posicion X marcador 0	(Diestros)
	PUZZLE02INFO[5].mark_y[0][0] = 16;		// Posicion X marcador 0
	PUZZLE02INFO[5].mark_x[1][0] = 192;		// Posicion X marcador 1
	PUZZLE02INFO[5].mark_y[1][0] = 144;		// Posicion X marcador 1
	PUZZLE02INFO[5].mark_x[2][0] = 32;		// Posicion X marcador 2
	PUZZLE02INFO[5].mark_y[2][0] = 16;		// Posicion X marcador 2
	PUZZLE02INFO[5].mark_x[3][0] = 32;		// Posicion X marcador 3
	PUZZLE02INFO[5].mark_y[3][0] = 144;		// Posicion X marcador 3
	PUZZLE02INFO[5].mark_x[0][1] = 32;		// Posicion X marcador 0	(Zurdos)
	PUZZLE02INFO[5].mark_y[0][1] = 144;		// Posicion X marcador 0
	PUZZLE02INFO[5].mark_x[1][1] = 32;		// Posicion X marcador 1
	PUZZLE02INFO[5].mark_y[1][1] = 16;		// Posicion X marcador 1
	PUZZLE02INFO[5].mark_x[2][1] = 192;		// Posicion X marcador 2
	PUZZLE02INFO[5].mark_y[2][1] = 144;		// Posicion X marcador 2
	PUZZLE02INFO[5].mark_x[3][1] = 192;		// Posicion X marcador 3
	PUZZLE02INFO[5].mark_y[3][1] = 16;		// Posicion X marcador 3



	///////////////
	// Puzzle 03 //
	///////////////

	/*
	Atencion!
	El motor 3D de DS usa como unidad el numero 16:1
	Esto significa que un pixel real equivale a 16 unidades
	Por lo que todas las coordenadas reales se conviertes
	con el siguiente bitshift (1 << 4)
	*/


	//////////////////////
	// Shape de la bola //
	//////////////////////
	
	P03_BALL_SHAPE[0] = 4;
	P03_BALL_SHAPE[1] = 6;
	P03_BALL_SHAPE[2] = 7;
	P03_BALL_SHAPE[3] = 8;
	P03_BALL_SHAPE[4] = 8;
	P03_BALL_SHAPE[5] = 9;
	P03_BALL_SHAPE[6] = 9;
	P03_BALL_SHAPE[7] = 9;


	///////////////////////////////////
	// Informacion de cada Laberinto //
	///////////////////////////////////

	// Cueva
	sprintf(PUZZLE03INFO[0].name, "Cave");			// Normbre del laberinto
	sprintf(PUZZLE03INFO[0].author, "NightFox");	// Autor
	PUZZLE03INFO[0].start_x[0] = 472;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[0].start_y[0] = 48;
	PUZZLE03INFO[0].start_x[1] = 40;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[0].start_y[1] = 464;
	PUZZLE03INFO[0].width = 512;					// Medidas del laberinto
	PUZZLE03INFO[0].height = 512;

	// Infierno
	sprintf(PUZZLE03INFO[1].name, "Inferno");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[1].author, "NightFox");	// Autor
	PUZZLE03INFO[1].start_x[0] = 328;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[1].start_y[0] = 336;
	PUZZLE03INFO[1].start_x[1] = 440;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[1].start_y[1] = 432;
	PUZZLE03INFO[1].width = 768;					// Medidas del laberinto
	PUZZLE03INFO[1].height = 768;

	// Nuvoso
	sprintf(PUZZLE03INFO[2].name, "Cloudy");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[2].author, "NightFox");	// Autor
	PUZZLE03INFO[2].start_x[0] = 280;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[2].start_y[0] = 984;
	PUZZLE03INFO[2].start_x[1] = 744;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[2].start_y[1] = 40;
	PUZZLE03INFO[2].width = 1024;					// Medidas del laberinto
	PUZZLE03INFO[2].height = 1024;

	// Volcano
	sprintf(PUZZLE03INFO[3].name, "Volcano");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[3].author, "NightFox");	// Autor
	PUZZLE03INFO[3].start_x[0] = 736;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[3].start_y[0] = 32;
	PUZZLE03INFO[3].start_x[1] = 32;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[3].start_y[1] = 736;
	PUZZLE03INFO[3].width = 768;					// Medidas del laberinto
	PUZZLE03INFO[3].height = 768;

	// Paranoia
	sprintf(PUZZLE03INFO[4].name, "Paranoia");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[4].author, "NightFox");	// Autor
	PUZZLE03INFO[4].start_x[0] = 976;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[4].start_y[0] = 48;
	PUZZLE03INFO[4].start_x[1] = 48;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[4].start_y[1] = 976;
	PUZZLE03INFO[4].width = 1024;					// Medidas del laberinto
	PUZZLE03INFO[4].height = 1024;

	// 8 bits
	sprintf(PUZZLE03INFO[5].name, "8 bits");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[5].author, "NightFox");	// Autor
	PUZZLE03INFO[5].start_x[0] = 480;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[5].start_y[0] = 32;
	PUZZLE03INFO[5].start_x[1] = 32;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[5].start_y[1] = 480;
	PUZZLE03INFO[5].width = 512;					// Medidas del laberinto
	PUZZLE03INFO[5].height = 512;

	// Hurry!
	sprintf(PUZZLE03INFO[6].name, "On Ice!");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[6].author, "NightFox");	// Autor
	PUZZLE03INFO[6].start_x[0] = 480;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[6].start_y[0] = 32;
	PUZZLE03INFO[6].start_x[1] = 32;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[6].start_y[1] = 480;
	PUZZLE03INFO[6].width = 512;					// Medidas del laberinto
	PUZZLE03INFO[6].height = 512;

	// Sandbox!
	sprintf(PUZZLE03INFO[7].name, "Sandbox");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[7].author, "NightFox");	// Autor
	PUZZLE03INFO[7].start_x[0] = 480;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[7].start_y[0] = 480;
	PUZZLE03INFO[7].start_x[1] = 544;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[7].start_y[1] = 544;
	PUZZLE03INFO[7].width = 1024;					// Medidas del laberinto
	PUZZLE03INFO[7].height = 1024;

	// Rainbow!
	sprintf(PUZZLE03INFO[8].name, "Rainbow");		// Normbre del laberinto
	sprintf(PUZZLE03INFO[8].author, "NightFox");	// Autor
	PUZZLE03INFO[8].start_x[0] = 736;				// Posicion inicial de la bola (diestro)
	PUZZLE03INFO[8].start_y[0] = 32;
	PUZZLE03INFO[8].start_x[1] = 32;				// Posicion inicial de la bola (zurdo)
	PUZZLE03INFO[8].start_y[1] = 736;
	PUZZLE03INFO[8].width = 768;					// Medidas del laberinto
	PUZZLE03INFO[8].height = 768;

}
