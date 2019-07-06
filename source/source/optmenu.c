
// Mind Maze - Funciones del Menu de opciones
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





// Funcion OptMenu();
void OptMenu(void) {

	//////////////////////////
	//						//
	//	Inicializaciones	//
	//						//
	//////////////////////////

	// Resetealo todo
	ResetAll();


	// Variables locales

	s32 bg_pos = 0;				// Posicion del fondo
	s32 bg_y = 0;

	bool fade_in = true;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	bool loop = true;			// Control del bucle principal

	OPTMCONTROL.lefthanded = GAMEOPTIONS[0].lefthanded;		// Copia de LEFTHANDED


	// Inicializa variables globales
	OPTMCONTROL.touch_down = false;
	OPTMCONTROL.thread = 0;


	// Habilita el 3D en la pantalla inferior
	Set3DinTouchScreen();



	//////////////////////
	//					//
	//	Crea el menu	//
	//					//
	//////////////////////

	// Carga los archivos necesarios
	LoadOptMenuFiles();

	// Crea el menu
	CreateOptMenu();


	//////////////////////////
	//						//
	//	Nucleo del Menu		//
	//						//
	//////////////////////////

	// Bucle principal del menu
	while (loop) {

		// Lee el touchpad
		ReadTouchpad();

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
			// Ajusta el brillo
			setBrightness(3, (0 - (fade >> 8)));
			// Si es necesario, ajusta el volumen
			/*
			if ((menu_option == 2) || (menu_option == 10)) {
				// Ajusta el volumen
				BgmVolume(((4096 - fade) >> 2));
			}
			*/
		}

		// Calcula el movimiento de los fondos
		if (OPTMCONTROL.lefthanded) {
			bg_pos -= 128;
			if (bg_pos < 0) bg_pos += (128 << 8);
			bg_y -= 128;
			if (bg_y < 0) bg_y += (16 << 8);
		} else {
			bg_pos += 128;
			if (bg_pos > (128 << 8)) bg_pos -= (128 << 8);
			bg_y += 128;
			if (bg_y > (16 << 8)) bg_y -= (16 << 8);
		}


		// Minikernel de capas del menu
		if (!fade_in && !fade_out && loop) {
			fade_out = OPTM_Kernel();
		}


		// Dibujado en pantalla y sincronismo
		NF_Draw3dSprites();
		glFlush(0);
		swiWaitForVBlank();

		// Mueve las capas de fondo
		NF_ScrollBg(0, 3, (bg_pos >> 8), (bg_pos >> 8));
		NF_ScrollBg(1, 3, (bg_pos >> 8), (bg_pos >> 8));
		if (OPTMCONTROL.lefthanded) {
			NF_ScrollBg(0, 2, (bg_y >> 8), 64);
		} else {
			NF_ScrollBg(0, 2, (bg_y >> 8), 0);
		}

	}

}



// Funcion LoadOptMenuFiles();
void LoadOptMenuFiles(void) {

	// Variables
	u8 n = 0;
	char file[128];
	char lang[4];

	// Medidas de los botones
	u16 size_x[10];
	u16 size_y[10];

	// Inicializa las medidas de los botones
	for (n = 0; n < 10; n ++) {
		size_x[n] = 32; size_y[n] = 128;
	}

	// Idioma
	if (GAMEOPTIONS[0].language) {
		sprintf(lang, "en");
	} else {
		sprintf(lang, "es");
	}

	// Carga los fondos
	NF_LoadTiledBg("optmenu/bg/optm_bg3", "bg3", 512, 512);
	sprintf(file, "optmenu/bg/optm_bg2_%s", lang);
	NF_LoadTiledBg(file, "bg2", 256, 256);
	NF_LoadTiledBg("optmenu/bg/optm_bg0", "bg0", 256, 256);

	// Carga los botones del menu (0 - 3)
	for (n = 0; n < 4; n ++) {
		sprintf(file, "optmenu/sprite/optmb%02d_%s", (n + 1), lang);
		NF_LoadSpriteGfx(file, n, size_x[n], size_y[n]);
		NF_LoadSpritePal(file, n);
	}
	// Carga lo botones del cambio de idioma (4 - 5)
	for (n = 0; n < 2; n ++) {
		sprintf(file, "optmenu/sprite/optm_button_%02d", (n + 11));
		NF_LoadSpriteGfx(file, (n + 4), size_x[(n + 4)], size_y[(n + 4)]);
		NF_LoadSpritePal(file, (n + 4));
	}
	// Carga lo botones del cambio de orientacion (6 - 7)
	for (n = 0; n < 2; n ++) {
		sprintf(file, "optmenu/sprite/optm_button_%02d_%s", (n + 21), lang);
		NF_LoadSpriteGfx(file, (n + 6), size_x[(n + 6)], size_y[(n + 6)]);
		NF_LoadSpritePal(file, (n + 6));
	}
	// Carga lo botones del borrado de datos (8 - 9)
	for (n = 0; n < 2; n ++) {
		sprintf(file, "optmenu/sprite/optm_button_%02d_%s", (n + 31), lang);
		NF_LoadSpriteGfx(file, (n + 8), size_x[(n + 8)], size_y[(n + 8)]);
		NF_LoadSpritePal(file, (n + 8));
	}

	// Si es para zurdos...
	if (GAMEOPTIONS[0].lefthanded) {
		// Rota los fondos
		RotateTiledBg("bg3");
		RotateTiledBg("bg2");
		RotateTiledBg("bg0");
		// Rota los botones
		for (n = 0; n < 10; n ++) {
			Rotate3dSprite(n);
		}
	}

}



