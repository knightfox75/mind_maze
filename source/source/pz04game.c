
// Mind Maze - Funciones del Puzzle 4 - Cubos (Juego)
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
#include <n3_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion KernelPuzzle04()
void KernelPuzzle04(void) {

	// Reinicialo todo
	ResetAll();

	// Variables
	bool loop = true;		// Loop
	u8 thread = 0;			// Hilo de Ejecucion
	u8 n = 0;				// Uso general


	//////////////////////
	//					//
	//  Carga Archivos	//
	//					//
	//////////////////////

	// Pantalla "Cargando..."
	LoadingIn();
	// Carga los archivos
	LoadPuzzle04Files();
	// Carga la musica
	LoadBgm(MOD_P04STG01);
	// Oculta la pantalla "Cargando..."
	LoadingOut();


	///////////////////////
	//				     //
	//	Crea el Puzzle	 //
	//					 //
	///////////////////////


	// Habilita el 3D en la pantalla tactil
	N3_Set3D(1, 0);
	N3_Init3dEngine();

	// Habilita los fondos tileados
	NF_InitTiledBgSys(0);

	// Y habilita la capa de dibujado para el 3D
	NF_ShowBg(0, 0);

	// Crea el puzzle
	CreatePuzzle04();

	// Reproduce la musica
	PlayBgm();


	///////////////////////
	//				     //
	//	Juega al Puzzle	 //
	//					 //
	///////////////////////

	// Repite mientras no pidas otro puzzle o salir
	while (loop) {
	
		// Juega al puzzle
		switch (thread) {

			// Fade-in
			case 0:
				Puzzle04FadeIn();
				thread = 1;
				break;

			// Mezcla el puzzle
			case 1:
				if (Puzzle04Mix()) {
					// Si se ha mezclado con exito, a jugar
					thread = 2;
				} else {
					// Si se ha cancelado, sal sin registrar nada
					thread = 3;
				}
				break;

			// Juega al puzzle
			case 2:
				// Contabiliza una jugada mas
				for (n = 0; n < 4; n ++) {
					SAVEDATA[3].played[n] ++;
				}
				// Ejecuta el nucleo de juego
				if (PlayPuzzle04()) {
					// Si se ha completado con exito, hay record?
					if (Puzzle04CheckRecord()) {
						// Si hay record, actualiza los marcadores
						Puzzle04WriteReport();
					}
				}
				// Guarda los datos
				WriteSavegame();
				// Muestra el menu de fin de puzzle
				if (Puzzle04ExitMenu()) {
					// Repite el puzzle
					Puzzle04FadeOut(false);
					Puzzle04InitPuzzleData();
					Puzzle04WriteReport();
					Puzzle04RenderAll();
					Puzzle04Engine2D();
					thread = 0;
				} else {
					// Sal del puzzle
					thread = 3;
				}
				break;

			// Fade-out
			case 3:
				Puzzle04FadeOut(true);
				thread = 255;
				loop = false;
				break;

			// Por defecto, no hagas nada
			default:
				break;
		}

	}

}



// Puzzle04FadeIn();
void Puzzle04FadeIn(void) {

	// Variables
	bool loop = true;
	s32 fade = (16 << 8);

	// Bucle
	while (loop) {
		fade -= 64;
		if (fade <= 0) {
			fade = 0;
			loop = false;
		}
		// Ajusta el brillo
		setBrightness(3, (0 - (fade >> 8)));
		// Ejecuta los renders 3D necesarios
		Puzzle04RenderAll();
		// Ejecuta los renders 2D necesarios
		Puzzle04Engine2D();
	}

}



// Funcion Puzzle04FadeOut();
void Puzzle04FadeOut(bool exit) {

	// Variables
	bool loop = true;
	s32 fade = 0;

	// Bucle
	while (loop) {
		fade += 64;
		if (fade >= (16 << 8)) {
			fade = (16 << 8);
			loop = false;
			if (exit) StopBgm();
		}
		// Ajusta el brillo
		setBrightness(3, (0 - (fade >> 8)));
		// Si es necesario, ajusta el volumen
		if (exit) BgmVolume(((4096 - fade) >> 2));
		// Ejecuta los renders 3D necesarios
		Puzzle04RenderAll();
		// Ejecuta los renders 2D necesarios
		Puzzle04Engine2D();
	}

}



