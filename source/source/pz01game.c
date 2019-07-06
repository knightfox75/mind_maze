
// Mind Maze - Funciones del Puzzle 1 - Puzzle deslizante (Juego)
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





// Funcion KernelPuzzle01()
u8 KernelPuzzle01(u8 puzzle) {

	// Reinicialo todo
	ResetAll();

	// Variables
	u8 end = 0;			// Resultado del puzzle
	u8 option = 0;		// Opcion del menu de fin de puzzle
	s32 fade = 0;		// Brillo
	bool loop = true;	// Loop

	
	//////////////////////
	//					//
	//  Carga Archivos	//
	//					//
	//////////////////////

	// Pantalla "Cargando..."
	LoadingIn();
	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();
	// Carga los archivos necesarios
	LoadPuzzle01Files(puzzle);
	// Carga la musica
	LoadBgm((MOD_P01STG01 + puzzle));
	// Oculta la pantalla "Cargando..."
	LoadingOut();


	/////////////////////////////////////////////
	//										   //
	//  Pon la DS en modo 16 bits (SUBSCREEN)  //
	//										   //
	/////////////////////////////////////////////

	// Inicializa el motor 2D en modo BITMAP
	NF_Set2D(1, 5);
	// Inicializa los fondos en modo "BITMAP"
	NF_InitBitmapBgSys(1, 1);
	// Inicializa los Sprites
	NF_InitSpriteSys(1, 128);


	//////////////////////
	//					//
	//	Crea el Puzzle	//
	//					//
	//////////////////////

	// Crea el puzzle
	CreatePuzzle01();
	// Reproduce la musica
	PlayBgm();


	//////////////////////////
	//						//
	//	Nucleo el Puzzle	//
	//						//
	//////////////////////////

	// Fade in
	FadeIn(3, 64);

	// Repite mientras no pidas otro puzzle o salir
	while (loop) {

		// Mezcla el puzle (50 por defecto)
		MixPuzzle01(PUZZLE01_MIXMOVES);

		// Suma un contador al numero de veces jugadas
		SAVEDATA[0].played[puzzle] ++;
		
		// Juega al puzzle
		end = PlayPuzzle01(puzzle);

		// Al terminar el puzzle...
		EndPuzzle01(end);

		// Menu de final de puzzle
		option = EndMenuPuzzle01(end);

		// Si has seleccionado repetir el mismo puzzle
		if (option == 0) {
			RetryPuzzle01();
		} else {
			loop = false;
		}

	}

	// Fade out de la pantalla y musica
	loop = true;
	while (loop) {
		fade += 64;
		if (fade >= (16 << 8)) {
			fade = (16 << 8);
			loop = false;
			// Deten la musica
			StopBgm();
		}
		// Ajusta el brillo
		setBrightness(3, (0 - (fade >> 8)));
		// Ajusta el volumen
		BgmVolume(((4096 - fade) >> 2));
		// Espera al sincronismo
		swiWaitForVBlank();
	}


	///////////////////////////////
	//							 //
	//  Resetea el modo 16 bits  //
	//							 //
	///////////////////////////////

	// Resetea los buffers de 16 bits
	NF_Reset16bitsBgBuffers();


	// Devuelve el resultado de ejecucion
	// 0 - Repetir puzzle
	// 1 - Cambiar puzzle
	// 2 - Salir
	return option;

}