// Funcion CreateOptMenu();
void CreateOptMenu(void) {

	// Variables
	u8 n = 0;

	// Inicializa la posicion de los botones
	if (GAMEOPTIONS[0].lefthanded) {
		// Menu principal
		OPTM[0].fx = (28 << 8); OPTM[0].fy = (32 << 8);
		OPTM[1].fx = (84 << 8); OPTM[1].fy = (32 << 8);
		OPTM[2].fx = (140 << 8); OPTM[2].fy = (32 << 8);
		OPTM[3].fx = (196 << 8); OPTM[3].fy = (32 << 8);
		// Cambio de idioma
		OPTM[4].fx = (84 << 8); OPTM[4].fy = -(128 << 8);
		OPTM[5].fx = (140 << 8); OPTM[5].fy = -(128 << 8);
		// Cambio de orientacion
		OPTM[6].fx = (84 << 8); OPTM[6].fy = -(128 << 8);
		OPTM[7].fx = (140 << 8); OPTM[7].fy = -(128 << 8);
		// Borrado de datos
		OPTM[8].fx = (84 << 8); OPTM[8].fy = -(128 << 8);
		OPTM[9].fx = (140 << 8); OPTM[9].fy = -(128 << 8);
	} else {
		// Menu principal
		OPTM[0].fx = (196 << 8); OPTM[0].fy = (32 << 8);
		OPTM[1].fx = (140 << 8); OPTM[1].fy = (32 << 8);
		OPTM[2].fx = (84 << 8); OPTM[2].fy = (32 << 8);
		OPTM[3].fx = (28 << 8); OPTM[3].fy = (32 << 8);
		// Cambio de idioma
		OPTM[4].fx = (140 << 8); OPTM[4].fy = (192 << 8);
		OPTM[5].fx = (84 << 8); OPTM[5].fy = (192 << 8);
		// Cambio de orientacion
		OPTM[6].fx = (140 << 8); OPTM[6].fy = (192 << 8);
		OPTM[7].fx = (84 << 8); OPTM[7].fy = (192 << 8);
		// Borrado de datos
		OPTM[8].fx = (140 << 8); OPTM[8].fy = (192 << 8);
		OPTM[9].fx = (84 << 8); OPTM[9].fy = (192 << 8);
	}

	// Crea los fondos
	NF_CreateTiledBg(0, 3, "bg3");
	NF_CreateTiledBg(1, 3, "bg3");
	NF_CreateTiledBg(1, 2, "bg2");
	NF_CreateTiledBg(0, 2, "bg0");

	// Ajusta los fondos si es zurdo
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(0, 2, 0, 64);
		NF_ScrollBg(1, 2, 0, 64);
	}

	// Crea los botones del menu principal (20 - 23)
	for (n = 0; n < 4; n ++) {
		NF_Vram3dSpriteGfx(n, (n + 20), true);
		NF_Vram3dSpritePal(n, (n + 20));
		if (OPTM[n].fx < 0) {
			OPTM[n].x = -((-OPTM[n].fx) >> 8);
		} else {
			OPTM[n].x = (OPTM[n].fx >> 8);
		}
		if (OPTM[n].fy < 0) {
			OPTM[n].y = -((-OPTM[n].fy) >> 8);
		} else {
			OPTM[n].y = (OPTM[n].fy >> 8);
		}
		NF_Create3dSprite((n + 20), (n + 20), (n + 20), OPTM[n].x, OPTM[n].y);
	}
	// Crea los botones del cambio de idioma (10 - 11),
	// de cambio de orientacion (12 - 13),
	// y de borrado de datos (14 - 15)
	for (n = 0; n < 6; n ++) {
		NF_Vram3dSpriteGfx((n + 4), (n + 10), true);
		NF_Vram3dSpritePal((n + 4), (n + 10));
		if (OPTM[(n + 4)].fx < 0) {
			OPTM[(n + 4)].x = -((-OPTM[(n + 4)].fx) >> 8);
		} else {
			OPTM[(n + 4)].x = (OPTM[(n + 4)].fx >> 8);
		}
		if (OPTM[(n + 4)].fy < 0) {
			OPTM[(n + 4)].y = -((-OPTM[(n + 4)].fy) >> 8);
		} else {
			OPTM[(n + 4)].y = (OPTM[(n + 4)].fy >> 8);
		}
		NF_Create3dSprite((n + 10), (n + 10), (n + 10), OPTM[(n + 4)].x, OPTM[(n + 4)].y);
	}
	// Ordena los sprites
	NF_Sort3dSprites();

	// Ajusta la posicion de la capa 3D
	NF_3dSpritesLayer(2);

	// Dibujado en pantalla y sincronismo
	NF_Draw3dSprites();
	glFlush(0);
	swiWaitForVBlank();

}