// Funcion PlayPuzzle04();
bool PlayPuzzle04(void) {

	// Variables
	bool loop = true;			// Control de ejecucion
	u8 thread = 1;				// Hilo de ejecucion

	u8 cube = 255;				// Id del cubo tocado

	bool touch_held = false;	// Control tactil
	s8 touch_count = 0;

	s16 start_posx = 0;			// Control de la deteccion de movimiento
	s16 start_posy = 0;
	s16 move_x = 0;
	s16 move_y = 0;
	bool get_move = false;

	s32 move_count = 0;			// Cuenta los movimientos
	s8 move_dir = 0;			

	s8 x = 0;					// Calculo de bloque adyacentes
	s8 y = 0;
	s8 temp_x = 0;
	s8 temp_y = 0;
	s8 temp_id = 0;

	s8 tics = 0;				// Control de tiempo de juego

	s16 id = 0;					// Verificacion de puzzle resuelto
	bool done = false;

	bool result = true;			// Devuleve si el puzzle se ha resulto o no

	bool cancel = false;		// Flag de cancelacion

	s16 n = 0;					// Uso general


	// Ejecuta el puzzle mientras sea necesario
	while (loop) {

		// Lee el teclado y touchpad
		ReadKeypad();
		ReadTouchpad();

		// Control del stylus
		if (TOUCHPAD.held) {
			// Si ya estavas sobre la pantalla
			if (touch_held) {
			} else {	// Si no lo estavas...
				touch_held = true;
			}
			// Registra el tiempo
			if (touch_count < 10) touch_count ++;
		} else {
			// Resetea el flag
			touch_held = false;
		}

		// Hilos de ejecucion
		switch (thread) {

			//////////////////////////////////////////
			// Espera a que toques un bloque valido //
			//////////////////////////////////////////
			case 1:
				cube = 255;
				// Si es el primer toque, obten el ID del cubo
				if (touch_held && (touch_count == 1)) cube = Puzzle04GetCubeId();
				// Si devuelve una ID valida, salta a la animacio de sacar el cubo
				if (cube != 255) {
					P04WORLD.cube_z[cube] = 1;
					P04WORLD.cube_zoom = 0;
					thread = 2;
				}
				// Si se ha marcado para cancelar, hazlo
				if (cancel) {
					loop = false;
					thread = 255;
				}
				break;

			///////////////////////////
			// Extrae el cubo tocado //
			///////////////////////////
			case 2:
				// Zoom del cubo marcado
				if (Puzzle04ZoomInCube()) {
					// Cuando termines el zoom, siguiente thread
					move_count = 0;
					thread = 3;
				}
				break;

			///////////////////////////////////////
			// Registra el movimiento a realizar //
			///////////////////////////////////////
			case 3:
				// Registra la posicion inicial y flag de movimiento
				if (touch_held) {
					if (touch_count == 1) {
						get_move = true;
						start_posx = TOUCHPAD.pos_x;
						start_posy = TOUCHPAD.pos_y;
						move_x = 0;
						move_y = 0;
					}
				} else {
					// Si o te has desplazado, indica que se deven de recolocar los bloques
					if ((move_x >= -P04_CUBE_DEADZONE) && (move_x <= P04_CUBE_DEADZONE) && (move_y >= -P04_CUBE_DEADZONE) && (move_y <= P04_CUBE_DEADZONE) && (touch_count >= P04_CUBE_MINTIME) && get_move) {
						P04WORLD.cube_zoom = 0;
						thread = 5;
					}
					get_move = false;
				}
				// Si es necesario, detecta el movimiento a ejecutar
				if (get_move) {
					move_x = (start_posx - TOUCHPAD.pos_x);
					move_y = (start_posy - TOUCHPAD.pos_y);
					// Si lo detectas, procede a ejecutarlo, segun seas diestor o zurdo
					if ((move_y <= -P04_CUBE_MOVE) || (move_y >= P04_CUBE_MOVE)) {
						if (GAMEOPTIONS[0].lefthanded) {
							if (move_y > 0) {
								P04WORLD.cube_rot[cube] = 1;
							} else {
								P04WORLD.cube_rot[cube] = -1;
							}
						} else {
							if (move_y > 0) {
								P04WORLD.cube_rot[cube] = -1;
							} else {
								P04WORLD.cube_rot[cube] = 1;
							}
						}
						// Registra el movimiento y reinicia las variables necesarias
						move_count += P04WORLD.cube_rot[cube];
						if (move_count == -4) move_count = 0;
						if (move_count == 4) move_count = 0;
						get_move = false;
						move_x = 0;
						move_y = 0;
						P04WORLD.cube_rotation = 0;
						thread = 4;
					}
				}
				break;

			////////////////////////////////////////
			// Rota el cubo a derecha o izquierda //
			////////////////////////////////////////
			case 4:
				// Rota el cubo
				if (Puzzle04RotateCube()) {
					// Si has terminado, salta al thread correspondiente
					thread = 3;
				}
				break;


			////////////////////////////
			// Inserta el cubo tocado //
			////////////////////////////
			case 5:
				// Zoom del cubo marcado
				if (Puzzle04ZoomOutCube()) {
					// Cuando termines el zoom, siguiente thread
					if (move_count != 0) {
						thread = 6;		// Si se ha rotado, toca rotar los adyacentes a la inversa
					} else {	
						thread = 1;		// Si no se ha rotado, a la espera de rotar un cubo
					}
				}
				break;

			////////////////////////////////////////////
			// Calcula los cubos a girar a la inversa //
			////////////////////////////////////////////
			case 6:
				// Calcula el cubo actual
				y = (int)(cube / 3);
				x = (cube - (y * 3));
				// Y el movimiento a realizar
				if (move_count > 0) {
					move_dir = -1;
				} else {
					move_dir = 1;
				}
				// Marca para mover, si es posible, el adyacente superior
				temp_y = (y - 1);
				if (temp_y >= 0) {
					temp_id = ((temp_y * 3) + x);
					P04WORLD.cube_rot[temp_id] = move_dir;
					P04WORLD.cube_z[temp_id] = 1;
				}
				// Marca para mover, si es posible, el adyacente inferior
				temp_y = (y + 1);
				if (temp_y <= 3) {
					temp_id = ((temp_y * 3) + x);
					P04WORLD.cube_rot[temp_id] = move_dir;
					P04WORLD.cube_z[temp_id] = 1;
				}
				// Marca para mover, si es posible, el adyacente izquierdo
				temp_x = (x - 1);
				if (temp_x >= 0) {
					temp_id = ((y * 3) + temp_x);
					P04WORLD.cube_rot[temp_id] = move_dir;
					P04WORLD.cube_z[temp_id] = 1;
				}
				// Marca para mover, si es posible, el adyacente derecho
				temp_x = (x + 1);
				if (temp_x <= 2) {
					temp_id = ((y * 3) + temp_x);
					P04WORLD.cube_rot[temp_id] = move_dir;
					P04WORLD.cube_z[temp_id] = 1;
				}
				// Salta al thread de espera
				temp_id = 0;
				thread = 7;
				// Si se ha marcado para cancelar, hazlo
				if (cancel) {
					loop = false;
					thread = 255;
				}
				break;

			/////////////////////////////////////////////////
			// Espera unos instantes antes de la animacion //
			/////////////////////////////////////////////////
			case 7:
				temp_id ++;
				if (temp_id >= 15) {
					// Salta al thread de Zoom IN
					P04WORLD.cube_zoom = 0;
					thread = 8;
				}
				break;

			/////////////////////////////////
			// Extrae los cubos a invertir //
			/////////////////////////////////
			case 8:
				// Zoom del cubos marcados
				if (Puzzle04ZoomInCube()) {
					// Cuando termines el zoom, siguiente thread
					P04WORLD.cube_rotation = 0;
					thread = 9;
				}
				break;

			//////////////////////////////////////////
			// Rota los cubos a derecha o izquierda //
			//////////////////////////////////////////
			case 9:
				// Rota los cubos
				if (Puzzle04RotateCube()) {
					// Si has terminado, salta al thread correspondiente
					P04WORLD.cube_zoom = 0;
					thread = 10;
				}
				break;

			//////////////////////////////////
			// Inserta los cubos invertidos //
			//////////////////////////////////
			case 10:
				// Zoom del cubos marcados
				if (Puzzle04ZoomOutCube()) {
					// Cuando termines el zoom, thread de verificacion
					thread = 11;
				}
				break;

			/////////////////////////////////////////
			// Verifica si se ha resulto el puzzle //
			/////////////////////////////////////////
			case 11:
				// Cuenta un movimiento
				P04WORLD.moves ++;
				if (P04WORLD.moves > 999) P04WORLD.moves = 999;
				// Verifica si se ha resuleto (todas las caras iguales)
				done = true;
				id = P04WORLD.cube_angle[0];
				for (n = 1; n < 12; n ++) {
					if (id != P04WORLD.cube_angle[n]) done = false;
				}
				// Si se ha resuelto
				if (done) {
					result = true;
					loop = false;
				} else {
					// Si no, continua jugando
					thread = 1;
				}
				break;

			////////////////////////////////
			// Por defecto, no hagas nada //
			////////////////////////////////
			default:
				break;

		}

		// Reinicia las variables del control del touchpad
		if (!touch_held) touch_count = 0;

		// Control de tiempo de juego
		tics ++;
		if (tics > 59) {
			tics = 0;
			P04WORLD.time ++;
			if (P04WORLD.time > 5999) P04WORLD.time = 5999;
		}

		// Si has pulsado la tecla de cancelar
		if (loop && AbortKeys()) {
			cancel = true;
			result = false;
		}

		// Ejecuta los renders 3D necesarios
		Puzzle04RenderAll();

		// Ejecuta los renders 2D necesarios
		Puzzle04Engine2D();

	}

	// Devuelve el resultado
	return result;

}