// Funcion MixPuzzle01()
void MixPuzzle01(u8 amount) {

	// Variables
	bool loop = true;		// Loop de ejecucion del puzzle
	u8 move = 0;			// Flag movimiento fichas
	u8 id = 0;				// Identificador de la ficha seleccionada
	u8 last = 255;			// Ultima ficha movida
	u8 count = 0;			// Contador de fichas movidas
	u8 n = 0;				// Uso general
	bool slide = false;		// Flag deslizamiento de la ficha

	// Inicializa el random
	srand(time(NULL));

	// Loop de ejecuccion
	while(loop) {

		// Si no se esta moviendo ninguna ficha....
		if (!slide) {

			// Calcula el siguiente movimiento
			id = 255;
			while (id == 255) {
				n = (int)(rand() % 4);
				// Segun el aleatorio...
				switch (n) {
					case 0:		// Arriba
						if (PUZZLE01SPR[11].gy > 0) {
							id = PUZZLE01MOV.id[PUZZLE01SPR[11].gx][(PUZZLE01SPR[11].gy - 1)];
						}
						break;
					case 1:		// Abajo
						if (PUZZLE01SPR[11].gy < 3) {
							id = PUZZLE01MOV.id[PUZZLE01SPR[11].gx][(PUZZLE01SPR[11].gy + 1)];
						}
						break;
					case 2:
						if (PUZZLE01SPR[11].gx < 2) {
							id = PUZZLE01MOV.id[(PUZZLE01SPR[11].gx + 1)][PUZZLE01SPR[11].gy];
						}
						break;
					case 3:
						if (PUZZLE01SPR[11].gx > 0) {
							id = PUZZLE01MOV.id[(PUZZLE01SPR[11].gx - 1)][PUZZLE01SPR[11].gy];
						}
						break;
					default:
						break;
				}
				// Si es la misma pieza... escoge otra
				if (last == id) id = 255;
			}
			// Si obtienes  un movimiento valido, validalo
			move = GetPuzzle01Move(id);
			PUZZLE01MOV.target = id;
			PUZZLE01MOV.move = move;
			PUZZLE01MOV.count = 0;
			last = id;
			slide = true;

		} else {

			// Mueve la ficha seleccionada
			PUZZLE01SPR[id].x += PUZZLE01MOV.add_x;
			PUZZLE01SPR[id].y += PUZZLE01MOV.add_y;
			NF_MoveSprite(1, (id + PUZZLE01_SPRITE), (PUZZLE01SPR[id].x - 4), (PUZZLE01SPR[id].y - 4));
			// Cuenta el movimiento
			PUZZLE01MOV.count += PUZZLE01_SPEED;
			// Si se ha completado el movimiento
			if (PUZZLE01MOV.count >= 56) {
				// Verifica la posicion de cada ficha
				Puzzle01CheckGoodPlace();
				// Deten el movimiento
				slide = false;
				// Suma al contador
				count ++;
			}

		}

		// Si se han mezclado 'n' veces...
		if (count >= amount) loop = false;

		// Dibujado en pantalla y sincronismo
		UpdateBothScreens2D();

	}

}



// Funcion PlayPuzzle01()
u8 PlayPuzzle01(u8 puzzle) {

	// Variables
	bool loop = true;		// Loop de ejecucion del puzzle
	bool touch = false;		// Control de primer toque
	u8 move = 0;			// Flag movimiento fichas
	u8 id = 0;				// Identificador de la ficha tocada
	u8 good = 0;			// Piezas en orden
	bool slide = false;		// Flag deslizamiento de la ficha
	u8 result = 0;			// Resultado de la ejecuccion
	u8 tics = 0;			// Control del tiempo de juego
	u32 time = 0;			// Tiempo de juego
	u32 pmoves = 0;			// Movimientos realizados

	// Loop de ejecuccion
	while(loop) {

		// Lee el touchpad y teclado
		ReadKeypad();
		ReadTouchpad();

		// Si no se esta moviendo ninguna ficha....
		if (!slide) {

			// Si el lapiz toca la pantalla...
			if (TOUCHPAD.held) {
				// Si es el primer toque
				if (!touch) {
					// Marcalo
					touch = true;
					// Obten el ID por si se ha tocado algun Sprite
					id = GetPuzzle01SpriteId();
					// Si has tocado algun Sprite valido...
					if (id != 255) {
						move = GetPuzzle01Move(id);
						// Si ese sprite puede moverse...
						if (move != 0) {
							// Indica todos los parametros para el movimiento
							PUZZLE01MOV.target = id;
							PUZZLE01MOV.move = move;
							PUZZLE01MOV.count = 0;
							slide = true;
						}
					}
				}
			} else {
				// Si no lo toca, marcalo
				touch = false;
			}

		} else {

			// Mueve la ficha seleccionada
			PUZZLE01SPR[id].x += PUZZLE01MOV.add_x;
			PUZZLE01SPR[id].y += PUZZLE01MOV.add_y;
			NF_MoveSprite(1, (id + PUZZLE01_SPRITE), (PUZZLE01SPR[id].x - 4), (PUZZLE01SPR[id].y - 4));
			// Cuenta el movimiento
			PUZZLE01MOV.count += PUZZLE01_SPEED;
			// Si se ha completado el movimiento
			if (PUZZLE01MOV.count >= 56) {
				// Verifica la posicion de cada ficha
				good = Puzzle01CheckGoodPlace();
				// Deten el movimiento
				slide = false;
				// Y registralo
				pmoves ++;
			}

		}

		// Si has pulsado la tecla de cancelar
		if (loop && !slide && AbortKeys()) {
			loop = false;
			result = 1;
		}

		// Si se ha resuelto el puzzle, sal
		if (good == 12) {
			loop = false;
			result = 0;
		}

		// Calculo del tiempo
		tics ++;
		if (tics > 59) {
			tics = 0;
			time ++;
		}

		// Dibujado en pantalla y sincronismo
		UpdateBothScreens2D();

	}

	// Si se ha resuelto el puzzle, calcula los resultados
	if (result == 0) Puzzle01Results(puzzle, time, pmoves);

	// Devuelve el resultado de la ejecucion
	// 0 - Completado
	// 1 - Abortado
	return result;

}