// Micro kernel del menu de opciones
bool OPTM_Kernel(void) {

	// Variables
	bool exit = false;

	// Nucleo de ejecucion
	switch (OPTMCONTROL.thread) {

		//////////////////////////////////////////////////
		// Menu principal								//
		//////////////////////////////////////////////////

		// Menu general
		case 0:
			
			// Segun el resultado devuelto por el menu principal...
			switch (OPTM_Main()) {

				// Se ha pulsado el boton de cambio de idioma (20)
				case 0:
					OPTMCONTROL.counter = 64;	// Tamaño del boton
					OPTMCONTROL.thread = 11;
					break;

				// Se ha pulsado el boton de cambio de orientacion (21)
				case 1:
					OPTMCONTROL.counter = 64;	// Tamaño del boton
					OPTMCONTROL.thread = 21;
					break;

				// Se ha pulsado el boton del borrado de datos (22)
				case 2:
					OPTMCONTROL.counter = 64;	// Tamaño del boton
					OPTMCONTROL.thread = 31;
					break;

				// Se ha pulsado el boton de salir (23)
				case 3:
					exit = true;
					break;

				default:
					break;

			}

			break;


		//////////////////////////////////////////////////
		// Cambio de idioma								//
		//////////////////////////////////////////////////

		// Animacion entrada cambio de idioma, zoom out
		case 11:
			if (OPTM_LangMenIn(0)) {
				OPTMCONTROL.counter = (192 << 8);
				OPTMCONTROL.thread = 12;
			}
			break;


		// Animacion entrada cambio de idioma, botones dentro
		case 12:
			if (OPTM_LangMenIn(1)) {
				OPTMCONTROL.thread = 13;
			}
			break;


		// Espera que se seleccion una opcion del menu de cambio de idioma
		case 13:

			// Segun el resultado devuelto por el menu de idioma...
			switch (OPTM_Main()) {

				// Se ha seleccionado Español
				case 1:
					if (GAMEOPTIONS[0].language) {
						// Esta en Ingles, cambialo a Español
						GAMEOPTIONS[0].language = 0;
						// Guarda los datos
						WriteSavegame();
						// Y sal al menu del juego para aplicar los cambios
						exit = true;
					} else {
						// Ya esta en Español, vuelve al menu anterior
						OPTMCONTROL.counter = (32 << 8);
						OPTMCONTROL.thread = 14;
					}
					break;

				// Se ha seleccionado Ingles
				case 2:
					if (!GAMEOPTIONS[0].language) {
						// Esta en Español, cambialo a Ingles
						GAMEOPTIONS[0].language = 1;
						// Guarda los datos
						WriteSavegame();
						// Y sal al menu del juego para aplicar los cambios
						exit = true;
					} else {
						// Ya esta en Ingles, vuelve al menu anterior
						OPTMCONTROL.counter = (32 << 8);
						OPTMCONTROL.thread = 14;
					}
					break;

				// Se ha seleccionado volver
				case 3:
					OPTMCONTROL.counter = (32 << 8);
					OPTMCONTROL.thread = 14;
					break;

				default:
					break;

			}

			break;


		// Vuelve del menu de Idioma, al menu principal, botones fuera
		case 14:
			if (OPTM_LangMenOut(1)) {
				OPTMCONTROL.counter = 48;
				OPTMCONTROL.thread = 15;
			}
			break;


		// Vuelve del menu de Idioma, al menu principal, zoom in
		case 15:
			if (OPTM_LangMenOut(0)) {
				OPTMCONTROL.thread = 0;
			}
			break;


		//////////////////////////////////////////////////
		// Cambio de orientacion						//
		//////////////////////////////////////////////////

		// Animacion entrada cambio de orientacion, zoom out
		case 21:
			if (OPTM_HandMenIn(0)) {
				NF_Swap3dSpritePriority(20, 21);
				OPTMCONTROL.counter = (56 >> 1);
				OPTMCONTROL.thread = 22;
			}
			break;

		// Animacion entrada cambio de orientacion, move up
		case 22:
			if (OPTM_HandMenIn(1)) {
				OPTMCONTROL.counter = (192 << 8);
				OPTMCONTROL.thread = 23;
			}
			break;

		// Animacion entrada cambio de orientacion, buttons in
		case 23:
			if (OPTM_HandMenIn(2)) {
				OPTMCONTROL.thread = 24;
			}
			break;

		// Espera que se seleccion una opcion del menu de cambio de orientacion
		case 24:

			// Segun el resultado devuelto por el menu de idioma...
			switch (OPTM_Main()) {

				// Se ha seleccionado Diestro
				case 1:
					if (GAMEOPTIONS[0].lefthanded) {
						// Esta en Zurdo, cambialo a Diestro
						GAMEOPTIONS[0].lefthanded = false;
						// Guarda los datos
						WriteSavegame();
						// Y sal al menu del juego para aplicar los cambios
						exit = true;
					} else {
						// Ya esta en Diestro, vuelve al menu anterior
						OPTMCONTROL.counter = (32 << 8);
						OPTMCONTROL.thread = 25;
					}
					break;

				// Se ha seleccionado Zurdo
				case 2:
					if (!GAMEOPTIONS[0].lefthanded) {
						// Esta en Diestro, cambialo a Zurdo
						GAMEOPTIONS[0].lefthanded = true;
						// Guarda los datos
						WriteSavegame();
						// Y sal al menu del juego para aplicar los cambios
						exit = true;
					} else {
						// Ya esta en Zurdo, vuelve al menu anterior
						OPTMCONTROL.counter = (32 << 8);
						OPTMCONTROL.thread = 25;
					}
					break;

				// Se ha seleccionado volver
				case 3:
					OPTMCONTROL.counter = (32 << 8);
					OPTMCONTROL.thread = 25;
					break;

				default:
					break;

			}

			break;


		// Animacion salida cambio de orientacion, buttons out
		case 25:
			if (OPTM_HandMenOut(2)) {
				OPTMCONTROL.counter = (56 >> 1);
				OPTMCONTROL.thread = 26;
			}
			break;


		// Animacion salida cambio de orientacion, move down
		case 26:
			if (OPTM_HandMenOut(1)) {
				NF_Swap3dSpritePriority(20, 21);
				OPTMCONTROL.counter = 48;
				OPTMCONTROL.thread = 27;
			}
			break;

		// Animacion salida cambio de orientacion, zoom in
		case 27:
			if (OPTM_HandMenOut(0)) {
				OPTMCONTROL.thread = 0;
			}
			break;


		//////////////////////////////////////////////////
		// Borrado de datos								//
		//////////////////////////////////////////////////

		// Animacion entrada borrado de datos, zoom out
		case 31:
			if (OPTM_DeleteMenIn(0)) {
				NF_Swap3dSpritePriority(20, 22);
				OPTMCONTROL.counter = (56 >> 1);
				OPTMCONTROL.thread = 32;
			}
			break;

		// Animacion entrada borrado de datos, move up
		case 32:
			if (OPTM_DeleteMenIn(1)) {
				OPTMCONTROL.counter = (192 << 8);
				OPTMCONTROL.thread = 33;
			}
			break;

		// Animacion entrada borrado de datos, buttons in
		case 33:
			if (OPTM_DeleteMenIn(2)) {
				OPTMCONTROL.thread = 34;
			}
			break;

		// Espera que se seleccion una opcion del menu borrado de datos
		case 34:

			// Segun el resultado devuelto por el menu de borrado de datos...
			switch (OPTM_Main()) {

				// Se ha seleccionado Borrar los datos
				case 1:
					InitAllPuzzlesSaveData();
					WriteSavegame();
					OPTMCONTROL.counter = (32 << 8);
					OPTMCONTROL.thread = 35;
					break;

				// Se ha seleccionado no borrar los datos
				case 2:
					OPTMCONTROL.counter = (32 << 8);
					OPTMCONTROL.thread = 35;
					break;

				// Se ha seleccionado volver
				case 3:
					OPTMCONTROL.counter = (32 << 8);
					OPTMCONTROL.thread = 35;
					break;

				default:
					break;

			}

			break;


		// Animacion salida cambio de orientacion, buttons out
		case 35:
			if (OPTM_DeleteMenOut(2)) {
				OPTMCONTROL.counter = (56 >> 1);
				OPTMCONTROL.thread = 36;
			}
			break;


		// Animacion salida cambio de orientacion, move down
		case 36:
			if (OPTM_DeleteMenOut(1)) {
				NF_Swap3dSpritePriority(20, 22);
				OPTMCONTROL.counter = 48;
				OPTMCONTROL.thread = 37;
			}
			break;

		// Animacion salida cambio de orientacion, zoom in
		case 37:
			if (OPTM_DeleteMenOut(0)) {
				OPTMCONTROL.thread = 0;
			}
			break;



		// Error trap
		default:
			break;

	}

	// Devuelve el estado
	return exit;

}