// Funcion LoadPuzzle04Files();
void LoadPuzzle04Files(void) {

	// Variables locales
	s32 tex_id = P04_CUBE_TEXTURE;		// Id de textura en RAM
	s32 pal_id = P04_CUBE_PAL;			// Id de paleta en RAM
	s32 cube = 0;						// nº de cubo
	s32 face = 0;						// nº de cara
	char path[32];						// Almacen de path y archivo
	char file[128];
	char lang[4];
	s32 n = 0;							// Uso general

	// Define el path base
	sprintf(path, "puzz04/tex/stg01");

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

	// Carga las texturas de las caras comunes de los cubos
	sprintf(file, "%s/p04s01c000", path);
	N3_LoadTexture(file, tex_id, 32, 32);
	tex_id ++;
	sprintf(file, "%s/p04s01c0", path);
	N3_LoadPal(file, pal_id);
	pal_id ++;

	// Carga las texturas de cada cara del puzzle
	for (cube = 1; cube <= 12; cube ++) {
		for (face = 1; face <= 4; face ++) {
			sprintf(file, "%s/p04s01c%d%02d", path, face, cube);
			N3_LoadTexture(file, tex_id, 32, 32);
			tex_id ++;
		}
	}

	// Carga la paletas
	for (n = 1; n <= 4; n ++) {
		sprintf(file, "%s/p04s01c%d", path, n);
		N3_LoadPal(file, pal_id);
		pal_id ++;
	}

	// Carga la textura de los cubos del fondo
	for (n = 1; n <= 3; n ++) {
		sprintf(file, "%s/p04s01bg%02d", path, n);
		N3_LoadTexture(file, tex_id, 32, 32);
		tex_id ++;
	}
	sprintf(file, "%s/p04s01bg", path);
	N3_LoadPal(file, pal_id);
	pal_id ++;

	// Carga la textura de los botones del menu
	for (n = 0; n < 2; n ++) {
		sprintf(file, "puzz04/tex/common/p04mb%01d_%s", n, lang);
		N3_LoadTexture(file, tex_id, 128, 32);
		tex_id ++;
	}
	N3_LoadPal("puzz04/tex/common/p04mb", pal_id);
	pal_id ++;

	// Carga los fondos 2D
	NF_LoadTiledBg("puzz04/bg/stg01/p04bg3", "bg3", 512, 512);
	NF_LoadTiledBg("puzz04/bg/stg01/p04bg2", "bg2", 256, 256);
	if (GAMEOPTIONS[0].lefthanded) {
		RotateTiledBg("bg3");
		RotateTiledBg("bg2");
	}

	// Carga los sprites 2D (GFX)
	for (n = 1; n <= 12; n ++) {	// Miniaturas
		sprintf(file, "puzz04/sprite/stg01/p04s01m%02d", n);
		NF_LoadSpriteGfx(file, n, 32, 32);
	}
	if (GAMEOPTIONS[0].lefthanded) {
		for (n = 1; n <= 12; n ++) {
			RotateSprite(n);
		}
	}
	for (n = 21; n <= 23; n ++) {		// Boton de record
		sprintf(file, "puzz04/sprite/common/p04record%01d", (n - 20));
		NF_LoadSpriteGfx(file, n, 16, 16);
	}

	// Carga los sprites 2D (PAL)
	for (n = 1; n <= 4; n ++) {		// Miniaturas
		sprintf(file, "puzz04/sprite/stg01/p04s01pal%02d", n);
		NF_LoadSpritePal(file, n);
	}
	NF_LoadSpritePal("puzz04/sprite/common/p04record", 5);		// Boton de record


	// Carga las fuentes
	if (GAMEOPTIONS[0].lefthanded) {
		n = 2;
	} else {
		n = 1;
	}
	NF_LoadTextFont("font/8x8", "text", 256, 256, n);

}



// Funcion CreatePuzzle04();
void CreatePuzzle04(void) {

	// Variables locales
	s32 tex_id = P04_CUBE_TEXTURE;		// Id de textura en RAM
	s32 pal_id = P04_CUBE_PAL;			// Id de paleta en RAM
	s32 cube = 0;						// nº de cubo
	s32 face = 0;						// nº de cara
	s16 x = 0;							// Uso general
	s16 y = 0;
	s16 n = 0;
	s16 row = 0;
	s16 col = 0;

	// Inicializa el random
	srand(time(NULL));

	// Crea los fondos
	NF_CreateTiledBg(0, 3, "bg3");
	NF_CreateTiledBg(1, 3, "bg3");
	NF_CreateTiledBg(1, 2, "bg2");

	// Crea la capa de texto
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTextLayer(1, 1, 2, "text");
	} else {
		NF_CreateTextLayer(1, 1, 1, "text");
	}
	NF_DefineTextColor(1, 1, 1, 31, 31, 31);
	NF_SetTextColor(1, 1, 1);

	// Correccion de posicion de los fondos
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(1, 2, 0, 64);
		NF_ScrollBg(1, 1, 4, 64);
	} else {
		NF_ScrollBg(1, 1, 4, 0);
	}

	// Carga los sprites 2D en VRAM con sus paletas
	for (n = 1; n <= 12; n ++) {
		NF_VramSpriteGfx(1, n, n, false);	// Miniaturas
	}
	for (n = 21; n <= 23; n ++) {
		NF_VramSpriteGfx(1, n, n, true);	// Record
	}
	for (n = 1; n <= 5; n ++) {
		NF_VramSpritePal(1, n, n);			// Paletas
	}

	// Crea los sprites
	n = 1;
	if (GAMEOPTIONS[0].lefthanded) {
		// Zurdos
		// Miniaturas
		x = 20;
		for (row = 1; row <= 4; row ++) {
			y = 136;
			for (col = 1; col <= 3; col ++) {
				NF_CreateSprite(1, n, n, row, x, y);
				y -= 32;
				n ++;
			}
			x += 56;
		}
		// Record
		x = 28;
		y = 48;
		for (n = 21; n <= 23; n ++) {
			NF_CreateSprite(1, n, n, 5, x, y);
			NF_HflipSprite(1, n, true);
			NF_VflipSprite(1, n, true);
			NF_ShowSprite(1, n, false);
			y -= 16;
		}
	} else {
		// Diestros
		// Miniaturas
		x = 204;
		for (row = 1; row <= 4; row ++) {
			y = 24;
			for (col = 1; col <= 3; col ++) {
				NF_CreateSprite(1, n, n, row, x, y);
				y += 32;
				n ++;
			}
			x -= 56;
		}
		// Record
		x = 212;
		y = 128;
		for (n = 21; n <= 23; n ++) {
			NF_CreateSprite(1, n, n, 5, x, y);
			NF_ShowSprite(1, n, false);
			y += 16;
		}
	}

	// Carga las texturas de cada cara del puzzle a la VRAM
	N3_VramTexture(tex_id, tex_id, false);
	tex_id ++;
	for (cube = 1; cube <= 12; cube ++) {
		for (face = 1; face <= 4; face ++) {
			N3_VramTexture(tex_id, tex_id, false);
			tex_id ++;
		}
	}

	// Carga la paletas de las caras
	for (n = 0; n <= 4; n ++) {
		N3_VramPalette(pal_id, pal_id);
		pal_id ++;
	}

	// Transfiere la textura y paleta del fondo
	for (n = 1; n <= 3; n ++) {
		N3_VramTexture(tex_id, tex_id, false);
		tex_id ++;
	}
	N3_VramPalette(pal_id, pal_id);
	pal_id ++;

	// Transfiere la textura y paleta del menu
	for (n = 0; n < 2; n ++) {
		N3_VramTexture(tex_id, tex_id, false);
		tex_id ++;
	}
	N3_VramPalette(pal_id, pal_id);
	pal_id ++;

	// Habilita el alpha
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG0 | (BLEND_DST_BG1 + BLEND_DST_BG2 + BLEND_DST_BG3);
	REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
	REG_BLDALPHA_SUB = 0x1F | (0x05 << 8);

	// Pantalla en negro
	setBrightness(3, -16);

	// Inicializa los datos del puzzle
	Puzzle04InitPuzzleData();

	// Textos
	Puzzle04WriteReport();

	// Ejecuta los renders 3D necesarios
	Puzzle04RenderAll();

	// Ejecuta los renders 2D necesarios
	Puzzle04Engine2D();

}



