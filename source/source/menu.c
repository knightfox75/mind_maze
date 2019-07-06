#ifdef __cplusplus
extern "C" {
#endif





	// Mind Maze - Funciones del menu principal
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





	// Funcion Main Menu
	u8 MainMenu(void) {

		//////////////////////////
		//						//
		//	Inicializaciones	//
		//						//
		//////////////////////////

		// Resetealo todo
		ResetAll();


		// Variables locales

		s32 bgx = 0;				// Eje X
		s32 bgy = 0;				// Eje Y

		bool fade_in = true;		// Fade IN
		bool fade_out = false;		// Fade OUT
		s32 fade = (16 << 8);		// Valor del FADE

		bool loop = true;			// Control del bucle principal

		s32 temp = 0;				// Uso general...

		// Inicializa las variables
		MENU.first_pos = 0;		// Posicion al tocar la pantalla
		MENU.id = 255;			// Id actual
		MENU.id_enable = false;	// Habilita la validacion de ID
		MENU.inertia = false;	// Flag de inercia abajo
		MENU.last_pos = 0;		// Ultima posicion del Stylus
		MENU.move = 0;			// Factor de desplazamiento
		MENU.pos = 0;			// Posicion actual
		MENU.timer = 0;			// Tiempo de stylus en pantalla
		MENU.touch = false;		// Flag de stylus tocando la pantalla


		//////////////////////
		//					//
		//  Carga Archivos	//
		//					//
		//////////////////////

		// Carga los archivos necesarios
		LoadMainMenuFiles();
		// Carga la musica del menu si no esta sonando ninguna
		if (SOUNDTRACK.track_id == -1) LoadBgm(MOD_MAIN);


		//////////////////////
		//					//
		//	Crea el menu	//
		//					//
		//////////////////////

		// Habilita el 3D en la pantalla inferior
		Set3DinTouchScreen();

		// Crea el menu
		CreateMainMenu();

		// Reproduce la musica del menu si no esta sonando ninguna
		if (!SOUNDTRACK.playback) PlayBgm();


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
					if (MENU.id == 3) StopBgm();
				}
				// Ajusta el brillo
				setBrightness(3, (0 - (fade >> 8)));
				// Si es necesario, ajusta el volumen
				if ((MENU.id == 3) || (MENU.id == 4)) {
					// Ajusta el volumen
					BgmVolume(((4096 - fade) >> 2));
				}
			}

			// Calcula el movimiento de los fondos
			if (GAMEOPTIONS[0].lefthanded) {
				bgx -= 128;
				if (bgx < 0) bgx += (128 << 8);
				bgy -= 128;
				if (bgy < 0) bgy += (272 << 8);
			} else {
				bgx += 128;
				if (bgx > (128 << 8)) bgx -= (128 << 8);
				bgy += 128;
				if (bgy > (272 << 8)) bgy -= (272 << 8);
			}

			// Si no hay ningun efecto pendiente...
			if (!fade_in && !fade_out && loop) {

				// El Stylus toca la pantalla ?
				if (TOUCHPAD.held) {
					// Si ya estavas sobre la pantalla
					if (MENU.touch) {
						// Suma 1 al contador de tiempo
						MENU.timer ++;
						// Almacena la posicion anterior del stlus
						MENU.last_pos = MENU.pos;
						// Y ahora guarda la posicion actual
						MENU.pos = TOUCHPAD.pos_y;
						MENU.height = TOUCHPAD.pos_x;
						// Para terminar, calcula el diferencial de movimiento
						temp = (MENU.pos - MENU.last_pos);
						if (temp < 0) {
							MENU.move = -temp;
							if (MENU.move > MENU_MAXMOVE) MENU.move = MENU_MAXMOVE;
							if (MENU.move < MENU_MINMOVE) MENU.move = 0;
							MENU.move = (MENU.move << 8);
							MENU.move = -MENU.move;
						} else {
							if (temp > MENU_MAXMOVE) temp = MENU_MAXMOVE;
							if (temp < MENU_MINMOVE) temp = 0;
							MENU.move = (temp << 8);
						}
						// Calcula si se debe deshabilitar la ejecucion
						temp = (MENU.first_pos - MENU.pos);
						if ((temp < -MENU_DEADZONE) || (temp > MENU_DEADZONE)) MENU.id_enable = false;
					} else {
						// Si no, registralo
						MENU.touch = true;
						MENU.first_pos = TOUCHPAD.pos_y;
						MENU.pos = TOUCHPAD.pos_y;
						// Debes deshabilitar el flag de inercia?
						if (MENU.inertia && (MENU.move == 0)) MENU.inertia = false;
						// Verifica si has tocado algun icono
						MENU.id = MainMenuItemTouch(TOUCHPAD.pos_x, TOUCHPAD.pos_y);
						if (MENU.id != 255) {
							MENU.id_enable = true;
						} else {
							MENU.id_enable = false;
						}
					}
				} else {
					// Si estava sobre la pantalla, se ha levantado y estavas sobre un icono valido
					if ((MENU.id != 255) && (MENU.id == MainMenuItemTouch(MENU.height, MENU.pos)) && MENU.id_enable && !MENU.inertia) {
						// Si estas dentro del rango de desplazamiento
						temp = (MENU.first_pos - MENU.pos);
						if ((temp > -MENU_DEADZONE) && (temp < MENU_DEADZONE)) {
							// Si has estado sobre el icono almenos durante x frames
							if (MENU.timer >= CLICK_MINTIME) {
								// Sal del menu
								fade_out = true;
							}
						}

					}
					// Si se la levantado, registralo
					MENU.touch = false;
					MENU.timer = 0;
					MENU.id_enable = false;
				}

			}
				

			// Si se levanta el lapiz, calcula la inercia.
			if ((MENU.move != 0) && !MENU.touch) {
				if (MENU.move < 0) {
					temp = -MENU.move;
					temp -= (temp >> 5);
					if (temp < 64) temp = 0;
					MENU.move = -temp;
				} else {
					MENU.move -= (MENU.move >> 5);
					if (MENU.move < 64) MENU.move = 0;
				}
				// Registra que hay inercia
				MENU.inertia = true;
			}

			// Mueve el menu
			MainMenuMove(MENU.move);

			// Dibujado en pantalla y sincronismo
			NF_Draw3dSprites();
			glFlush(0);
			swiWaitForVBlank();

			// Mueve las capas de fondo
			NF_ScrollBg(0, 3, (bgx >> 8), (128 - (bgx >> 8)));
			NF_ScrollBg(1, 3, (bgx >> 8), (128 - (bgx >> 8)));
			NF_ScrollBg(0, 2, 0, (bgy >> 8));
			NF_ScrollBg(1, 2, 0, (bgy >> 8));

		}

		// Devuelve el ID del icono tocado
		return MENU.id;

	}



	// Funcion LoadMainMenuFiles();
	void LoadMainMenuFiles(void) {

		// Variables
		u8 n = 0;
		char file[128];

		// Carga los fondos
		NF_LoadTiledBg("mainmenu/bg/mmbg2", "bg2", 256, 512);
		NF_LoadTiledBg("mainmenu/bg/mmbg3", "bg3", 512, 512);
		NF_LoadTiledBg("mainmenu/bg/mmtitle", "title", 256, 256);

		// Carga los Sprites
		for (n = 0; n < MMITEMS; n ++) {
			sprintf(file, "mainmenu/sprite/mmico%02d", (n + 1));
			NF_LoadSpriteGfx(file, n, 64, 64);
			NF_LoadSpritePal(file, n);
		}

		// Si es para zurdos...
		if (GAMEOPTIONS[0].lefthanded) {
			// Rota los fondos
			RotateTiledBg("bg2");
			RotateTiledBg("bg3");
			RotateTiledBg("title");
			// Rota los sprites
			for (n = 0; n < MMITEMS; n ++) {
				Rotate3dSprite(n);
			}
		}

	}



	// Funcion CreateMainMenu();
	void CreateMainMenu(void) {

		// Crea los fondos
		NF_CreateTiledBg(0, 3, "bg3");
		NF_CreateTiledBg(0, 2, "bg2");
		NF_CreateTiledBg(1, 3, "bg3");
		NF_CreateTiledBg(1, 2, "bg2");
		NF_CreateTiledBg(1, 1, "title");

		// Ajusta los fondos si es zurdo
		if (GAMEOPTIONS[0].lefthanded) NF_ScrollBg(1, 1, 0, 64);

		// Crea los iconos del menu
		MainMenuSprites(0);

		// Dibujado en pantalla y sincronismo
		NF_Draw3dSprites();
		glFlush(0);
		swiWaitForVBlank();

	}



	// Funcion MainMenuSprites();
	void MainMenuSprites(u8 id) {

		// Variables
		s32 temp = 0;		// Temporal
		s32 value = 0;		// Temporal
		s8 gfx = 0;			// ID del grafico
		s8 n = 0;			// Uso general
		s16 x = 0;			
		s16 y = 0;
		u8 last = (MMITEMS - 1);	// Ultimo ID

		// Calcula el Gfx del Sprite central
		MMROLLER[2].y = (96 - 32);
		MMROLLER[2].fy = (MMROLLER[2].y << 8);
		MMROLLER[2].gfx = id;

		// Calcula los Gfx de los sprites a la derecha
		gfx = id;
		temp = MMROLLER[2].y;
		for (n = 3; n <= 4; n ++) {
			// Grafico
			if (GAMEOPTIONS[0].lefthanded) {
				gfx --;
				if (gfx < 0) gfx = last;
			} else {
				gfx ++;
				if (gfx > last) gfx = 0;
			}
			MMROLLER[n].gfx = gfx;
			// Posicion
			temp += 80;
			MMROLLER[n].y = temp;
			MMROLLER[n].fy = (temp << 8);
		}

		// Calcula los Gfx de los sprites a la izquierda
		gfx = id;
		temp = MMROLLER[2].y;
		for (n = 1; n >= 0; n --) {
			// Graficos
			if (GAMEOPTIONS[0].lefthanded) {
				gfx ++;
				if (gfx > last) gfx = 0;
			} else {
				gfx --;
				if (gfx < 0) gfx = last;
			}
			MMROLLER[n].gfx = gfx;
			// Posicion
			temp -= 80;
			MMROLLER[n].y = temp;
			if (temp < 0) {
				value = -temp;
				MMROLLER[n].fy = (value << 8);
				MMROLLER[n].fy = -MMROLLER[n].fy;
			} else {
				MMROLLER[n].fy = (temp << 8);
			}
		}

		// Crea todos los sprites
		x = 96;
		for (n = 0; n < 5; n ++) {
			// Transfiere los graficos a la VRAM
			NF_Vram3dSpriteGfx(MMROLLER[n].gfx, n, false);
			NF_Vram3dSpritePal(MMROLLER[n].gfx, n);
			// Calcula la posicion
			y = MMROLLER[n].y;
			if (y < -72) y = -72;
			if (y > 200) y = 200;
			NF_Create3dSprite(n, n, n, x, y);
		}
		
	}



	// Funcion MainMenuMove();
	void MainMenuMove(s32 move) {

		// Variables
		u8 n = 0;		// Uso general
		s32 temp = 0;	// Valor temporal
		s16 x = 96;		// Coordenadas
		s16 y = 0;
		u8 id = 255;	// ID de Sprite a cambiar
		s8 gfx = 0;		// GFX a usar
		s16 blend = 0;	// Transparencia
		s16 zoom = 0;	// Zoom
		u8 last = (MMITEMS - 1);	// Ultimo ID

		// Mueve los sprites
		for (n = 0; n < 5; n ++) {
			// Calcula el movimiento y posiciona el sprite
			MMROLLER[n].fy += move;
			if (MMROLLER[n].fy < 0) {
				temp = -MMROLLER[n].fy;
				MMROLLER[n].y = (temp >> 8);
				MMROLLER[n].y = -MMROLLER[n].y;
			} else {
				MMROLLER[n].y = (MMROLLER[n].fy >> 8);
			}
			y = MMROLLER[n].y;
			if (y < -72) y = -72;
			if (y > 200) y = 200;
			NF_Move3dSprite(n, x, y);
			// Calcula la transparencia
			blend = 31;
			if ((y >= -64) && (y <= 0)) {
				blend = ((64 + y) >> 1);
				if (blend < 1) blend = 1;
				if (blend > 31) blend = 31;
			}
			if ((y <= 192) && (y >= 128)) {
				blend = ((192 - y) >> 1);
				if (blend < 1) blend = 1;
				if (blend > 31) blend = 31;
			}
			// Aplica la transperencia
			NF_Blend3dSprite(n, (n + 1), blend);
			// Zoom
			zoom = 64;
			if (y >= 0 && y <= 64) {
				zoom = (64 + (y >> 2));
			}
			if ((y > 64) && (y <= 128)) {
				zoom = (64 + ((128 - y) >> 2));
			}
			if (y < 0) {
				temp = (64 + y);
				zoom = 32 + (temp >> 1);
				if (zoom < 0) zoom = 0;
			}
			if (y > 128) {
				temp = (64 - (y - 128));
				zoom = 32 + (temp >> 1);
				if (zoom < 0) zoom = 0;
			}
			// Aplica el escalado
			NF_Scale3dSprite(n, zoom, zoom);
			// Analiza si es el final de movimiento, segun el sentido
			if ((MMROLLER[n].y >= 304) || (MMROLLER[n].y <= -176)) id = n;
		}

		// Si has llegado al final, indicalo y haz el cambio de sprites
		if (id != 255) {
			// Borra el sprite y gfx del elemento antiguo
			NF_Delete3dSprite(id);
			NF_Free3dSpriteGfx(id);
			gfx = MMROLLER[id].gfx;
			if (move > 0) {
				// Reposicion los sprites
				MMROLLER[id].y -= 400;
				// Calcula el sprite nuevo
				if (GAMEOPTIONS[0].lefthanded) {
					for (n = 0; n < 5; n ++) {
						gfx ++;
						if (gfx > last) gfx = 0;
					}
				} else {
					for (n = 0; n < 5; n ++) {
						gfx --;
						if (gfx < 0) gfx = last;
					}
				}
			} else {
				// Reposicion los sprites
				MMROLLER[id].y += 400;
				// Calcula el sprite nuevo
				if (GAMEOPTIONS[0].lefthanded) {
					for (n = 0; n < 5; n ++) {
						gfx --;
						if (gfx < 0) gfx = last;
					}
				} else {
					for (n = 0; n < 5; n ++) {
						gfx ++;
						if (gfx > last) gfx = 0;
					}
				}
			}
			// Actualiza la coordenada en Fixed point del Sprite
			if (MMROLLER[id].y < 0) {
				temp = -MMROLLER[id].y;
				MMROLLER[id].fy = (temp << 8);
				MMROLLER[id].fy = -MMROLLER[id].fy;
			} else {
				MMROLLER[id].fy = (MMROLLER[id].y << 8);
			}
			// Manda el nuevo GFX a la VRAM y crea el Sprite
			MMROLLER[id].gfx = gfx;
			NF_Vram3dSpriteGfx(gfx, id, false);
			NF_Vram3dSpritePal(gfx, id);
			// Calcula la posicion
			y = MMROLLER[id].y;
			if (y < -72) y = -72;
			if (y > 200) y = 200;
			NF_Create3dSprite(id, id, id, x, MMROLLER[id].y);
		}

	}



	// Funcion MainMenuItemTouch();
	u8 MainMenuItemTouch(s16 x, s16 y) {

		// Variables
		u8 n = 0;
		u8 id = 255;

		// Define zona de util
		if ((x >= 96) && (x <= 160) && (y >= 8) && (y <= 184)) {
			// Comprueba si se ha tocado algun sprite
			for (n = 0; n < 5; n ++) {
				if ((MMROLLER[n].y > 0) && (y >= MMROLLER[n].y) && (y <= (MMROLLER[n].y + 64)) && (MMROLLER[n].y < 132)) {
					// Guarda el ID
					id = MMROLLER[n].gfx;
				}
			}
		}

		// Devuelve el ID del sprite tocado, 255 si no se ha tocado ninguno
		return id;

	}





#ifdef __cplusplus
}
#endif