// Menu de opciones, menu principal, test de botones
u8 OPTM_Main(void) {

	// Variables
	u8 n = 0;
	s16 x = 28;
	s16 y = 32;
	u8 r = 255;
	u8 result = 0;

	// Verifica si se ha pulsado sobre algun boton del menu
	if (TOUCHPAD.held) {		// Si tocas la pantalla
		if (!OPTMCONTROL.touch_down) {		// Y es el primer toque
			for (n = 0; n < 4; n ++) {		// Verifica si has tocado un boton
				if (
					(TOUCHPAD.pos_x >= x)
					&&
					(TOUCHPAD.pos_x <= (x + 32))
					&&
					(TOUCHPAD.pos_y >= y)
					&&
					(TOUCHPAD.pos_y <= (y + 128))
					) {
						r = n;
				}
				x += 56;
			}
		}
		OPTMCONTROL.touch_down = true;
	} else {	// Si no tocas la pantalla
		OPTMCONTROL.touch_down = false;
	}

	// Calcula la opcion devuelta
	if (r != 255) {
		if (OPTMCONTROL.lefthanded) {
			result = r;
		} else {
			result = (3 - r);
		}
	} else {
		result = 255;
	}

	// Devuelve el resultado
	return result;

}



// Funcion OPTM_LangMenIn();
bool OPTM_LangMenIn(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom out botones
		case 0:
			// Cambia el tamaño de los botones (21 y 22)
			OPTMCONTROL.counter --;
			// Si se han escaldo al 75%, sal
			if (OPTMCONTROL.counter <= 48) {
				OPTMCONTROL.counter = 48;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(21, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(22, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(21, 21, alpha);
			NF_Blend3dSprite(22, 22, alpha);
			break;

		// Entrada de los botones
		case 1:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter -= value;
			if (OPTMCONTROL.counter <= (32 << 8)) {
				OPTMCONTROL.counter = (32 << 8);
				exit = true;
			} else {
				// Coloca los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 4)].fy += value;
					} else {
						OPTM[(n + 4)].fy -= value;
					}
					if (OPTM[(n + 4)].fx < 0) {
						OPTM[(n + 4)].x = -((-OPTM[(n + 4)].fx) >> 8);
					} else {
						OPTM[(n + 4)].x = (OPTM[(n + 4)].fx >> 8);
					}
					if (OPTM[(n + 4)].fy < 0) {
						OPTM[(n + 4)].y = -((-OPTM[(n + 4)].fy) >> 8);
					} else {
						OPTM[(n + 4)].y = (OPTM[(n + 4)].fy >> 8);
					}
					NF_Move3dSprite((n + 10), OPTM[(n + 4)].x, OPTM[(n + 4)].y);
				}
			}
			break;

		// Error trap
		default:
			break;

	}

	// Devuelve el resultado
	return exit;

}