// Funcion Puzzle04InitPuzzleData();
void Puzzle04InitPuzzleData(void) {

	// Variables
	s32 cube = 0;						// nº de cubo
	s16 x = 0;							// Uso general
	s16 y = 0;
	s16 n = 0;

	// Calcula las distancias
	P04WORLD.origin = 0; 
	n = P04_CUBE_SIZE;
	P04WORLD.moveto = sqrt32(((n * n) + (n * n)));
	P04WORLD.moveto = (P04WORLD.moveto >> 1);

	// Inicializa los cubos
	cube = 0;
	n = (int)(rand() % 4);
	n = (n << 7);
	for (y = 0; y < 4; y ++) {
		for (x = 0; x < 3; x ++) {
			P04WORLD.cube_posx[cube] = (P04_CUBE_POSX - (y * P04_CUBE_SIZE));
			P04WORLD.cube_posy[cube] = (P04_CUBE_POSY - (x * P04_CUBE_SIZE));
			P04WORLD.cube_posz[cube] = P04_CUBE_POSZ;
			P04WORLD.cube_angle[cube] = n;
			P04WORLD.cube_rot[cube] = 0;
			P04WORLD.cube_z[cube] = 0;
			cube ++;
		}
	}

	// Inicializa el angulo de los cubos del fondo
	for (n = 0; n < P04_BG_CUBENUM; n ++) {
		P04WORLD.bg_angle[n] = (int)(rand() % 512);
	}

	// Inicializa la camara
	P04WORLD.eye_x = (128 << 4);
	P04WORLD.eye_y = (96 << 4);
	P04WORLD.eye_z = (128 << 4);
	P04WORLD.lookat_x = (128 << 4);
	P04WORLD.lookat_y = (96 << 4);
	P04WORLD.lookat_z = 0;
	if (GAMEOPTIONS[0].lefthanded) {
		P04WORLD.cam_up = -(128 << 4);
	} else {
		P04WORLD.cam_up = (128 << 4);
	}

	// Inicializa la posicion del fondo 2D
	P04WORLD.bgpos = 0;

	// Inicializa el control de tiempo y movimientos
	P04WORLD.moves = 0;
	P04WORLD.time = 0;

	// Inicializa el marcador de record
	P04WORLD.record = false;
	P04WORLD.r_frame = 0;
	P04WORLD.r_timer = 0;

}



// Funcion Puzzle04RenderAll();
void Puzzle04RenderAll(void) {

	// Variables
	s32 n = 0;

	// Fondo 3D
	Puzzle04Draw3dBg();

	// Cubos del tablero
	for (n = 0; n < 12; n ++) {
		Puzzle04DrawCube(n);
	}

	// Manda la escena 3D a la pantalla
	glFlush(0);

}



// Funcion Puzzle04DrawCube();
void Puzzle04DrawCube(u8 id) {

	// Variables
	typedef struct {
		s32 vertex_x[8];
		s32 vertex_y[8];
		s32 vertex_z[8];
	} cube_info;
	cube_info cube;

	s16 tex_id = (P04_CUBE_TEXTURE + 1 + (id << 2));		// Id de textura
	s16 pal_id = (P04_CUBE_PAL + 1);						// Id de paleta

	s32	cx = (P04WORLD.cube_posx[id] - (P04_CUBE_SIZE >> 1));	// Centro del cubo
	s32	cy = (P04WORLD.cube_posy[id] - (P04_CUBE_SIZE >> 1));
	s32	cz = (P04WORLD.cube_posz[id] - (P04_CUBE_SIZE >> 1));

	// Calcula los vertices
	cube.vertex_x[0] = P04WORLD.cube_posx[id];
	cube.vertex_y[0] = P04WORLD.cube_posy[id];
	cube.vertex_z[0] = P04WORLD.cube_posz[id];

	cube.vertex_x[1] = (cube.vertex_x[0] - P04_CUBE_SIZE);
	cube.vertex_y[1] = cube.vertex_y[0];
	cube.vertex_z[1] = cube.vertex_z[0];

	cube.vertex_x[2] = cube.vertex_x[1];
	cube.vertex_y[2] = (cube.vertex_y[1] - P04_CUBE_SIZE);
	cube.vertex_z[2] = cube.vertex_z[1];

	cube.vertex_x[3] = (cube.vertex_x[2] + P04_CUBE_SIZE);
	cube.vertex_y[3] = cube.vertex_y[2];
	cube.vertex_z[3] = cube.vertex_z[2];

	cube.vertex_x[4] = cube.vertex_x[0];
	cube.vertex_y[4] = cube.vertex_y[0];
	cube.vertex_z[4] = (cube.vertex_z[0] - P04_CUBE_SIZE);

	cube.vertex_x[5] = cube.vertex_x[1];
	cube.vertex_y[5] = cube.vertex_y[1];
	cube.vertex_z[5] = (cube.vertex_z[1] - P04_CUBE_SIZE);

	cube.vertex_x[6] = cube.vertex_x[2];
	cube.vertex_y[6] = cube.vertex_y[2];
	cube.vertex_z[6] = (cube.vertex_z[2] - P04_CUBE_SIZE);

	cube.vertex_x[7] = cube.vertex_x[3];
	cube.vertex_y[7] = cube.vertex_y[3];
	cube.vertex_z[7] = (cube.vertex_z[3] - P04_CUBE_SIZE);


	// Guarda la matriz original y posiciona la camara
	glPushMatrix();
	gluLookAtf32(	P04WORLD.eye_x, P04WORLD.eye_y, P04WORLD.eye_z,
					P04WORLD.lookat_x, P04WORLD.lookat_y, P04WORLD.lookat_z,
					0 , P04WORLD.cam_up, 0);

	// Calcula el angulo del dibujado
	glTranslatef32(cx, cy, cz);
	glRotateXi(P04WORLD.cube_angle[id] << 6);
	glTranslatef32(-cx, -cy, -cz);

	// Dibuja la cara frontal (1)
	N3_TexturedQuad(cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
					cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
					cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
					cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
					tex_id, pal_id);
	tex_id ++;
	pal_id ++;

	// Dibuja la cara derecha (2)
	N3_TexturedQuad(cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
					cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
					cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
					cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
					tex_id, pal_id);
	tex_id ++;
	pal_id ++;

	// Dibuja la cara trasera (3)
	N3_TexturedQuad(cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
					cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
					cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
					cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
					tex_id, pal_id);
	tex_id ++;
	pal_id ++;

	// Dibuja la cara izquierda (4)
	N3_TexturedQuad(cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
					cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
					cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
					cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
					tex_id, pal_id);
	tex_id ++;
	pal_id ++;

	// Prepara en alpha
	glPolyFmt(POLY_ALPHA(25) | POLY_ID(3) | POLY_CULL_NONE);

	// Dibuja la cara Superior
	N3_TexturedQuad(cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
					cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
					cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
					cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
					P04_CUBE_TEXTURE, P04_CUBE_PAL);

	// Prepara en alpha
	glPolyFmt(POLY_ALPHA(25) | POLY_ID(4) | POLY_CULL_NONE);

	// Dibuja la cara Inferior
	N3_TexturedQuad(cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
					cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
					cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
					cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
					P04_CUBE_TEXTURE, P04_CUBE_PAL);

	// Restaura el alpha
	glPolyFmt(POLY_ALPHA(31) | POLY_ID(2) | POLY_CULL_NONE);

	// Recupera la matriz original
	glPopMatrix(1);


}



