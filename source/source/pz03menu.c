
// Mind Maze - Funciones del Puzzle 3 - Laberinto (Menu)
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



// Funcion MenuPuzzle03();
u8 MenuPuzzle03(void) {

	//////////////////////////
	//						//
	//	Inicializaciones	//
	//						//
	//////////////////////////

	// Resetealo todo
	ResetAll();


	// Variables locales

	s32 bgy = 0;				// Eje Y

	bool fade_in = true;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	bool loop = true;			// Control del bucle principal

	s32 text_x = 0;				// Posicion de la capa de texto
	s32 text_y = 0;
	s16 text_speed = 0;			// Velocidad del texto
	s16 text_time = 120;		// Retardo en el rebote
	s8 text_page = 0;			// Pagina actual
	s8 text_id = 255;			// Control del cambio de ID
	s16 text_low = 0;			// Limite inferior de la capa
	s16 text_high = 0;			// Limite superior de la capa
	bool text_force = false;	// Fuerza la actualizacion de la capa de texto

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
	LoadPuzzle03MenuFiles();
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
	CreatePuzzle03Menu();

	// Textos iniciales del menu
	if (SAVEDATA[2].locked[0]) {
		text_page = 1;
	} else {
		text_page = 0;
	}
	Puzzle03MenuText(0, text_page);
	if (GAMEOPTIONS[0].lefthanded) {
		text_low = 8;
		text_high = 248,
		text_x = (8 << 8);
		text_y = 64;
		text_speed = 128;	
	} else {
		text_low = 0;
		text_high = 240,
		text_x = (240 << 8);
		text_y = 0;
		text_speed = -128;	
	}

	// Reproduce la musica del menu si no esta sonando ninguna
	if (!SOUNDTRACK.playback) PlayBgm();


	//////////////////////////
	//						//
	//	Nucleo del Menu		//
	//						//
	//////////////////////////

	// Bucle principal del menu
	while (loop) {

		// Variables
		text_force = false;		// Flag abajo de la actualizacion forzada de textos

		// Lee el touchpad y teclado
		ReadKeypad();
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
				if (MENU.id != 255) StopBgm();
			}
			// Ajusta el brillo
			setBrightness(3, (0 - (fade >> 8)));
			// Si es necesario, ajusta el volumen
			if (MENU.id != 255) {
				// Ajusta el volumen
				BgmVolume(((4096 - fade) >> 2));
			}
		}

		// Calcula el movimiento de los fondos
		if (GAMEOPTIONS[0].lefthanded) {
			bgy -= 128;
			if (bgy < 0) bgy += (128 << 8);
		} else {
			bgy += 128;
			if (bgy > (128 << 8)) bgy -= (128 << 8);
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
					MENU.id = Puzzle03MenuItemTouch(TOUCHPAD.pos_x, TOUCHPAD.pos_y);
					if (MENU.id != 255) {
						MENU.id_enable = true;
					} else {
						MENU.id_enable = false;
					}
				}
			} else {
				// Si estava sobre la pantalla, se ha levantado y estavas sobre un icono valido
				if ((MENU.id != 255) && (MENU.id == Puzzle03MenuItemTouch(MENU.height, MENU.pos)) && MENU.id_enable && !MENU.inertia) {
					// Si estas dentro del rango de desplazamiento
					temp = (MENU.first_pos - MENU.pos);
					if ((temp > -MENU_DEADZONE) && (temp < MENU_DEADZONE)) {
						// Si has estado sobre el icono almenos durante x frames
						if (MENU.timer >= CLICK_MINTIME) {
							// Si el icono tocado esta comprado...
							if (!SAVEDATA[2].locked[MENU.id]) {
								// Sal del menu y devuelve la ID del icono tocado
								fade_out = true;
							} else {
								// Si no, intenta comprarlo
								text_force = Puzzle03Buy(MENU.id);
							}
						}
					}

				}
				// Si se la levantado, registralo
				MENU.touch = false;
				MENU.timer = 0;
				MENU.id_enable = false;
			}

			// Si se debe mostrar la informacion de otro puzzle?
			if (((text_id != MENU.id) && (MENU.id != 255)) || text_force) {
				text_id = MENU.id;
				text_page = 0;
				text_time = 120;
				MENU.id_enable = false;
				if (GAMEOPTIONS[0].lefthanded) {
					text_x = (text_low << 8);
					text_y = 64;
					text_speed = 128;	
				} else {
					text_x = (text_high << 8);
					text_y = 0;
					text_speed = -128;	
				}
				if (SAVEDATA[2].locked[text_id]) {
					text_page = 1;
				} else {
					text_page = 0;
				}
				Puzzle03MenuText(text_id, text_page);
			}

			// Calcula el scroll de la capa de texto
			if ((text_time > 0) && (text_page == 0)) text_time --;
			if (text_time == 0) {
				text_x += text_speed;
				if (text_x <= (text_low << 8)) {
					text_x = (text_low << 8);
					text_speed = -text_speed;
					text_time = 120;
				}
				if (text_x >= (text_high << 8)) {
					text_x = (text_high << 8);
					text_speed = -text_speed;
					text_time = 120;
				}
			}

		}

		// Si se levanta el lapiz, calcula la inercia.
		if ((MENU.move != 0) && (!MENU.touch)) {
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
		Puzzle03MenuMove(MENU.move);

		// Abortar? (Salir del menu)
		if (!fade_in && !fade_out && AbortKeys()) {
			fade_out = true;
			MENU.id = 255;
		}

		// Dibujado en pantalla y sincronismo
		NF_Draw3dSprites();
		glFlush(0);
		swiWaitForVBlank();

		// Mueve las capas de fondo
		NF_ScrollBg(0, 3, 0, (bgy >> 8));
		NF_ScrollBg(1, 3, 0, (bgy >> 8));

		// Mueve la capa de texto
		NF_ScrollBg(1, 2, (text_x >> 8), text_y);

	}

	// Devuelve el ID del icono tocado
	return MENU.id;

}