// Funcion EndPuzzle01();
void EndPuzzle01(u8 end) {

	// Variables
	s16 n = 0;
	bool loop = true;
	s32 y = 0;
	s32 speed = 0;

	// FadeIn a blanco
	for (n = 0; n <= 16; n ++) {
		setBrightness(2, n);
		swiWaitForVBlank();
	}

	// Dependiendo del resultado del puzzle
	switch (end) {
		case 0:		// Puzzle resuelto
			// Cambia la imagen de fondo
			NF_Copy16bitsBuffer(1, 0, 1);
			break;
		case 1:		// Puzzle cancelado
			loop = false;
			break;
		default:
			break;
	}

	// Borra todos los Sprites de la pantalla tactil
	for (n = 0; n < 12; n ++) {
		NF_DeleteSprite(1, (PUZZLE01_SPRITE + n));
		NF_FreeSpriteGfx(1, n);
	}
	NF_FreeSpriteGfx(1, 12);
	// Y actualiza las pantallas
	UpdateBothScreens2D();

	// FadeOut desde blanco
	for (n = 16; n >= 0; n --) {
		setBrightness(2, n);
		swiWaitForVBlank();
	}

	
	// Entrada del report, si se ha resuelto el puzzle
	if (end == 0) {
		y = (192 << 8);
		while (y > 0) {
			speed = (y >> 3);
			if (speed < 512) speed = 512;
			y -= speed;
			if (y < 0) y = 0;
			if (GAMEOPTIONS[0].lefthanded) {
				NF_ScrollBg(0, 2, 0, (320 - (y >> 8)));
				NF_ScrollBg(0, 1, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(0, 2, 0, (y >> 8));
				NF_ScrollBg(0, 1, 0, (y >> 8));
			}
			swiWaitForVBlank();
		}
	}


	// Espera un toque en la pantalla o 5 segundos (si no es una cancelacion)
	n = 300;
	while(loop) {
		ReadTouchpad();
		n --;
		if (TOUCHPAD.held || (n <= 0)) loop = false;
		swiWaitForVBlank();
	}

}



// Funcion EndMenuPuzzle01();
u8 EndMenuPuzzle01(u8 end) {

	// Variables
	s16 n = 0;			// Uso general
	bool loop = false;	// Control de bucles
	s32 x[3];			// Posicion de los botones
	s32 in[4];			// Distancia de entrada
	s32 out[4];			// Distancia de salida
	s32 y = 0;			// Vertical de los botones y posicion de la capa de report
	s32 speed = 0;		// Calculo de velocidad
	u8 id = 0;			// ID de sprite
	bool touch = false;	// Control tactil
	u8 option = 0;		// Opcion seleccionada

	// Inicializa las variables
	if (GAMEOPTIONS[0].lefthanded) {
		x[0] = (60 << 8);					// Posicion de los botones
		x[1] = (108 << 8);
		x[2] = (156 << 8);
		in[0] = ((32 << 8) + x[0]);			// Distancia
		in[1] = ((32 << 8) + x[1]);
		in[2] = ((32 << 8) + x[2]);
		in[3] = 0;
		out[0] = ((256 << 8) - x[0]);		// Distancia
		out[1] = ((256 << 8) - x[1]);
		out[2] = ((256 << 8) - x[2]);
		out[3] = (256 << 8);
	} else {
		x[0] = (172 << 8);					// Posicion de los botones
		x[1] = (124 << 8);
		x[2] = (76 << 8);
		in[0] = ((256 << 8) - x[0]);		// Distancia
		in[1] = ((256 << 8) - x[1]);
		in[2] = ((256 << 8) - x[2]);
		in[3] = 0;
		out[0] = ((32 << 8) + x[0]);		// Distancia
		out[1] = ((32 << 8) + x[1]);
		out[2] = ((32 << 8) + x[2]);
		out[3] = (256 << 8);

	}
	y = 32;


	///////////////////////////////
	// Crea los botones del menu //
	///////////////////////////////

	// Carga la paleta
	NF_VramSpritePal(1, 13, 0);
	// Carga los graficos
	for (n = 0; n < 6; n ++) {
		NF_VramSpriteGfx(1, (13 + n), n, false);
	}

	// Crea los sprites
	for (n = 0; n < 3; n ++) {
		id = (n << 1);
		if (GAMEOPTIONS[0].lefthanded) {
			NF_CreateSprite(1, (id + 1), (id + 1), 0, (((x[n] - in[n]) >> 8) - 4), y);
			NF_CreateSprite(1, id, id, 0, (((x[n] - in[n]) >> 8) - 4), (y + 64));
		} else {
			NF_CreateSprite(1, id, id, 0, (((x[n] + in[n]) >> 8) - 4), y);
			NF_CreateSprite(1, (id + 1), (id + 1), 0, (((x[n] + in[n]) >> 8) - 4), (y + 64));
		}
	}
	// Actualiza ambas pantallas
	UpdateBothScreens2D();


	////////////////////////////
	// Entrada de los botones //
	////////////////////////////

	loop = true;
	while (loop) {
		// Calcula y mueve los botones
		for (n = 2; n >= 0; n --) {
			if ((in[n] > 0) && ((in[(n + 1)] < (48 << 8)))) {
				speed = (in[n] >> 3);
				if (speed < 512) speed = 512;
				in[n] -= speed;
				if (in[n] < 0) in[n] = 0;
				id = (n << 1);
				if (GAMEOPTIONS[0].lefthanded) {
					NF_MoveSprite(1, (id + 1), (((x[n] - in[n]) >> 8) - 4), y);
					NF_MoveSprite(1, id, (((x[n] - in[n]) >> 8) - 4), (y + 64));
				} else {
					NF_MoveSprite(1, id, (((x[n] + in[n]) >> 8) - 4), y);
					NF_MoveSprite(1, (id + 1), (((x[n] + in[n]) >> 8) - 4), (y + 64));
				}
			}
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == 0) && (in[1] == 0) && (in[2] == 0)) loop = false;
		// Actualiza ambas pantallas
		UpdateBothScreens2D();
	}


	////////////////////////
	// Seleccion del menu //
	////////////////////////

	loop = true;
	while (loop) {
		// Lee el tactil
		ReadTouchpad();
		// Si el lapiz toca la pantalla...
		if (TOUCHPAD.held) {
			// Si es el primer toque
			if (!touch) {
				// Marcalo
				touch = true;
				// Verifica los botones
				for (n = 0; n < 3; n ++) {
					if (
						(TOUCHPAD.pos_x >= (x[n] >> 8))
						&&
						(TOUCHPAD.pos_x <= ((x[n] >> 8) + 24))
						&&
						(TOUCHPAD.pos_y >= y)
						&&
						(TOUCHPAD.pos_y <= (y + 128))
						) {
						// Si has tocado un boton, registralo y sal
						loop = false;
						option = n;
					}
				}
			}
		} else {
			// Si no lo toca, marcalo
			touch = false;
		}
		// Espera al sincronismo
		swiWaitForVBlank();
	}


	///////////////////////////
	// Salida de los botones //
	///////////////////////////

	loop = true;
	in[3] = out[3];
	while (loop) {
		// Calcula y mueve los botones
		for (n = 2; n >= 0; n --) {
			if ((in[n] < out[n]) && ((in[(n + 1)] > (24 << 8)))) {
				speed = (in[n] >> 3);
				if (speed < 512) speed = 512;
				in[n] += speed;
				if (in[n] > out[n]) in[n] = out[n];
				id = (n << 1);
				if (GAMEOPTIONS[0].lefthanded) {
					NF_MoveSprite(1, (id + 1), (((x[n] + in[n]) >> 8) - 4), y);
					NF_MoveSprite(1, id, (((x[n] + in[n]) >> 8) - 4), (y + 64));
				} else {
					NF_MoveSprite(1, id, (((x[n] - in[n]) >> 8) - 4), y);
					NF_MoveSprite(1, (id + 1), (((x[n] - in[n]) >> 8) - 4), (y + 64));
				}
			}
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == out[0]) && (in[1] == out[1]) && (in[2] == out[2])) loop = false;
		// Actualiza ambas pantallas
		UpdateBothScreens2D();
	}


	////////////////////////////////////////
	// Salida del report, si es necesario //
	////////////////////////////////////////

	// Salida del report, si se ha resuelto el puzzle
	if (end == 0) {
		y = 0;
		while (y < (192 << 8)) {
			speed = (y >> 3);
			if (speed < 512) speed = 512;
			y += speed;
			if (y > (192 << 8)) y = (192 << 8);
			if (GAMEOPTIONS[0].lefthanded) {
				NF_ScrollBg(0, 2, 0, (320 - (y >> 8)));
				NF_ScrollBg(0, 1, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(0, 2, 0, (y >> 8));
				NF_ScrollBg(0, 1, 0, (y >> 8));
			}
			swiWaitForVBlank();
		}
	}

	// Devuelve la opcion
	return option;

}