// Funcion Puzzle04Draw3dBg();
void Puzzle04Draw3dBg(void) {

	// Variables
	s32 x = P04_BG_CUBE_X;		// Control de coordenadas
	s32 y = P04_BG_CUBE_Y;
	s32 z = P04_BG_CUBE_Z;
	s32 r = 0;				// Bucle
	s32 c = 0;
	s32 id = 0;
	s32 cx = 0;
	s32 cy = 0;
	s32 cz = 0;

	// Variables de los vertices
	typedef struct {
		s32 vertex_x[8];
		s32 vertex_y[8];
		s32 vertex_z[8];
	} cube_info;
	cube_info cube;

	// Dibuja los cubos del fondo
	for (r = 0; r < P04_BG_ROW; r ++) {
		for (c = 0; c < P04_BG_COL; c ++) {

			// Calcula los vertices
			cube.vertex_x[0] = x;
			cube.vertex_y[0] = y;
			cube.vertex_z[0] = z;

			cube.vertex_x[1] = (cube.vertex_x[0] - P04_BG_CUBESIZE);
			cube.vertex_y[1] = cube.vertex_y[0];
			cube.vertex_z[1] = cube.vertex_z[0];

			cube.vertex_x[2] = cube.vertex_x[1];
			cube.vertex_y[2] = (cube.vertex_y[1] - P04_BG_CUBESIZE);
			cube.vertex_z[2] = cube.vertex_z[1];

			cube.vertex_x[3] = (cube.vertex_x[2] + P04_BG_CUBESIZE);
			cube.vertex_y[3] = cube.vertex_y[2];
			cube.vertex_z[3] = cube.vertex_z[2];

			cube.vertex_x[4] = cube.vertex_x[0];
			cube.vertex_y[4] = cube.vertex_y[0];
			cube.vertex_z[4] = (cube.vertex_z[0] - P04_BG_CUBESIZE);

			cube.vertex_x[5] = cube.vertex_x[1];
			cube.vertex_y[5] = cube.vertex_y[1];
			cube.vertex_z[5] = (cube.vertex_z[1] - P04_BG_CUBESIZE);

			cube.vertex_x[6] = cube.vertex_x[2];
			cube.vertex_y[6] = cube.vertex_y[2];
			cube.vertex_z[6] = (cube.vertex_z[2] - P04_BG_CUBESIZE);

			cube.vertex_x[7] = cube.vertex_x[3];
			cube.vertex_y[7] = cube.vertex_y[3];
			cube.vertex_z[7] = (cube.vertex_z[3] - P04_BG_CUBESIZE);

			// Calcula el centro del cubo
			cx = (cube.vertex_x[0] - (P04_BG_CUBESIZE >> 1));	
			cy = (cube.vertex_y[0] - (P04_BG_CUBESIZE >> 1));
			cz = (cube.vertex_z[0] - (P04_BG_CUBESIZE >> 1));

			// Guarda la matriz original y posiciona la camara
			glPushMatrix();
			gluLookAtf32(	P04WORLD.eye_x, P04WORLD.eye_y, P04WORLD.eye_z,
							P04WORLD.lookat_x, P04WORLD.lookat_y, P04WORLD.lookat_z,
							0 , P04WORLD.cam_up, 0);

			// Calcula el angulo del dibujado
			glTranslatef32(cx, cy, cz);
			glRotateXi((P04WORLD.bg_angle[id] << 6));
			glRotateZi((P04WORLD.bg_angle[id] << 6));
			glTranslatef32(-cx, -cy, -cz);

			// Dibuja la cara frontal (1)
			N3_TexturedQuad(cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
							cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
							cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
							cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
							(P04_BG_TEXTURE + 1), P04_BG_PAL);

			// Dibuja la cara derecha (2)
			N3_TexturedQuad(cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
							cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
							cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
							cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
							P04_BG_TEXTURE, P04_BG_PAL);

			// Dibuja la cara trasera (3)
			N3_TexturedQuad(cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
							cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
							cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
							cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
							(P04_BG_TEXTURE + 2), P04_BG_PAL);

			// Dibuja la cara izquierda (4)
			N3_TexturedQuad(cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
							cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
							cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
							cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
							P04_BG_TEXTURE, P04_BG_PAL);

			// Dibuja la cara Superior
			N3_TexturedQuad(cube.vertex_x[4], cube.vertex_y[4], cube.vertex_z[4],
							cube.vertex_x[0], cube.vertex_y[0], cube.vertex_z[0],
							cube.vertex_x[3], cube.vertex_y[3], cube.vertex_z[3],
							cube.vertex_x[7], cube.vertex_y[7], cube.vertex_z[7],
							P04_BG_TEXTURE, P04_BG_PAL);

			// Dibuja la cara Inferior
			N3_TexturedQuad(cube.vertex_x[1], cube.vertex_y[1], cube.vertex_z[1],
							cube.vertex_x[5], cube.vertex_y[5], cube.vertex_z[5],
							cube.vertex_x[6], cube.vertex_y[6], cube.vertex_z[6],
							cube.vertex_x[2], cube.vertex_y[2], cube.vertex_z[2],
							P04_BG_TEXTURE, P04_BG_PAL);

			// Recupera la matriz original
			glPopMatrix(1);

			// Calcula la siguiente posicion
			y -= (P04_BG_CUBESIZE << 1);

			// Y la siguiente id
			id ++;

		}

		y = P04_BG_CUBE_Y;
		x -= (P04_BG_CUBESIZE << 1);

	}

	// Recalcula la animacion para el proximo frame
	for (id = 0; id < P04_BG_CUBENUM; id ++) {
		P04WORLD.bg_angle[id] ++;
		if (P04WORLD.bg_angle[id] > 511) P04WORLD.bg_angle[id] -= 512;
	}

}