// Funcion OPTM_LangMenOut();
bool OPTM_LangMenOut(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom In botones
		case 0:
			// Cambia el tamaño de los botones (21 y 22)
			OPTMCONTROL.counter ++;
			// Si se han escaldo al 100%, sal
			if (OPTMCONTROL.counter >= 64) {
				OPTMCONTROL.counter = 64;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(21, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(22, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(21, 21, alpha);
			NF_Blend3dSprite(22, 22, alpha);
			break;

		// Entrada de los botones
		case 1:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter += value;
			if (OPTMCONTROL.counter >= (192 << 8)) {
				OPTMCONTROL.counter = (192 << 8);
				exit = true;
			} else {
				// Coloca los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 4)].fy -= value;
					} else {
						OPTM[(n + 4)].fy += value;
					}
					if (OPTM[(n + 4)].fx < 0) {
						OPTM[(n + 4)].x = -((-OPTM[(n + 4)].fx) >> 8);
					} else {
						OPTM[(n + 4)].x = (OPTM[(n + 4)].fx >> 8);
					}
					if (OPTM[(n + 4)].fy < 0) {
						OPTM[(n + 4)].y = -((-OPTM[(n + 4)].fy) >> 8);
					} else {
						OPTM[(n + 4)].y = (OPTM[(n + 4)].fy >> 8);
					}
					NF_Move3dSprite((n + 10), OPTM[(n + 4)].x, OPTM[(n + 4)].y);
				}
			}
			break;

		// Error trap
		default:
			break;

	}

	// Reinicia la posicion de los botones si se ha completado la animacion
	if (exit) {
		if (OPTMCONTROL.lefthanded) {
			OPTM[4].fx = (84 << 8); OPTM[4].fy = -(128 << 8);
			OPTM[5].fx = (140 << 8); OPTM[5].fy = -(128 << 8);
		} else {
			OPTM[4].fx = (140 << 8); OPTM[4].fy = (192 << 8);
			OPTM[5].fx = (84 << 8); OPTM[5].fy = (192 << 8);
		}
		for (n = 0; n < 2; n ++) {
			if (OPTM[(n + 4)].fx < 0) {
				OPTM[(n + 4)].x = -((-OPTM[(n + 4)].fx) >> 8);
			} else {
				OPTM[(n + 4)].x = (OPTM[(n + 4)].fx >> 8);
			}
			if (OPTM[(n + 4)].fy < 0) {
				OPTM[(n + 4)].y = -((-OPTM[(n + 4)].fy) >> 8);
			} else {
				OPTM[(n + 4)].y = (OPTM[(n + 4)].fy >> 8);
			}
			NF_Move3dSprite((n + 10), OPTM[(n + 4)].x, OPTM[(n + 4)].y);
		}
	}

	// Devuelve el resultado
	return exit;

}



// Funcion OPTM_HandMenIn();
bool OPTM_HandMenIn(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom out botones
		case 0:
			// Cambia el tamaño de los botones (20 y 22)
			OPTMCONTROL.counter --;
			// Si se han escaldo al 75%, sal
			if (OPTMCONTROL.counter <= 48) {
				OPTMCONTROL.counter = 48;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(20, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(22, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(20, 20, alpha);
			NF_Blend3dSprite(22, 22, alpha);
			break;

		// Mueve el boton arriba
		case 1:
			// Mueve el boton (21)
			OPTMCONTROL.counter --;
			// Completado el movimiento, siguiente paso
			if (OPTMCONTROL.counter <= 0) {
				OPTMCONTROL.counter = 0;
				exit = true;
			}
			if (OPTMCONTROL.lefthanded) { 
				OPTM[1].fx -= 512;
			} else {
				OPTM[1].fx += 512;
			}
			if (OPTM[1].fx < 0) {
				OPTM[1].x = -((-OPTM[1].fx) >> 8);
			} else {
				OPTM[1].x = (OPTM[1].fx >> 8);
			}
			if (OPTM[1].fy < 0) {
				OPTM[1].y = -((-OPTM[1].fy) >> 8);
			} else {
				OPTM[1].y = (OPTM[1].fy >> 8);
			}
			NF_Move3dSprite(21, OPTM[1].x, OPTM[1].y);
			break;

		// Entrada de los botones
		case 2:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter -= value;
			// Verifica si es el fin del movimiento...
			if (OPTMCONTROL.counter <= (32 << 8)) {
				OPTMCONTROL.counter = (32 << 8);
				exit = true;
			} else {
				// Si no, mueve los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 6)].fy += value;
					} else {
						OPTM[(n + 6)].fy -= value;
					}
					if (OPTM[(n + 6)].fx < 0) {
						OPTM[(n + 6)].x = -((-OPTM[(n + 6)].fx) >> 8);
					} else {
						OPTM[(n + 6)].x = (OPTM[(n + 6)].fx >> 8);
					}
					if (OPTM[(n + 6)].fy < 0) {
						OPTM[(n + 6)].y = -((-OPTM[(n + 6)].fy) >> 8);
					} else {
						OPTM[(n + 6)].y = (OPTM[(n + 6)].fy >> 8);
					}
					NF_Move3dSprite((n + 12), OPTM[(n + 6)].x, OPTM[(n + 6)].y);
				}
			}

			break;

		// Error trap
		default:
			break;

	}

	// Devuelve el resultado
	return exit;

}