// Funcion RetryPuzzle01();
void RetryPuzzle01(void) {

	// Variables
	s16 n = 0;

	// FadeIn a blanco
	for (n = 0; n <= 16; n ++) {
		setBrightness(2, n);
		swiWaitForVBlank();
	}

	// Elimina los botones del menu
	for (n = 0; n < 6; n ++) {
		NF_DeleteSprite(1, n);
	}
	for (n = 0; n < 6; n ++) {
		NF_FreeSpriteGfx(1, n);
	}
	// Actualiza el contenido de la pantalla
	UpdateBothScreens2D();

	// Reordena la VRAM de los sprites (Evita la corrupcion aleatoria por fragmentacion)
	NF_VramSpriteGfxDefrag(1);


	/////////////////////////////
	// Crea de nuevo el puzzle //
	/////////////////////////////

	// Inicializa las estructuras de control del puzzle
	InitPuzzle01Data();

	// Crea los fondos
	NF_Copy16bitsBuffer(1, 0, 0);	// Pantalla tactil

	// Transfiere los graficos y paletas a la VRAM
	for (n = 0; n < 13; n ++) {
		// Transfiere los sprites de la pantalla tactil... (0 - 12)
		NF_VramSpriteGfx(1, n, n, false);
		NF_VramSpritePal(1, n, n);
	}

	// Crea los Sprites de la pantalla (Cada pantalla del 10 al 21) e inicializa el resto de variables
	for (n = 0; n < 12; n ++) {
		// Crea los sprites
		NF_CreateSprite(1, (n + PUZZLE01_SPRITE), PUZZLE01SPR[n].gfx, PUZZLE01SPR[n].pal, (PUZZLE01SPR[n].x - 4), (PUZZLE01SPR[n].y - 4));
	}

	// Cambia el grafico para la ficha mobil
	UpdateSprite(1, (PUZZLE01_SPRITE + 11), 0, 0);

	// Actualiza todas las paletas de la pantalla 0 (Todos a COLOR)
	for (n = 0; n < 13; n ++) {
		NF_SpriteUpdatePalette(0, n);
	}

	// Actualiza el contenido de la pantalla
	UpdateBothScreens2D();

	// FadeOut desde blanco
	for (n = 16; n >= 0; n --) {
		setBrightness(2, n);
		swiWaitForVBlank();
	}

}