// Funcion Puzzle04DrawMenu();
void Puzzle04DrawMenu(s32 x1, s32 y1, s32 x2, s32 y2) {

	// Variables
	s32 x[2];		// Posicion de los botones
	s32 y[2];
	u8 n = 0;		// Uso general

	// Control de vertices
	typedef struct {
		s32 vertex_x[4];
		s32 vertex_y[4];
		s32 vertex_z[4];
	} button_info;
	button_info button;

	// Inicializa las variables
	x[0] = N3_RealToV16(x1);
	x[1] = N3_RealToV16(x2);
	y[0] = N3_RealToV16(y1);
	y[1] = N3_RealToV16(y2);
	
	// Guarda la matriz actual
	glPushMatrix();

	// Posiciona la camara
	gluLookAtf32(	P04WORLD.eye_x, P04WORLD.eye_y, P04WORLD.eye_z,
					P04WORLD.lookat_x, P04WORLD.lookat_y, P04WORLD.lookat_z,
					0 , P04WORLD.cam_up, 0);

	// Dibuja los botones
	for (n = 0; n < 2; n ++) {
		// Calcula los vertices
		button.vertex_x[0] = x[n];
		button.vertex_y[0] = y[n];
		button.vertex_z[0] = (1 << 4);
		//
		button.vertex_x[1] = (button.vertex_x[0] - (32 << 4));
		button.vertex_y[1] = button.vertex_y[0];
		button.vertex_z[1] = button.vertex_z[0];
		//
		button.vertex_x[2] = button.vertex_x[1];
		button.vertex_y[2] = (button.vertex_y[1] - (128 << 4));
		button.vertex_z[2] = button.vertex_z[1];
		//
		button.vertex_x[3] = (button.vertex_x[2] + (32 << 4));
		button.vertex_y[3] = button.vertex_y[2];
		button.vertex_z[3] = button.vertex_z[2];
		// Dibuja el poligono
		N3_TexturedQuad(button.vertex_x[0], button.vertex_y[0], button.vertex_z[0],
						button.vertex_x[1], button.vertex_y[1], button.vertex_z[1],
						button.vertex_x[2], button.vertex_y[2], button.vertex_z[2],
						button.vertex_x[3], button.vertex_y[3], button.vertex_z[3],
						(P04_MENU_TEXTURE + n), P04_MENU_PAL);
	}

	// Recupera la matriz original
	glPopMatrix(1);

}



// Funcion Puzzle04Engine2D();
void Puzzle04Engine2D(void) {

	// Variables
	u8 n = 0;

	// Calcula el movimiento del fondo
	if (GAMEOPTIONS[0].lefthanded) {
		P04WORLD.bgpos -= 64;
		if (P04WORLD.bgpos < 0) P04WORLD.bgpos += (64 << 8);
	} else {
		P04WORLD.bgpos += 64;
		if (P04WORLD.bgpos > (64 << 8)) P04WORLD.bgpos -= (64 << 8);
	}

	// Gestiona el grafico de nuevo record
	if (P04WORLD.record) {
		// Fuerza que se muestre el boton
		for (n = 21; n <= 23; n ++) {
			NF_ShowSprite(1, n, true);
		}
		// Temporizador de animacion
		P04WORLD.r_timer ++;
		if (P04WORLD.r_timer > 5) {
			P04WORLD.r_timer = 0;
			P04WORLD.r_frame ++;
			if (P04WORLD.r_frame > 7) P04WORLD.r_frame = 0;
			for (n = 21; n <= 23; n ++) {
				NF_SpriteFrame(1, n, P04WORLD.r_frame);
			}
		}
	} else {
		// Fuerza que se oculte el boton
		for (n = 21; n <= 23; n ++) {
			NF_ShowSprite(1, n, false);
		}
	}

	// Actualiza el OAM de NFlib
	NF_SpriteOamSet(1);

	// Espera al sincronismo vertical
	swiWaitForVBlank();

	// Actualiza el OAM de libnds
	oamUpdate(&oamSub);

	// Posiciona los fondos 2D
	NF_ScrollBg(0, 3, (P04WORLD.bgpos >> 8), (P04WORLD.bgpos >> 8));
	NF_ScrollBg(1, 3, (P04WORLD.bgpos >> 8), (P04WORLD.bgpos >> 8));

}



// Funcion Puzzle04GetCubeId();
u8 Puzzle04GetCubeId(void) {

	// Variables
	u8 cube = 0;						// Id del cubo
	u8 result = 255;					// Resultado a devolver
	s16 x = 0;							// Bucle
	s16 y = 0;
	s16 start_x = 0;					// Inicio del panel
	s16 start_y = 0;
	s16 cube_x = 0;						// Coordenadas del cubo
	s16 cube_y = 0;
	s16 sty_x = TOUCHPAD.pos_x;			// Coordenadas del tocuhpad
	s16 sty_y = TOUCHPAD.pos_y;
	s16 size = 0;						// Tamaño del cubo
	s16 area = (P04_CUBE_SIZE >> 4);

	// Inicializa variables segun la orientacion
	if (GAMEOPTIONS[0].lefthanded) {
		start_x = P04_PANEL_POSX_L;
		start_y = P04_PANEL_POSY_L;
		size = -(P04_CUBE_SIZE >> 4);
	} else {
		start_x = P04_PANEL_POSX_R;
		start_y = P04_PANEL_POSY_R;
		size = (P04_CUBE_SIZE >> 4);
	}

	// Busca coincidencias
	for (y = 0; y < 4; y ++) {
		for (x = 0; x < 3; x ++) {
			// Calcula si el lapiz toca un cubo
			cube_x = (start_x - (y * size));
			cube_y = (start_y + (x * size));
			if (GAMEOPTIONS[0].lefthanded) {
				if ((sty_x >= cube_x) && (sty_x <= (cube_x + area)) && (sty_y >= (cube_y - area)) && (sty_y <= cube_y)) result = cube;
			} else {
				if ((sty_x >= (cube_x - area)) && (sty_x <= cube_x) && (sty_y >= cube_y) && (sty_y <= (cube_y + area))) result = cube;
			}
			cube ++;
		}
	}

	// Devuelve el resultado
	return result;

}