// Funcion OPTM_HandMenOut();
bool OPTM_HandMenOut(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom out botones
		case 0:
			// Cambia el tamaño de los botones (20 y 22)
			OPTMCONTROL.counter ++;
			// Si se han escaldo al 100%, sal
			if (OPTMCONTROL.counter >= 64) {
				OPTMCONTROL.counter = 64;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(20, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(22, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(20, 20, alpha);
			NF_Blend3dSprite(22, 22, alpha);
			break;

		// Mueve el boton Abajo
		case 1:
			// Mueve el boton (21)
			OPTMCONTROL.counter --;
			// Completado el movimiento, siguiente paso
			if (OPTMCONTROL.counter <= 0) {
				OPTMCONTROL.counter = 0;
				exit = true;
			}
			if (OPTMCONTROL.lefthanded) { 
				OPTM[1].fx += 512;
			} else {
				OPTM[1].fx -= 512;
			}
			if (OPTM[1].fx < 0) {
				OPTM[1].x = -((-OPTM[1].fx) >> 8);
			} else {
				OPTM[1].x = (OPTM[1].fx >> 8);
			}
			if (OPTM[1].fy < 0) {
				OPTM[1].y = -((-OPTM[1].fy) >> 8);
			} else {
				OPTM[1].y = (OPTM[1].fy >> 8);
			}
			NF_Move3dSprite(21, OPTM[1].x, OPTM[1].y);
			break;

		// Salida de los botones
		case 2:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter += value;
			// Verifica si es el fin del movimiento...
			if (OPTMCONTROL.counter >= (192 << 8)) {
				OPTMCONTROL.counter = (192 << 8);
				exit = true;
			} else {
				// Si no, mueve los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 6)].fy -= value;
					} else {
						OPTM[(n + 6)].fy += value;
					}
					if (OPTM[(n + 6)].fx < 0) {
						OPTM[(n + 6)].x = -((-OPTM[(n + 6)].fx) >> 8);
					} else {
						OPTM[(n + 6)].x = (OPTM[(n + 6)].fx >> 8);
					}
					if (OPTM[(n + 6)].fy < 0) {
						OPTM[(n + 6)].y = -((-OPTM[(n + 6)].fy) >> 8);
					} else {
						OPTM[(n + 6)].y = (OPTM[(n + 6)].fy >> 8);
					}
					NF_Move3dSprite((n + 12), OPTM[(n + 6)].x, OPTM[(n + 6)].y);
				}
			}

			break;



		// Error trap
		default:
			break;

	}

	// Reinicia la posicion de los botones si se ha completado la animacion
	if (exit) {
		if (OPTMCONTROL.lefthanded) {
			OPTM[6].fx = (84 << 8); OPTM[6].fy = -(128 << 8);
			OPTM[7].fx = (140 << 8); OPTM[7].fy = -(128 << 8);
		} else {
			OPTM[6].fx = (140 << 8); OPTM[6].fy = (192 << 8);
			OPTM[7].fx = (84 << 8); OPTM[7].fy = (192 << 8);
		}
		for (n = 0; n < 2; n ++) {
			if (OPTM[(n + 6)].fx < 0) {
				OPTM[(n + 6)].x = -((-OPTM[(n + 6)].fx) >> 8);
			} else {
				OPTM[(n + 6)].x = (OPTM[(n + 6)].fx >> 8);
			}
			if (OPTM[(n + 6)].fy < 0) {
				OPTM[(n + 6)].y = -((-OPTM[(n + 6)].fy) >> 8);
			} else {
				OPTM[(n + 6)].y = (OPTM[(n + 6)].fy >> 8);
			}
			NF_Move3dSprite((n + 12), OPTM[(n + 6)].x, OPTM[(n + 6)].y);
		}
	}

	// Devuelve el resultado
	return exit;

}



// Funcion OPTM_DeleteMenIn();
bool OPTM_DeleteMenIn(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom out botones
		case 0:
			// Cambia el tamaño de los botones (20 y 21)
			OPTMCONTROL.counter --;
			// Si se han escaldo al 75%, sal
			if (OPTMCONTROL.counter <= 48) {
				OPTMCONTROL.counter = 48;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(20, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(21, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(20, 20, alpha);
			NF_Blend3dSprite(21, 21, alpha);
			break;

		// Mueve el boton arriba
		case 1:
			// Mueve el boton (22)
			OPTMCONTROL.counter --;
			// Completado el movimiento, siguiente paso
			if (OPTMCONTROL.counter <= 0) {
				OPTMCONTROL.counter = 0;
				exit = true;
			}
			if (OPTMCONTROL.lefthanded) { 
				OPTM[2].fx -= 1024;
			} else {
				OPTM[2].fx += 1024;
			}
			if (OPTM[2].fx < 0) {
				OPTM[2].x = -((-OPTM[2].fx) >> 8);
			} else {
				OPTM[2].x = (OPTM[2].fx >> 8);
			}
			if (OPTM[2].fy < 0) {
				OPTM[2].y = -((-OPTM[2].fy) >> 8);
			} else {
				OPTM[2].y = (OPTM[2].fy >> 8);
			}
			NF_Move3dSprite(22, OPTM[2].x, OPTM[2].y);
			break;

		// Entrada de los botones
		case 2:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter -= value;
			// Verifica si es el fin del movimiento...
			if (OPTMCONTROL.counter <= (32 << 8)) {
				OPTMCONTROL.counter = (32 << 8);
				exit = true;
			} else {
				// Si no, mueve los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 8)].fy += value;
					} else {
						OPTM[(n + 8)].fy -= value;
					}
					if (OPTM[(n + 8)].fx < 0) {
						OPTM[(n + 8)].x = -((-OPTM[(n + 8)].fx) >> 8);
					} else {
						OPTM[(n + 8)].x = (OPTM[(n + 8)].fx >> 8);
					}
					if (OPTM[(n + 8)].fy < 0) {
						OPTM[(n + 8)].y = -((-OPTM[(n + 8)].fy) >> 8);
					} else {
						OPTM[(n + 8)].y = (OPTM[(n + 8)].fy >> 8);
					}
					NF_Move3dSprite((n + 14), OPTM[(n + 8)].x, OPTM[(n + 8)].y);
				}
			}

			break;

		// Error trap
		default:
			break;

	}

	// Devuelve el resultado
	return exit;

}