// Funcion LoadPuzzle01Files();
void LoadPuzzle01Files(u8 puzzle) {

	// Variables
	u8 n = 0;
	u8 m = 0;
	u8 id = 0;
	char file[128];
	char lang[4];
	u8 stage = (puzzle + 1);


	// Carga los fondos comunes a todos los puzzles
	NF_Load16bitsBg("puzz01/bg/p01bg03sub", 0);
	sprintf(file, "puzz01/bg/p01bg03sub%02d", stage);
	NF_Load16bitsBg(file, 1);
	NF_LoadTiledBg("puzz01/bg/p01bg03main", "bg3", 256, 256);
	NF_LoadTiledBg("puzz01/bg/p01bg02main", "bg2", 256, 512);


	// Carga los Sprites de el puzzle seleccionado
	for (n = 0; n < 13; n ++) {
		// Carga los sprites de la pantalla tactil... (0 - 12)
		sprintf(file, "puzz01/sprite/stg%02d/p01s%02da%02d", stage, stage, n);
		NF_LoadSpriteGfx(file, n, 64, 64);
		NF_LoadSpritePal(file, n);
		// Carga los sprites de la pantalla normal... (20 - 32)
		sprintf(file, "puzz01/sprite/stg%02d/p01s%02db%02d", stage, stage, n);
		NF_LoadSpriteGfx(file, (n + 20), 64, 64);
		NF_LoadSpritePal(file, (n + 20));
	}


	// Carga los Sprites del menu
	// Seleccion de idioma
	switch (GAMEOPTIONS[0].language) {
		case 0:		// Español
			sprintf(lang, "es");
			break;
		case 1:		// Ingles
			sprintf(lang, "en");
			break;
		default:
			break;
	}
	// Carga los botones (13 - 18)(13)
	id = 13;
	for (n = 1; n <= 3; n ++) {
		for (m = 1; m <= 2; m ++) {
			sprintf(file, "puzz01/sprite/common/p01mb%d%d_%s", n, m, lang);
			NF_LoadSpriteGfx(file, id, 32, 64);
			id ++;
		}
	}
	NF_LoadSpritePal("puzz01/sprite/common/p01menu", 13);


	// Carga las fuentes
	if (GAMEOPTIONS[0].lefthanded) {
		n = 2;
	} else {
		n = 1;
	}
	NF_LoadTextFont16("font/8x16", "text", 256, 512, n);


	// Si es para zurdos...
	if (GAMEOPTIONS[0].lefthanded) {
		// Rota los fondos
		Rotate16bitsImage(0);
		Rotate16bitsImage(1);
		RotateTiledBg("bg3");
		RotateTiledBg("bg2");
		// Rota los sprites de las fichas
		for (n = 0; n < 13; n ++) {
			RotateSprite(n);
			RotateSprite((n + 20));
		}
		// Rota los sprites de los botones del menu
		for (n = 13; n <= 18; n ++) {
			RotateSprite(n);
		}
	}

}