// Funcion LoadPuzzle03MenuFiles();
void LoadPuzzle03MenuFiles(void) {

	// Variables
	u8 n = 0;
	char file[128];

	// Carga los fondos
	NF_LoadTiledBg("puzz03/bg/menu/p03mbg03", "bg3", 256, 512);
	NF_LoadTiledBg("puzz03/bg/menu/p03mbg02", "bg2", 256, 256);

	// Carga los Sprites
	for (n = 0; n < P03_ITEMS; n ++) {
		sprintf(file, "puzz03/sprite/menu/p03ico%02d", (n + 1));
		NF_LoadSpriteGfx(file, n, 128, 128);
		NF_LoadSpritePal(file, n);
	}

	// Carga las fuentes
	if (GAMEOPTIONS[0].lefthanded) {
		n = 2;
	} else {
		n = 1;
	}
	NF_LoadTextFont16("font/8x16", "text", 512, 256, n);

	// Rota los graficos si es necesario
	if (GAMEOPTIONS[0].lefthanded) {
		// Rota los fondos
		RotateTiledBg("bg2");
		RotateTiledBg("bg3");
		// Rota los sprites
		for (n = 0; n < P03_ITEMS; n ++) {
			Rotate3dSprite(n);
		}
	}

}



// Funcion CreatePuzzle03Menu();
void CreatePuzzle03Menu(void) {

	// Crea los fondos del menu
	NF_CreateTiledBg(0, 3, "bg3");
	NF_CreateTiledBg(1, 3, "bg3");
	NF_CreateTiledBg(0, 2, "bg2");
	NF_CreateTiledBg(1, 1, "bg2");

	// Crea la capa de texto
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTextLayer16(1, 2, 2, "text");
	} else {
		NF_CreateTextLayer16(1, 2, 1, "text");
	}
	NF_DefineTextColor(1, 2, 1, 31, 31, 31);
	NF_SetTextColor(1, 2, 1);

	// Ajusta la posicion de los fondos
	if (GAMEOPTIONS[0].lefthanded) {
		// Ajusta la posicion
		NF_ScrollBg(0, 2, 0, 64);
		NF_ScrollBg(1, 2, 0, 64);
		NF_ScrollBg(1, 1, 0, 64);
	} else {
		NF_ScrollBg(1, 2, 256, 0);
	}

	// Crea los iconos del menu
	Puzzle03MenuSprites(0);

	// Ajusta la posicion de la capa 3D
	NF_3dSpritesLayer(2);

	// Actualiza el contenido de la pantalla
	NF_Draw3dSprites();
	glFlush(0);
	swiWaitForVBlank();

}