// Funcion DeleteMenOut();
bool OPTM_DeleteMenOut(u8 thread) {

	// Variables
	bool exit = false;
	s16 alpha = 0;
	s32 value = 0;
	u8 n = 0;

	// Kernel de la funcion
	switch (thread) {

		// Zoom out botones
		case 0:
			// Cambia el tamaño de los botones (20 y 21)
			OPTMCONTROL.counter ++;
			// Si se han escaldo al 100%, sal
			if (OPTMCONTROL.counter >= 64) {
				OPTMCONTROL.counter = 64;
				exit = true;
			}
			// Aplica el cambio de tamaño
			NF_Scale3dSprite(20, OPTMCONTROL.counter, OPTMCONTROL.counter);
			NF_Scale3dSprite(21, OPTMCONTROL.counter, OPTMCONTROL.counter);
			// Aplica el alpha
			alpha = (31 - (32 - (OPTMCONTROL.counter >> 1)));
			NF_Blend3dSprite(20, 20, alpha);
			NF_Blend3dSprite(21, 21, alpha);
			break;

		// Mueve el boton Abajo
		case 1:
			// Mueve el boton (22)
			OPTMCONTROL.counter --;
			// Completado el movimiento, siguiente paso
			if (OPTMCONTROL.counter <= 0) {
				OPTMCONTROL.counter = 0;
				exit = true;
			}
			if (OPTMCONTROL.lefthanded) { 
				OPTM[2].fx += 1024;
			} else {
				OPTM[2].fx -= 1024;
			}
			if (OPTM[2].fx < 0) {
				OPTM[2].x = -((-OPTM[2].fx) >> 8);
			} else {
				OPTM[2].x = (OPTM[2].fx >> 8);
			}
			if (OPTM[2].fy < 0) {
				OPTM[2].y = -((-OPTM[2].fy) >> 8);
			} else {
				OPTM[2].y = (OPTM[2].fy >> 8);
			}
			NF_Move3dSprite(22, OPTM[2].x, OPTM[2].y);
			break;

		// Salida de los botones
		case 2:
			// Calcula la distancia de los botones
			value = ((OPTMCONTROL.counter - (32 << 8)) >> 3);
			if (value < 256) value = 256;
			OPTMCONTROL.counter += value;
			// Verifica si es el fin del movimiento...
			if (OPTMCONTROL.counter >= (192 << 8)) {
				OPTMCONTROL.counter = (192 << 8);
				exit = true;
			} else {
				// Si no, mueve los botones
				for (n = 0; n < 2; n ++) {
					if (OPTMCONTROL.lefthanded) { 
						OPTM[(n + 8)].fy -= value;
					} else {
						OPTM[(n + 8)].fy += value;
					}
					if (OPTM[(n + 8)].fx < 0) {
						OPTM[(n + 8)].x = -((-OPTM[(n + 8)].fx) >> 8);
					} else {
						OPTM[(n + 8)].x = (OPTM[(n + 8)].fx >> 8);
					}
					if (OPTM[(n + 8)].fy < 0) {
						OPTM[(n + 8)].y = -((-OPTM[(n + 8)].fy) >> 8);
					} else {
						OPTM[(n + 8)].y = (OPTM[(n + 8)].fy >> 8);
					}
					NF_Move3dSprite((n + 14), OPTM[(n + 8)].x, OPTM[(n + 8)].y);
				}
			}

			break;



		// Error trap
		default:
			break;

	}

	// Reinicia la posicion de los botones si se ha completado la animacion
	if (exit) {
		if (OPTMCONTROL.lefthanded) {
			OPTM[8].fx = (84 << 8); OPTM[8].fy = -(128 << 8);
			OPTM[9].fx = (140 << 8); OPTM[9].fy = -(128 << 8);
		} else {
			OPTM[8].fx = (140 << 8); OPTM[8].fy = (192 << 8);
			OPTM[9].fx = (84 << 8); OPTM[9].fy = (192 << 8);
		}
		for (n = 0; n < 2; n ++) {
			if (OPTM[(n + 8)].fx < 0) {
				OPTM[(n + 8)].x = -((-OPTM[(n + 8)].fx) >> 8);
			} else {
				OPTM[(n + 8)].x = (OPTM[(n + 8)].fx >> 8);
			}
			if (OPTM[(n + 8)].fy < 0) {
				OPTM[(n + 8)].y = -((-OPTM[(n + 8)].fy) >> 8);
			} else {
				OPTM[(n + 8)].y = (OPTM[(n + 8)].fy >> 8);
			}
			NF_Move3dSprite((n + 14), OPTM[(n + 8)].x, OPTM[(n + 8)].y);
		}
	}

	// Devuelve el resultado
	return exit;

}