// Funcion CreatePuzzle01();
void CreatePuzzle01(void) {

	// Variables
	u8 n = 0;
	u8 id = 0;

	// Inicializa las estructuras de control del puzzle
	InitPuzzle01Data();

	// Crea los fondos
	NF_CreateTiledBg(0, 3, "bg3");	// Pantalla normal
	NF_CreateTiledBg(0, 2, "bg2");	// Pantalla normal
	NF_Copy16bitsBuffer(1, 0, 0);	// Pantalla tactil

	// Crea la capa de texto
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTextLayer16(0, 1, 2, "text");
	} else {
		NF_CreateTextLayer16(0, 1, 1, "text");
	}
	NF_DefineTextColor(0, 1, 1, 31, 31, 31);
	NF_SetTextColor(0, 1, 1);

	// Habilita el alpha
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
	REG_BLDALPHA = 0x1F | (0x08 << 8);

	// Ajusta los fondos si es zurdo
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(0, 3, 0, 64);
		NF_ScrollBg(0, 2, 0, (320 - 192));
		NF_ScrollBg(0, 1, 0, (320 - 192));
	} else {
		NF_ScrollBg(0, 2, 0, 192);
		NF_ScrollBg(0, 1, 0, 192);
	}

	// Transfiere los graficos y paletas a la VRAM
	for (n = 0; n < 13; n ++) {
		// Transfiere los sprites de la pantalla tactil... (0 - 12)
		NF_VramSpriteGfx(1, n, n, false);
		NF_VramSpritePal(1, n, n);
		// Transfiere los sprites de la pantalla normal... (20 - 32)
		NF_VramSpriteGfx(0, (n + 20), n, false);
		NF_VramSpritePal(0, (n + 20), n);
	}

	// Crea los Sprites de la pantalla (Cada pantalla del 10 al 21) e inicializa el resto de variables
	for (id = 0; id < 12; id ++) {
		// Crea los sprites
		NF_CreateSprite(0, (id + PUZZLE01_SPRITE), PUZZLE01SPR[id].gfx, PUZZLE01SPR[id].pal, (PUZZLE01SPR[id].x - 4), (PUZZLE01SPR[id].y - 4));
		NF_SpriteLayer(0, (id + PUZZLE01_SPRITE), 3);
		NF_CreateSprite(1, (id + PUZZLE01_SPRITE), PUZZLE01SPR[id].gfx, PUZZLE01SPR[id].pal, (PUZZLE01SPR[id].x - 4), (PUZZLE01SPR[id].y - 4));
	}

	// Cambia el grafico para la ficha mobil
	UpdateSprite(1, (PUZZLE01_SPRITE + 11), 0, 0);

	// Actualiza el contenido de la pantalla
	UpdateBothScreens2D();

}