// Funcion Puzzle04RotateCube();
bool Puzzle04RotateCube(void) {

	// Variables
	bool result = false;
	u8 n = 0;

	// Rota los cubos necesarios
	for (n = 0; n < 12; n ++) {
		if (P04WORLD.cube_rot[n] != 0) {
			// Realiza los calculos
			P04WORLD.cube_angle[n] += (P04_CUBE_SPEED_ROT * P04WORLD.cube_rot[n]);
			// Verifica el limite de angulos
			if (P04WORLD.cube_angle[n] > 511) P04WORLD.cube_angle[n] -= 512;
			if (P04WORLD.cube_angle[n] < 0) P04WORLD.cube_angle[n] += 512;
		}
	}

	// Verifica si ya se ha completado la rotacion
	P04WORLD.cube_rotation += P04_CUBE_SPEED_ROT;
	if (P04WORLD.cube_rotation >= 128) {
		result = true;
	}

	// Devuelve el resultado
	return result;

}



// Funcion Puzzle04ZoomInCube();
bool Puzzle04ZoomInCube(void) {

	// Variables
	bool result = false;
	u8 n = 0;

	// Mueve los cubos necesarios
	for (n = 0; n < 12; n ++) {
		if (P04WORLD.cube_z[n] == 0) {
			P04WORLD.cube_posz[n] -= P04_CUBE_SPEED_Z;
		} else {
			P04WORLD.cube_posz[n] += P04_CUBE_SPEED_Z;
		}
	}
	P04WORLD.eye_z += (P04_CUBE_SPEED_Z >> 1);

	// Verifica si ya se ha completado el zoom
	P04WORLD.cube_zoom += P04_CUBE_SPEED_Z;
	if (P04WORLD.cube_zoom >= P04WORLD.moveto) {
		for (n = 0; n < 12; n ++) {
			if (P04WORLD.cube_z[n] == 0) {
				P04WORLD.cube_posz[n] = -P04WORLD.moveto;
			} else {
				P04WORLD.cube_posz[n] = P04WORLD.moveto;
			}
		}
		result = true;
	}

	// Devuelve el resultado
	return result;

}



// Funcion Puzzle04ZoomOutCube();
bool Puzzle04ZoomOutCube(void) {

	// Variables
	bool result = false;
	u8 n = 0;

	// Mueve los cubos necesarios
	for (n = 0; n < 12; n ++) {
		if (P04WORLD.cube_z[n] == 0) {
			P04WORLD.cube_posz[n] += P04_CUBE_SPEED_Z;
		} else {
			P04WORLD.cube_posz[n] -= P04_CUBE_SPEED_Z;
		}
	}
	P04WORLD.eye_z -= (P04_CUBE_SPEED_Z >> 1);

	// Verifica si ya se ha completado el zoom y reinicia parametros
	P04WORLD.cube_zoom += P04_CUBE_SPEED_Z;
	if (P04WORLD.cube_zoom >= P04WORLD.moveto) {
		for (n = 0; n < 12; n ++) {
			P04WORLD.cube_posz[n] = P04WORLD.origin;
			P04WORLD.cube_rot[n] = 0;
			P04WORLD.cube_z[n] = 0;
		}
		P04WORLD.eye_z = (128 << 4);
		result = true;
	}

	// Devuelve el resultado
	return result;

}



// Funcion Puzzle04Mix();
bool Puzzle04Mix(void) {

	// Variables
	bool loop = true;		// Control del loop
	u8 thread = 1;			// Hilo
	u8 cube = 0;			// nº de cubo a girar
	u8 counter = 0;			// Control del mezclado
	u8 turns = 0;
	u8 last = 0;
	bool result = true;

	// Bucle
	while (loop) {

		// Lee el teclado
		ReadKeypad();
	
		// Nucleo de ejecucion
		switch (thread) {
			
			////////////////////////////////
			// Selecciona el cubo a rotar //
			////////////////////////////////
			case 1:
				P04WORLD.cube_z[cube] = 1;
				P04WORLD.cube_zoom = 0;
				thread = 2;
				break;

			////////////////////
			// Extrae el cubo //
			////////////////////
			case 2:
				// Zoom del cubo marcado
				if (Puzzle04ZoomInCube()) {
					// Cuando termines el zoom, siguiente thread
					thread = 3;
				}
				break;

			///////////////////////////////////////
			// Registra el movimiento a realizar //
			///////////////////////////////////////
			case 3:
				if (GAMEOPTIONS[0].lefthanded) {
					P04WORLD.cube_rot[cube] = 1;
				} else {
					P04WORLD.cube_rot[cube] = -1;
				}
				// Registra el movimiento y reinicia las variables necesarias
				counter = 0;
				while ((turns == 0) || (turns == last)) {
					turns = ((int)(rand() % 4)) + 1;
				}
				last = turns;
				P04WORLD.cube_rotation = 0;
				thread = 4;
				break;

			////////////////////////////////////////
			// Rota el cubo a derecha o izquierda //
			////////////////////////////////////////
			case 4:
				// Rota el cubo
				if (Puzzle04RotateCube()) {
					// Contabiliza el movimiento
					counter ++;
					// Si los has completado todos..
					if (counter >= turns) {
						// Si has terminado, salta al thread correspondiente
						P04WORLD.cube_zoom = 0;
						thread = 5;
					} else {
						// Si no, dale otro giro
						P04WORLD.cube_rotation = 0;
					}
				}
				break;


			////////////////////////////
			// Inserta el cubo tocado //
			////////////////////////////
			case 5:
				// Zoom del cubo marcado
				if (Puzzle04ZoomOutCube()) {
					// Cuando termines el zoom, siguiente cubo
					cube ++;
					if (cube > P04_PUZZLE_MIX) {
						thread = 255;
						loop = false;
					} else {
						thread = 1;
					}
				}
				break;

			////////////////////////////////
			// Por defecto, no hagas nada //
			////////////////////////////////
			default:
				break;

		}

		// Si has pulsado la tecla de cancelar (Zurdos)
		if (GAMEOPTIONS[0].lefthanded && (KEYPRESS.up || KEYPRESS.down || KEYPRESS.right || KEYPRESS.left)) {
			loop = false;
			result = false;
		}

		// Si has pulsado la tecla de cancelar (Diestros)
		if (!GAMEOPTIONS[0].lefthanded && (KEYPRESS.a || KEYPRESS.b || KEYPRESS.x || KEYPRESS.y)) {
			loop = false;
			result = false;
		}

		// Ejecuta los renders 3D necesarios
		Puzzle04RenderAll();

		// Ejecuta los renders 2D necesarios
		Puzzle04Engine2D();

	}

	// Revuelve el resultado
	return result;

}