// Funcion Puzzle03MenuSprites();
void Puzzle03MenuSprites(u8 id) {

	// Variables
	s32 temp = 0;		// Temporal
	s32 value = 0;		// Temporal
	s8 gfx = 0;			// ID del grafico
	s8 n = 0;			// Uso general
	s16 x = 0;			
	s16 y = 0;
	u8 last = (P03_ITEMS - 1);	// Ultimo ID

	// Calcula el Gfx del Sprite central
	P03MROLLER[2].y = (96 - 48);
	P03MROLLER[2].fy = (P03MROLLER[2].y << 8);
	P03MROLLER[2].gfx = id;

	// Calcula los Gfx de los sprites a la derecha
	gfx = id;
	temp = P03MROLLER[2].y;
	for (n = 3; n <= 4; n ++) {
		// Grafico
		if (GAMEOPTIONS[0].lefthanded) {
			gfx --;
			if (gfx < 0) gfx = last;
		} else {
			gfx ++;
			if (gfx > last) gfx = 0;
		}
		P03MROLLER[n].gfx = gfx;
		// Posicion
		temp += 112;
		P03MROLLER[n].y = temp;
		P03MROLLER[n].fy = (temp << 8);
	}

	// Calcula los Gfx de los sprites a la izquierda
	gfx = id;
	temp = P03MROLLER[2].y;
	for (n = 1; n >= 0; n --) {
		// Graficos
		if (GAMEOPTIONS[0].lefthanded) {
			gfx ++;
			if (gfx > last) gfx = 0;
		} else {
			gfx --;
			if (gfx < 0) gfx = last;
		}
		P03MROLLER[n].gfx = gfx;
		// Posicion
		temp -= 112;
		P03MROLLER[n].y = temp;
		if (temp < 0) {
			value = -temp;
			P03MROLLER[n].fy = (value << 8);
			P03MROLLER[n].fy = -P03MROLLER[n].fy;
		} else {
			P03MROLLER[n].fy = (temp << 8);
		}
	}

	// Crea todos los sprites
	x = 64;
	for (n = 0; n < 5; n ++) {
		// Transfiere los graficos a la VRAM
		NF_Vram3dSpriteGfx(P03MROLLER[n].gfx, n, false);
		NF_Vram3dSpritePal(P03MROLLER[n].gfx, n);
		// Desatura las paletas de los puzzles bloqueados
		if (SAVEDATA[2].locked[P03MROLLER[n].gfx]) {
			Desaturate3dSprite(n, 10);
			P03MROLLER[n].bn = true;
		} else {
			P03MROLLER[n].bn = false;
		}
		// Calcula la posicion
		y = P03MROLLER[n].y;
		if (y < -104) y = -104;
		if (y > 200) y = 200;
		NF_Create3dSprite(n, n, n, x, (y - 16));
	}
	
}