// Funcion InitPuzzle01Data();
void InitPuzzle01Data(void) {

	// Variables
	u8 n = 0;
	u8 m = 0;
	u8 id = 0;
	s16 x = 0;
	s16 y = 0;

	// Inicializa las estructuras de datos de los Sprites (Gfx del 1 al 12)
	id = 0;
	if (GAMEOPTIONS[0].lefthanded) {
		x = 16;
		for (n = 0; n < 4; n ++) {
			y = (180 - 56);
			for (m = 0; m < 3; m ++) {
				PUZZLE01SPR[id].x = x;
				PUZZLE01SPR[id].y = y;
				PUZZLE01SPR[id].gfx = (id + 1);
				PUZZLE01SPR[id].pal = (id + 1);
				y -= 56;
				id ++;
			}
			x += 56;
		}
	} else {
		x = (240 - 56);
		for (n = 0; n < 4; n ++) {
			y = 12;
			for (m = 0; m < 3; m ++) {
				PUZZLE01SPR[id].x = x;
				PUZZLE01SPR[id].y = y;
				PUZZLE01SPR[id].gfx = (id + 1);
				PUZZLE01SPR[id].pal = (id + 1);
				y += 56;
				id ++;
			}
			x -= 56;
		}
	}

	// Inicializa la matriz de control del puzzle
	id = 0;
	for (y = 0; y < 4; y ++) {
		for (x = 0; x < 3; x ++) {
			PUZZLE01MOV.id[x][y] = id;
			PUZZLE01SPR[id].gx = x;
			PUZZLE01SPR[id].gy = y;
			PUZZLE01SPR[id].bn = false;
			id ++;
		}
	}
	PUZZLE01MOV.count = 0;
	PUZZLE01MOV.move = 0;
	PUZZLE01MOV.target = 255;

}



// Funcion GetPuzzle01SpriteId();
u8 GetPuzzle01SpriteId(void) {

	// Variables
	u8 id = 255;
	u8 n = 0;

	// Obten la ID si se toca una ficha
	for (n = 0; n < 12; n ++) {
		if (
			(TOUCHPAD.pos_x >= PUZZLE01SPR[n].x)
			&&
			(TOUCHPAD.pos_x <= (PUZZLE01SPR[n].x + 56))
			&&
			(TOUCHPAD.pos_y >= PUZZLE01SPR[n].y)
			&&
			(TOUCHPAD.pos_y <= (PUZZLE01SPR[n].y + 56))
			) {
			id = n;
		}
	}

	// Devuelve el valor
	return id;

}



// Funcion GetPuzzle01Move();
u8 GetPuzzle01Move(u8 id) {

	// Variables
	u8 move = 0;
	u8 ax = 0;		// Destino del sprite que se mueve
	u8 ay = 0;
	u8 bx = 0;		// Destino del sprite vacio (id 11)
	u8 by = 0;
	
	// Guarda la posicion actual del ID
	bx = PUZZLE01SPR[id].gx;
	by = PUZZLE01SPR[id].gy;

	// Y el desplazamiento por defecto
	PUZZLE01MOV.add_x = 0;
	PUZZLE01MOV.add_y = 0;

	// Verifica si puedes moverte arriba... (1)
	if (PUZZLE01SPR[id].gy > 0) {
		if (PUZZLE01MOV.id[PUZZLE01SPR[id].gx][(PUZZLE01SPR[id].gy - 1)] == 11) {
			move = 1;
			PUZZLE01MOV.add_x = PUZZLE01_SPEED;
			PUZZLE01MOV.add_y = 0;
			ax = PUZZLE01SPR[id].gx;
			ay = (PUZZLE01SPR[id].gy - 1);
		}
	}

	// Verifica si puedes moverte abajo... (2)
	if (PUZZLE01SPR[id].gy < 3) {
		if (PUZZLE01MOV.id[PUZZLE01SPR[id].gx][(PUZZLE01SPR[id].gy + 1)] == 11) {
			move = 2;
			PUZZLE01MOV.add_x = -PUZZLE01_SPEED;
			PUZZLE01MOV.add_y = 0;
			ax = PUZZLE01SPR[id].gx;
			ay = (PUZZLE01SPR[id].gy + 1);
		}
	}

	// Verifica si puedes moverte a la derecha... (3)
	if (PUZZLE01SPR[id].gx < 2) {
		if (PUZZLE01MOV.id[(PUZZLE01SPR[id].gx + 1)][PUZZLE01SPR[id].gy] == 11) {
			move = 3;
			PUZZLE01MOV.add_x = 0;
			PUZZLE01MOV.add_y = PUZZLE01_SPEED;
			ax = (PUZZLE01SPR[id].gx + 1);
			ay = PUZZLE01SPR[id].gy;
		}
	}

	// Verifica si puedes moverte a la izquierda... (4)
	if (PUZZLE01SPR[id].gx > 0) {
		if (PUZZLE01MOV.id[(PUZZLE01SPR[id].gx - 1)][PUZZLE01SPR[id].gy] == 11) {
			move = 4;
			PUZZLE01MOV.add_x = 0;
			PUZZLE01MOV.add_y = -PUZZLE01_SPEED;
			ax = (PUZZLE01SPR[id].gx - 1);
			ay = PUZZLE01SPR[id].gy;
		}
	}

	// Si hay movimiento, actualiza la parrilla
	if (move > 0) {
		// Ficha seleccionada
		PUZZLE01SPR[id].gx = ax;
		PUZZLE01SPR[id].gy = ay;
		PUZZLE01MOV.id[ax][ay] = id;
		// Ficha comodin
		PUZZLE01SPR[11].gx = bx;
		PUZZLE01SPR[11].gy = by;
		PUZZLE01MOV.id[bx][by] = 11;
		// Y por ultimo, la posicion de la ficha comodin
		PUZZLE01SPR[11].x = PUZZLE01SPR[id].x;
		PUZZLE01SPR[11].y = PUZZLE01SPR[id].y;
	}

	// Si es para zurdos...
	if (GAMEOPTIONS[0].lefthanded) {
		// Invierte el movimiento de las fichas
		PUZZLE01MOV.add_x = -PUZZLE01MOV.add_x;
		PUZZLE01MOV.add_y = -PUZZLE01MOV.add_y;
	}

	// Devuelve el resultado
	return move;

}