// Funcion Puzzle04WriteReport();
void Puzzle04WriteReport(void) {

	// Variables
	u8 n = 0;
	s16 y = 0;
	s16 min = 0;
	s16 sec = 0;
	char text[32];

	// Posicionate en la Y
	if (GAMEOPTIONS[0].lefthanded) {
		y = 8;
	} else {
		y = 7;
	}

	// Actualiza la capa de texto
	for (n = 0; n < 4; n ++) {
		if (SAVEDATA[3].solved[n] == 0) {
			sprintf(text, "T: --:--   M: ---");
		} else {
			min = (int)(SAVEDATA[3].time[n] / 60);
			sec = (SAVEDATA[3].time[n] - (min * 60));
			sprintf(text, "T: %02d:%02d   M: %03d", min, sec, SAVEDATA[3].moves[n]);
		}
		NF_WriteText(1, 1, 3, y, text);
		y += 7;
	}
	NF_UpdateTextLayers();

}



// Funcion Puzzle04CheckRecord();
bool Puzzle04CheckRecord(void) {

	// Variables
	bool record = false;
	u8 puzzle = 0;
	s16 x = 0;
	s16 y = 0;
	s16 n = 0;

	// Obten la ID del puzzle resuelto
	switch (P04WORLD.cube_angle[0]) {
		case 0:
			puzzle = 0;
			break;
		case 128:
			puzzle = 3;
			break;
		case 256:
			puzzle = 2;
			break;
		case 384:
			puzzle = 1;
			break;
		default:
			break;
	}

	// Verifica si hay record de movimientos
	if (P04WORLD.moves < SAVEDATA[3].moves[puzzle]) {
		SAVEDATA[3].moves[puzzle] = P04WORLD.moves;
		record = true;
	}

	// Verifica si hay record de tiempo
	if (P04WORLD.time < SAVEDATA[3].time[puzzle]) {
		SAVEDATA[3].time[puzzle] = P04WORLD.time;
		record = true;
	}

	// Si hay un record, marcalo en pantalla
	if (record) {
		// Marcalo
		P04WORLD.record = true;
		// Muestra el marcador de record en pantalla
		if (GAMEOPTIONS[0].lefthanded) {
		x = (28 + (puzzle * 56));
		y = 48;
			for (n = 21; n <= 23; n ++) {
				NF_MoveSprite(1, n, x, y);
				y -= 16;
			}
		} else {
			x = (212 - (puzzle * 56));
			y = 128;
			for (n = 21; n <= 23; n ++) {
				NF_MoveSprite(1, n, x, y);
				y += 16;
			}
		}
	}

	// Suma 1 al contador de resueltos
	SAVEDATA[3].solved[puzzle] ++;

	// Devuelve si hay record
	return record;

}



// Funcion Puzzle04ExitMenu();
bool Puzzle04ExitMenu(void) {

	// Variables
	bool loop = true;			// Control del bucle
	bool result = false;		// Resultado
	u8 thread = 0;				// Hilo de ejecucion
	s32 x[2];					// Posicion de los botones
	s32 in[2];
	s32 out[2];
	s32 speed = 0;
	bool touch = false;			// Control del tactil
	s8 n = 0;					// Uso general

	// Inicializa las variables
	x[0] = (176 << 8);
	x[1] = (112 << 8);
	in[0] = (112 << 8);
	in[1] = (176 << 8);
	out[0] = x[0];
	out[1] = x[1];


	// Bucle de ejecucion
	while (loop) {

		// Lectura de touchpad y teclas
		ReadTouchpad();
		ReadKeypad();

		// Hilos
		switch (thread) {

			// Entrada de los botones
			case 0:
				// Calcula la posicion de los botones
				for (n = 1; n >=0; n --) {
					speed = (in[n] >> 3);
					if (speed < 512) speed = 512;
					if ((in[1] > (64 << 8)) && (n == 0)) speed = 0;
					in[n] -= speed;
					if (in[n] < 0) in[n] = 0;
				}
				// Dibuja los botones
				Puzzle04DrawMenu(((x[0] + in[0]) >> 8), 160, ((x[1] + in[1]) >> 8), 160);
				// Verifica si ya se han posicionado para continuar...
				if ((in[0] == 0) && (in[1] == 0)) thread = 1;
				break;

			// Espera a que selecciones una opcion
			case 1:
				// Control del touchpad
				if (TOUCHPAD.held) {
					// Si es el primer toque, verifica si has tocado un boton
					if (!touch) {
						if (GAMEOPTIONS[0].lefthanded) {
							// Boton "Repetir"
							if ((TOUCHPAD.pos_x >= 80) && (TOUCHPAD.pos_x <= 105) && (TOUCHPAD.pos_y >= 26) && (TOUCHPAD.pos_y <= 165)) {
								result = true;
								thread = 2;
							}
							// Boton "Salir"
							if ((TOUCHPAD.pos_x >= 149) && (TOUCHPAD.pos_x <= 174) && (TOUCHPAD.pos_y >= 26) && (TOUCHPAD.pos_y <= 165)) {
								result = false;
								thread = 2;
							}
						} else {
							// Boton "Repetir"
							if ((TOUCHPAD.pos_x >= 151) && (TOUCHPAD.pos_x <= 176) && (TOUCHPAD.pos_y >= 27) && (TOUCHPAD.pos_y <= 166)) {
								result = true;
								thread = 2;
							}
							// Boton "Salir"
							if ((TOUCHPAD.pos_x >= 82) && (TOUCHPAD.pos_x <= 107) && (TOUCHPAD.pos_y >= 27) && (TOUCHPAD.pos_y <= 166)) {
								result = false;
								thread = 2;
							}
						}
					}
					// Marca el primer toque
					touch = true;
				} else {
					// Se ha levantado el stylus
					touch = false;
				}
				// Dibuja los botones
				Puzzle04DrawMenu((x[0] >> 8), 160, (x[1] >> 8), 160);
				break;

			// Salida de los botones
			case 2:
				// Calcula la posicion de los botones
				for (n = 1; n >=0; n --) {
					speed = ((x[n] - out[n]) >> 3);
					if (speed < 512) speed = 512;
					if ((out[1] > (64 << 8)) && (n == 0)) speed = 0;
					out[n] -= speed;
					if (out[n] < 0) out[n] = 0;
				}
				// Dibuja los botones
				Puzzle04DrawMenu((out[0] >> 8), 160, (out[1] >> 8), 160);
				// Verifica si ya se han posicionado para continuar...
				if ((out[0] == 0) && (out[1] == 0)) {
					thread = 255;
					loop = false;
				}
				break;

			// default
			default:
				break;

		}
		
		// Ejecuta los renders 3D necesarios
		Puzzle04RenderAll();

		// Ejecuta los renders 2D necesarios
		Puzzle04Engine2D();

	}

	// Devuelve el resultado
	return result;

}
