
// Mind Maze - Animaciones de presentacion
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010



// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


// Includes propietarios NDS
#include <nds.h>

// Includes NightFox's Lib
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion Opening();
void Opening(void) {

	// Pantalla en negro y espera 30 frames
	setBrightness(3, -16);
	WaitTime(15);

	// Pantallas de las compos
	CompoScreens();

	// Carga y reproduce la musica del Opening/Menu
	LoadBgm(MOD_MAIN);
	PlayBgm();

	// Video de la intro
	OpeningMovie();

	// Pantallas del juego
	SplashScreens();

}



// Funcion CompoScreens();
void CompoScreens(void) {

	// Resetea todo
	ResetAll();

	///////////////////////////////
	// Pon la DS en modo 16 bits //
	///////////////////////////////

	// Inicializa el motor 2D en modo BITMAP
	NF_Set2D(0, 5);				// Modo 2D_5 en ambas pantallas
	NF_Set2D(1, 5);
	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();
	// Inicializa los fondos en modo "BITMAP"
	NF_InitBitmapBgSys(0, 1);
	NF_InitBitmapBgSys(1, 1);


	///////////////////////
	// Carga de graficos //
	///////////////////////

	// Variables
	u8 slot[2];					// Slots para asignar las imagenes
	u8 thread = 0;				// Proceso
	bool next = false;			// Flag del siguiente proceso
	bool loop = true;			// Control del LOOP

	bool fade_in = false;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	s16 time = 0;				// Contador de tiempo
	s16 time_goal = 0;			// Tiempo objetivo
	bool timer = false;			// Activa el temporizador

	// Configura los slots dependiendo de la mano
	if (GAMEOPTIONS[0].lefthanded) {
		slot[0] = 1;
		slot[1] = 0;
	} else {
		slot[0] = 0;
		slot[1] = 1;
	}

	// Carga las imagenes necesarias
	NF_Load16bitsBg("opening/bg/neocompo2011", slot[0]);
	NF_Load16bitsBg("opening/bg/scenery2011", slot[1]);

	// Si eres zurdo, rota 180º las imagenes
	if (GAMEOPTIONS[0].lefthanded) {
		Rotate16bitsImage(0);
		Rotate16bitsImage(1);
	}



	/////////////////////////
	// Nucleo de ejecucion //
	/////////////////////////

	// Ejecucion del Splash Screen
	while (loop) {

		// Lee el teclado
		ReadKeypad();

		// Effecto "fade in"
		if (fade_in) {
			fade -= 64;
			if (fade <= 0) {
				fade = 0;
				fade_in = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Effecto "fade out"
		if (fade_out) {
			fade += 64;
			if (fade >= (16 << 8)) {
				fade = (16 << 8);
				fade_out = false;
				loop = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Abortar? (Salir del menu)
		if (!fade_in && !fade_out && loop && AbortKeys()) {
			fade_out = true;
		}

		// Nucleo de la funcion
		if (loop && !fade_in && !fade_out && !timer) {

			switch (thread) {

				case 0:		// Fade In
					// Manda las imagenes a la pantalla
					NF_Copy16bitsBuffer(0, 0, 0);	
					NF_Copy16bitsBuffer(1, 0, 1);
					// Y activa el fade in
					fade_in = true;
					next = true;
					break;

				case 1:		// Muestra las pantallas de SceneryBeta 2010 con fade in durante 90 frames
					time = 0;
					time_goal = 90;
					timer = true;
					break;

				case 2:		// Fade Out
					fade_out = true;
					next = true;
					break;

				default:	// Nada
					break;

			}

		}

		// Temporizador
		if (timer) {
			time ++;
			if (time >= time_goal) {
				time = 0;
				time_goal = 0;
				timer = false;
				next = true;
			}
		}

		// Si debes saltar al siguiente thread, hazlo
		if (next) {
			thread ++;
			next = false;
		}

		swiWaitForVBlank();		// Espera al sincronismo vertical

	}


	/////////////////////////////
	// Resetea el modo 16 bits //
	/////////////////////////////

	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();

}



// Funcion SplashScreens();
void SplashScreens(void) {

	// Resetea todo
	ResetAll();

	///////////////////////////////
	// Pon la DS en modo 16 bits //
	///////////////////////////////

	// Inicializa el motor 2D en modo BITMAP
	NF_Set2D(0, 5);				// Modo 2D_5 en ambas pantallas
	NF_Set2D(1, 5);
	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();
	// Inicializa los fondos en modo "BITMAP"
	NF_InitBitmapBgSys(0, 1);
	NF_InitBitmapBgSys(1, 1);
	// Habilita el backbuffer en ambas pantallas
	NF_Enable16bitsBackBuffer(0);
	NF_Enable16bitsBackBuffer(1);


	u8 thread = 0;				// Proceso
	bool next = false;			// Flag del siguiente proceso
	bool loop = true;			// Control del LOOP
	u8 slot[4];					// Slots para asignar las imagenes

	bool fade_in = false;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	bool abort = false;			// Se debe abortar?

	s16 time = 0;				// Contador de tiempo
	s16 time_goal = 0;			// Tiempo objetivo
	bool timer = false;			// Activa el temporizador

	// Configura los slots dependiendo de la mano
	if (GAMEOPTIONS[0].lefthanded) {
		slot[0] = 1;
		slot[1] = 0;
		slot[2] = 3;
		slot[3] = 2;
	} else {
		slot[0] = 0;
		slot[1] = 1;
		slot[2] = 2;
		slot[3] = 3;
	}

	// Carga las imagenes necesarias
	NF_Load16bitsBg("opening/bg/devkitpro", slot[0]);
	NF_Load16bitsBg("opening/bg/nflib", slot[1]);
	NF_Load16bitsBg("opening/bg/nfc_left", slot[2]);
	switch (GAMEOPTIONS[0].language) {
		case 0:		// Español
			NF_Load16bitsBg("opening/bg/nfc_right_es", slot[3]);
			break;
		case 1:		// Ingles
			NF_Load16bitsBg("opening/bg/nfc_right_en", slot[3]);
			break;
		default:
			break;
	}
	// Si eres zurdo, rota 180º las imagenes
	if (GAMEOPTIONS[0].lefthanded) {
		Rotate16bitsImage(0);
		Rotate16bitsImage(1);
		Rotate16bitsImage(2);
		Rotate16bitsImage(3);
	}


	// Ejecucion del Splash Screen
	while (loop) {

		// Lee el teclado
		ReadKeypad();

		// Effecto "fade in"
		if (fade_in) {
			fade -= 64;
			if (fade <= 0) {
				fade = 0;
				fade_in = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Effecto "fade out"
		if (fade_out) {
			fade += 64;
			if (fade >= (16 << 8)) {
				fade = (16 << 8);
				fade_out = false;
				loop = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Abortar? (Salir del menu)
		if (!fade_in && !fade_out && loop && AbortKeys()) {
			fade_out = true;
		}

		// Nucleo de la funcion
		if (loop && !fade_in && !fade_out && !timer) {

			// Nucleo de la funcion
			switch (thread) {

				case 0:		// Fade In
					// Copia las imagenes iniciales a la pantalla
					NF_Copy16bitsBuffer(0, 1, 0);
					NF_Copy16bitsBuffer(1, 1, 1);
					NF_Flip16bitsBackBuffer(0);
					NF_Flip16bitsBackBuffer(1);
					// Y activa el fade in
					fade_in = true;
					next = true;
					break;

				case 1:		// Muestra las pantallas de devkitPro y NightFox's Lib durante 90 frames
					time = 0;
					time_goal = 90;
					timer = true;
					break;

				case 2:		// Cross Disolve a NightFox & Co. presenta
					if (!abort) abort = CrossDisolve(0, 0, 2);
					if (!abort) abort = CrossDisolve(1, 1, 3);
					if (!abort) {
						next = true;
					} else {
						thread = 255;
						fade_out = true;
					}
					break;

				case 3:		// Muestra las pantallas de NightFox & Co. presenta durante 90 frames
					time = 0;
					time_goal = 90;
					timer = true;
					break;

				case 4:		// Fade Out
					fade_out = true;
					next = true;
					break;

				default:	// Nada
					break;

			}

		}

		// Temporizador
		if (timer) {
			time ++;
			if (time >= time_goal) {
				time = 0;
				time_goal = 0;
				timer = false;
				next = true;
			}
		}

		// Si debes saltar al siguiente thread, hazlo
		if (next) {
			thread ++;
			next = false;
		}

		swiWaitForVBlank();		// Espera al sincronismo vertical

	}


	/////////////////////////////
	// Resetea el modo 16 bits //
	/////////////////////////////

	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();
	// Resetea los backbuffers y deshabilitalos
	NF_Disble16bitsBackBuffer(0);
	NF_Disble16bitsBackBuffer(1);	

}



// Funcion OpeningMovie();
void OpeningMovie(void) {

	// Variables locales
	bool loop = true;			// Flag del loop de ejecucion
	bool play = true;			// Flag de reproduccion activa
	s32 timer = 0;				// Tiempo en stand by
	bool fade_in = true;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE
	s32 bg_pos = 0;				// Posicion del fondo

	// Resetalo todo
	ResetAll();

	// Carga el fondo de la derecha
	NF_LoadTiledBg("opening/bg/opandco", "opandco", 512, 256);
	// Si eres zurdo, rota 180º la imagene
	if (GAMEOPTIONS[0].lefthanded) {
		RotateTiledBg("opandco");
	}


	// Inicializa el engine de animacion y
	// abre el stream de animacion
	if (GAMEOPTIONS[0].lefthanded) {
		NF_EnableAnimationEngine(1, 0);
		NF_OpenAnimationStream("opening/video/open_l", false);
		NF_CreateTiledBg(0, 3, "opandco");
		NF_ScrollBg(0, 3, 256, 64);
	} else {
		NF_EnableAnimationEngine(0, 0);
		NF_OpenAnimationStream("opening/video/open_r", false);
		NF_CreateTiledBg(1, 3, "opandco");
	}


	// Reproduce el video
	while(loop) {

		// Lee el teclado
		ReadKeypad();

		// Effecto "fade in"
		if (fade_in) {
			fade -= 64;
			if (fade <= 0) {
				fade = 0;
				fade_in = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Effecto "fade out"
		if (fade_out) {
			fade += 64;
			if (fade >= (16 << 8)) {
				fade = (16 << 8);
				fade_out = false;
				loop = false;
			}
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Abortar? (Salir del menu)
		if (!fade_in && !fade_out && loop && AbortKeys()) {
			fade_out = true;
		}

		// Actualiza el stream
		if (play) play = NF_UpdateAnimacionStream();

		// Si ha finalizado el video, espera 2 segundos
		if (!play && !fade_in && !fade_out) {
			timer ++;
			if (timer >= 60) {
				fade_out = true;
			}
		}

		// Control de posicion del fondo
		bg_pos += 219;
		if (bg_pos > (256 << 8)) bg_pos = (256 << 8);

		// Espera al sincronismo vertical
		swiWaitForVBlank();

		// Posiciona el fondo
		if (GAMEOPTIONS[0].lefthanded) {
			NF_ScrollBg(0, 3, (bg_pos >> 8), 64);
		} else {
			NF_ScrollBg(1, 3, (256 - (bg_pos >> 8)), 0);
		}
	
	}

	// Cierra el stream
	NF_CloseAnimationStream();

	// Deten el engine de animacion y vacia todos los buffers usados
	NF_DisableAnimationEngine();

}