// Funcion Puzzle03MenuMove();
void Puzzle03MenuMove(s32 move) {

	// Variables
	u8 n = 0;			// Uso general
	s32 temp = 0;		// Valor temporal
	s16 x = 64;			// Coordenadas
	s16 y = 0;
	u8 id = 255;		// ID de Sprite a cambiar
	s8 gfx = 0;			// GFX a usar
	s16 blend = 0;		// Transparencia
	u8 last = (P03_ITEMS - 1);		// Ultimo ID

	// Mueve los sprites
	for (n = 0; n < 5; n ++) {
		// Calcula el movimiento y posiciona el sprite
		P03MROLLER[n].fy += move;
		if (P03MROLLER[n].fy < 0) {
			temp = -P03MROLLER[n].fy;
			P03MROLLER[n].y = (temp >> 8);
			P03MROLLER[n].y = -P03MROLLER[n].y;
		} else {
			P03MROLLER[n].y = (P03MROLLER[n].fy >> 8);
		}
		y = P03MROLLER[n].y;
		if (y < -104) y = -104;
		if (y > 200) y = 200;
		NF_Move3dSprite(n, x, (y - 16));
		// Calcula la transparencia
		blend = 31;
		if ((y >= -96) && (y <= -32)) {
			blend = ((96 + y) >> 1);
			if (blend < 1) blend = 1;
			if (blend > 31) blend = 31;
		}
		if ((y <= 196) && (y >= 132)) {
			blend = ((196 - y) >> 1);
			if (blend < 1) blend = 1;
			if (blend > 31) blend = 31;
		}
		// Aplica la transperencia
		NF_Blend3dSprite(n, (n + 1), blend);
		// Analiza si es el final de movimiento, segun el sentido
		if ((P03MROLLER[n].y >= 384) || (P03MROLLER[n].y <= -288)) id = n;
	}

	// Si has llegado al final, indicalo y haz el cambio de sprites
	if (id != 255) {
		// Borra el sprite y gfx del elemento antiguo
		NF_Delete3dSprite(id);
		NF_Free3dSpriteGfx(id);
		gfx = P03MROLLER[id].gfx;
		if (move > 0) {
			// Reposicion los sprites
			P03MROLLER[id].y -= 560;
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
			P03MROLLER[id].y += 560;
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
		if (P03MROLLER[id].y < 0) {
			temp = -P03MROLLER[id].y;
			P03MROLLER[id].fy = (temp << 8);
			P03MROLLER[id].fy = -P03MROLLER[id].fy;
		} else {
			P03MROLLER[id].fy = (P03MROLLER[id].y << 8);
		}
		// Manda el nuevo GFX a la VRAM y crea el Sprite
		P03MROLLER[id].gfx = gfx;
		NF_Vram3dSpriteGfx(gfx, id, false);
		NF_Vram3dSpritePal(gfx, id);
		// Desatura las paletas de los puzzles bloqueados
		if (SAVEDATA[2].locked[gfx]) { 
			Desaturate3dSprite(id, 10);
			P03MROLLER[id].bn = true;
		} else {
			P03MROLLER[id].bn = false;
		}
		// Calcula la posicion
		y = P03MROLLER[id].y;
		if (y < -104) y = -104;
		if (y > 200) y = 200;
		NF_Create3dSprite(id, id, id, x, (y - 16));
	}

}



// Funcion Puzzle03MenuItemTouch();
u8 Puzzle03MenuItemTouch(s16 x, s16 y) {

	// Variables
	u8 n = 0;
	u8 id = 255;

	// Define zona de util
	if ((x >= 64) && (x <= 192) && (y >= 8) && (y <= 184)) {
		// Comprueba si se ha tocado algun sprite
		for (n = 0; n < 5; n ++) {
			if ((P03MROLLER[n].y > 0) && (y >= P03MROLLER[n].y) && (y <= (P03MROLLER[n].y + 96)) && (P03MROLLER[n].y < 100)) {
				// Guarda el ID
				id = P03MROLLER[n].gfx;
			}
		}
	}

	// Devuelve el ID del sprite tocado, 255 si no se ha tocado ninguno
	return id;

}


// Funcion Puzzle03MenuText();
void Puzzle03MenuText(u8 id, u8 page) {

	// Variables
	char text[24];
	s16 min = 0;
	s16 sec = 0;

	// Borra la pantalla
	NF_ClearTextLayer16(1, 2);

	switch (page) {

		case 0:		// Pagina 1 (Datos del puzzle)
			min = (int)(SAVEDATA[2].time[id] / 60);
			sec = (SAVEDATA[2].time[id] - (min * 60));
			switch (GAMEOPTIONS[0].language) {
				case 0:		// Castellano
					sprintf(text, "%s", PUZZLE03INFO[id].name);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 2, text);
					sprintf(text, "por");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 3, text);
					sprintf(text, "%s", PUZZLE03INFO[id].author);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 4, text);
					sprintf(text, "Se ha jugado");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 6, text);
					sprintf(text, "%d veces", SAVEDATA[2].played[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 7, text);
					sprintf(text, "Se ha resuelto");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 9, text);
					sprintf(text, "%d veces", SAVEDATA[2].solved[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 10, text);
					sprintf(text, "Desbloqueado el");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 12, text);
					sprintf(text, "%02d/%02d/%04d", SAVEDATA[2].day[id], SAVEDATA[2].month[id], SAVEDATA[2].year[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 13, text);
					sprintf(text, "El mas eficiente:");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 15, text);
					sprintf(text, "%d movimientos", SAVEDATA[2].moves[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 16, text);
					sprintf(text, "Mejor tiempo");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 18, text);
					sprintf(text, "%02d:%02d", min, sec);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 19, text);
					sprintf(text, "Puntos disponibles");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 21, text);
					sprintf(text, "%d", SAVEDATA[2].credits);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 22, text);
					sprintf(text, "Puntos Acumulados");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 24, text);
					sprintf(text, "%d", SAVEDATA[2].points);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 25, text);
					sprintf(text, "Conseguidos con");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 27, text);
					sprintf(text, "este laberinto");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 28, text);
					sprintf(text, "%d", SAVEDATA[2].score[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 29, text);
					break;
				case 1:		// Ingles
					sprintf(text, "%s", PUZZLE03INFO[id].name);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 2, text);
					sprintf(text, "by");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 3, text);
					sprintf(text, "%s", PUZZLE03INFO[id].author);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 4, text);
					sprintf(text, "Played");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 6, text);
					sprintf(text, "%d times", SAVEDATA[2].played[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 7, text);
					sprintf(text, "Resolved");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 9, text);
					sprintf(text, "%d times", SAVEDATA[2].solved[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 10, text);
					sprintf(text, "Unlocked the");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 12, text);
					sprintf(text, "%02d/%02d/%04d", SAVEDATA[2].month[id], SAVEDATA[2].day[id], SAVEDATA[2].year[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 13, text);
					sprintf(text, "The most efficient:");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 15, text);
					sprintf(text, "%d moves", SAVEDATA[2].moves[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 16, text);
					sprintf(text, "Best time");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 18, text);
					sprintf(text, "%02d:%02d", min, sec);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 19, text);
					sprintf(text, "Available points");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 21, text);
					sprintf(text, "%d", SAVEDATA[2].credits);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 22, text);
					sprintf(text, "Accumulated points");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 24, text);
					sprintf(text, "%d", SAVEDATA[2].points);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 25, text);
					sprintf(text, "Achieved with");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 27, text);
					sprintf(text, "this labyrinth");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 28, text);
					sprintf(text, "%d", SAVEDATA[2].score[id]);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 29, text);
					break;
				default:
					break;
			}
			break;

		case 1:		// Pagina 2 (Compra el puzzle)
			switch (GAMEOPTIONS[0].language) {
				case 0:		// Castellano
					sprintf(text, "El laberinto");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 2, text);
					sprintf(text, "%s", PUZZLE03INFO[id].name);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 3, text);
					sprintf(text, "esta bloqueado");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 4, text);
					sprintf(text, "Puedes comprarlo");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 5, text);
					sprintf(text, "por %d", SAVEDATA[2].cost);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 6, text);
					sprintf(text, "puntos");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 7, text);
					sprintf(text, "En estos momentos");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 8, text);
					sprintf(text, "tienes %d", SAVEDATA[2].credits);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 9, text);
					sprintf(text, "puntos");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 10, text);
					sprintf(text, "Toca el icono");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 12, text);
					sprintf(text, "para comprar");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 13, text);
					sprintf(text, "el laberinto");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 14, text);
					break;
				case 1:		// Ingles
					sprintf(text, "The labyrinth");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 2, text);
					sprintf(text, "%s", PUZZLE03INFO[id].name);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 3, text);
					sprintf(text, "is locked");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 4, text);
					sprintf(text, "You can buy it");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 5, text);
					sprintf(text, "for %d", SAVEDATA[2].cost);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 6, text);
					sprintf(text, "points");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 7, text);
					sprintf(text, "You currently");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 8, text);
					sprintf(text, "have %d", SAVEDATA[2].credits);
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 9, text);
					sprintf(text, "points");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 10, text);
					sprintf(text, "Tap the icon");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 12, text);
					sprintf(text, "to buy");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 13, text);
					sprintf(text, "the labyrinth");
					NF_WriteText16(1, 2, (12 - (strlen(text) >> 1)), 14, text);
					break;
				default:
					break;
			}
			break;

		default:
			break;

	}

	// Actualiza las capas de texto
	NF_UpdateTextLayers();

}


// Funcion Puzzle03Buy();
bool Puzzle03Buy(u8 id) {

	// Variables
	bool result = false;
	u8 n = 0;

	// Si tienes suficientes creditos
	if (SAVEDATA[2].credits >= SAVEDATA[2].cost) {
		// Marcalo como comprado
		SAVEDATA[2].locked[id] = false;
		// Y descuenta el coste
		SAVEDATA[2].credits -= SAVEDATA[2].cost;
		// Incrementa el precio de los demas puzzles
		SAVEDATA[2].cost += 150;
		// Obten la fecha de desbloqueo
		GetCurrentDate(&SAVEDATA[2].day[id], &SAVEDATA[2].month[id], &SAVEDATA[2].year[id]);
		// Marca la compra como realizada
		result = true;
		// Pon el icono en color
		for (n = 0; n < 5; n ++) {
			// Si estas en blanco y negro y estas desbloqueado
			if (P03MROLLER[n].bn && !SAVEDATA[2].locked[P03MROLLER[n].gfx]) {
				NF_3dSpriteUpdatePalette(n);
				P03MROLLER[n].bn = false;
			}
		}
		// Y guarda los datos actualizados
		WriteSavegame();
	}

	// Devuelve el resultado
	return result;

}