// Funcion Puzzle01CheckGoodPlace();
u8 Puzzle01CheckGoodPlace(void) {

	// Variables
	u8 x = 0;
	u8 y = 0;
	u8 id = 0;
	u8 good = 0;

	// Verifica la matriz del puzzle
	for (y = 0; y < 4; y ++) {
		for (x = 0; x < 3; x ++) {
			// La pieza esta en su sitio?
			if (id == PUZZLE01MOV.id[x][y]) {
				// Si esta en blanco y negro, pasala a color
				if (PUZZLE01SPR[id].bn) {
					NF_SpriteUpdatePalette(0, (id + 1));
					PUZZLE01SPR[id].bn = false;
				}
				// Y cuenta una buena
				good ++;
			} else {
				// Si no esta en su sitio, ponla en blanco y negro
				if (!PUZZLE01SPR[id].bn) {
					DesaturateSprite(0, (id + 1), 0);
					PUZZLE01SPR[id].bn = true;
				}
			}
			id ++;
		}
	}

	// Devuelve el numero de piezas puestas en su sitio
	return good;

}


// Funcion Puzzle01Results();
void Puzzle01Results(u8 puzzle, u32 time, u32 pmoves) {

	// Variables
	u16 points = 0;			// Puntos totales ganados
	u16 time_points = 0;	// Puntos ganados por tiempo
	u16 moves_points = 0;	// Puntos ganados por movimientos
	u16 record_points = 0;	// Puntos por record
	u16 mins = 0;			// Minutos
	u16 secs = 0;			// Segundos
	char text[24];			// Buffer de texto

	// Añade un contador a puzzles resueltos
	SAVEDATA[0].solved[puzzle] ++;

	// Maximo de tiempo
	if (time > 5999) time = 5999;
	// Maximo de movimientos
	if (pmoves > 999) pmoves = 999;

	// Calcula los minutos y segundos
	mins = (int)(time / 60);
	secs = (time - (mins * 60));


	//////////////////////////////
	// Calcula las puntuaciones //
	//////////////////////////////

	// Por tiempo
	if (time < 300) {	
		time_points = (300 - time);
	}

	// Por movimientos
	if (pmoves < 100) {
		moves_points = ((100 - pmoves) << 1);
	}

	// Registra los records
	if (time <= SAVEDATA[0].time[puzzle]) {
		SAVEDATA[0].time[puzzle] = time;
		record_points += 50;
	}
	if (pmoves <= SAVEDATA[0].moves[puzzle]) {
		SAVEDATA[0].moves[puzzle] = pmoves;
		record_points += 50;
	}

	// Total de puntos
	points = (100 + time_points + moves_points + record_points);

	// Registra la puntuacion
	SAVEDATA[0].points += points;
	SAVEDATA[0].credits += points;
	SAVEDATA[0].score[puzzle] += points;

	// Guardalo los datos
	WriteSavegame();

	// Genera el report
	NF_ClearTextLayer16(0, 1);

	switch (GAMEOPTIONS[0].language) {

		case 0:		// Castellano
			sprintf(text, "Tiempo: %02d:%02d", mins, secs);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Pasos: %03d", pmoves);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Puntuacion");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Tiempo:  %03d", time_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Pasos:   %03d", moves_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 12, text);
			break;

		case 1:		// Ingles
			sprintf(text, "Time: %02d:%02d", mins, secs);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Moves: %03d", pmoves);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Score");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Time:    %03d", time_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Moves:   %03d", moves_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(0, 1, (12 - (strlen(text) >> 1)), 12, text);
			break;

		default:
			break;

	}

	// Actualiza las capas de texto
	NF_UpdateTextLayers();

}