// Funcion GetOrientationAtBoot();
void GetOrientationAtBoot(void) {

	//////////////////////////
	//						//
	//	Inicializaciones	//
	//						//
	//////////////////////////

	// Resetealo todo
	ResetAll();


	// Variables locales

	s32 bg_pos = 0;				// Posicion del fondo

	bool fade_in = true;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	bool loop = true;			// Control del bucle principal

	bool lefthanded = GAMEOPTIONS[0].lefthanded;		// Copia de LEFTHANDED
	bool touch_down = false;

	u8 n = 0;			// Uso general
	char file[128];		// Nombre de archivos
	char lang[4];		// Idioma

	s16 pos_x[2];		// Posicion de los botones
	s16 pos_y[2];

	u8 id = 255;		// ID del boton tocado

	// Habilita el 3D en la pantalla inferior
	Set3DinTouchScreen();



	//////////////////////////
	//						//
	//	Carga los archivos	//
	//						//
	//////////////////////////

	// Idioma
	if (GAMEOPTIONS[0].language) {
		sprintf(lang, "en");
	} else {
		sprintf(lang, "es");
	}

	// Carga los fondos
	NF_LoadTiledBg("optmenu/bg/optm_bg3", "bg3", 512, 512);
	NF_LoadTiledBg("mainmenu/bg/mmtitle", "bg2", 256, 256);

	// Carga lo botones del cambio de orientacion (0 - 1)
	for (n = 0; n < 2; n ++) {
		sprintf(file, "optmenu/sprite/optm_button_%02d_%s", (n + 21), lang);
		NF_LoadSpriteGfx(file, n, 32, 128);
		NF_LoadSpritePal(file, n);
	}

	// Si es para zurdos...
	if (lefthanded) {
		// Rota los fondos
		RotateTiledBg("bg3");
		RotateTiledBg("bg2");
		// Rota los botones
		Rotate3dSprite(0);
		Rotate3dSprite(1);
	}


	//////////////////////////
	//						//
	//	Crea el menu		//
	//						//
	//////////////////////////

	// Carga la posicion de los botones
	if (lefthanded) {
		pos_x[0] = 84; pos_y[0] = 32;
		pos_x[1] = 140; pos_y[1] = 32;
	} else {
		pos_x[0] = 140; pos_y[0] = 32;
		pos_x[1] = 84; pos_y[1] = 32;
	}

	// Crea los fondos
	NF_CreateTiledBg(0, 3, "bg3");
	NF_CreateTiledBg(1, 3, "bg3");
	NF_CreateTiledBg(1, 2, "bg2");

	// Ajusta los fondos si es zurdo
	if (lefthanded) NF_ScrollBg(1, 1, 0, 64);

	// Crea los botones del menu principal (20 - 23)
	for (n = 0; n < 2; n ++) {
		NF_Vram3dSpriteGfx(n, n, true);
		NF_Vram3dSpritePal(n, n);
		NF_Create3dSprite(n, n, n, pos_x[n], pos_y[n]);
	}

	// Dibujado en pantalla y sincronismo
	NF_Draw3dSprites();
	glFlush(0);
	swiWaitForVBlank();


	//////////////////////////
	//						//
	//	Nucleo del Menu		//
	//						//
	//////////////////////////

	// Bucle principal del menu
	while (loop) {

		// Lee el touchpad
		ReadTouchpad();

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
			// Ajusta el brillo
			setBrightness(3, (0 - (fade >> 8)));
		}

		// Calcula el movimiento de los fondos
		if (lefthanded) {
			bg_pos -= 128;
			if (bg_pos < 0) bg_pos += (128 << 8);
		} else {
			bg_pos += 128;
			if (bg_pos > (128 << 8)) bg_pos -= (128 << 8);
		}


		// Segun que boton pulses del boton...
		if (!fade_in && !fade_out && loop) {
			// Si tocas el touchpad
			if (TOUCHPAD.held) {
				// Si es la primera pulsacion...
				if (!touch_down) {
					id = 255;
					// Verifica si se ha pulsado algun boton
					for (n = 0; n < 2; n ++) {
						if (
							(TOUCHPAD.pos_x >= pos_x[n])
							&&
							(TOUCHPAD.pos_x <= (pos_x[n] + 32))
							&&
							(TOUCHPAD.pos_y >= pos_y[n])
							&&
							(TOUCHPAD.pos_y <= (pos_y[n] + 128))
							) {
							id = n;
						}
					}
					// Si se ha pulsado un boton, registralo
					if (id != 255) {
						GAMEOPTIONS[0].lefthanded = id;
						fade_out = true;
					}

				} 
				// Registra la pulsacion
				touch_down = true;
			} else {
				// Registra que se ha levantado el stylus
				touch_down = false;
			}
		}

		// Dibujado en pantalla y sincronismo
		NF_Draw3dSprites();
		glFlush(0);
		swiWaitForVBlank();

		// Mueve las capas de fondo
		NF_ScrollBg(0, 3, (bg_pos >> 8), (bg_pos >> 8));
		NF_ScrollBg(1, 3, (bg_pos >> 8), (bg_pos >> 8));

	}

}
