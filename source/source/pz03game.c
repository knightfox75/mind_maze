
// Mind Maze - Funciones del Puzzle 3 - Laberinto (Juego)
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



// Funcion KernelPuzzle03()
u8 KernelPuzzle03(u8 puzzle) {

	// Reinicialo todo
	ResetAll();

	// Variables
	u8 end = 0;			// Resultado del puzzle
	u8 option = 0;		// Opcion del menu de fin de puzzle
	bool loop = true;	// Loop


	//////////////////////
	//					//
	//  Carga Archivos	//
	//					//
	//////////////////////

	// Pantalla "Cargando..."
	LoadingIn();
	// Carga los archivos necesarios
	LoadPuzzle03Files(puzzle);
	// Carga la musica
	LoadBgm((MOD_P03STG01 + puzzle));
	// Oculta la pantalla "Cargando..."
	LoadingOut();


	//////////////////////
	//					//
	//	Crea el Puzzle	//
	//					//
	//////////////////////

	// Habilita el 3D en la pantalla tactil
	N3_Set3D(1, 0);
	N3_Init3dEngine();

	// Habilita los fondos tileados
	NF_InitTiledBgSys(0);

	// Y habilita la capa de dibujado para el 3D
	NF_ShowBg(0, 0);

	// Crea el puzzle
	CreatePuzzle03(puzzle);

	// Reproduce la musica
	PlayBgm();


	///////////////////////
	//				     //
	//	Juega al Puzzle	 //
	//					 //
	///////////////////////

	// Repite mientras no pidas otro puzzle o salir
	while (loop) {

		// Suma un contador al numero de veces jugadas
		SAVEDATA[2].played[puzzle] ++;

		// Juega al puzzle
		end = PlayPuzzle03(puzzle);

		// Al terminar el puzzle...
		EndPuzzle03(end);

		// Menu de puzzle finalizado
		option = EndMenuPuzzle03(end);

		// Salida del puzzle
		Puzzle03Exit(option);

		// Si has seleccionado repetir el mismo puzzle
		if (option == 0) {
			RetryPuzzle03(puzzle);
		} else {
			loop = false;
		}

	}

	// Devuelve el resultado de ejecucion
	// 0 - Repetir puzzle
	// 1 - Cambiar puzzle
	// 2 - Salir
	return option;

}



// Funcion PlayPuzzle03();
u8 PlayPuzzle03(u8 puzzle) {

	// Variables
	bool loop = true;		// Control de bucle de ejecucion
	u8 result = 0;			// Resultado de la ejecucion
	u8 tics = 0;			// Control del tiempo de juego
	u32 time = 0;			// Tiempo de juego
	u32 moves = 0;			// Movimientos realizados

	// Animacion inicial
	Puzzle03Start();

	// Bucle de ejecucion
	while (loop) {

		// Lee el teclado y pantalla tactil
		ReadKeypad();
		ReadTouchpad();

		// Si has pulsado la tecla de cancelar (Zurdos)
		if (AbortKeys()) {
			loop = false;
			result = 1;
		}

		// Movimiento de la bola con el stylus
		moves += Puzzle03MoveBall();

		// Has alcanzado la salida?
		if (Puzzle03CheckGoal()) {
			loop = false;
			result = 0;	// 0
		}

		// Calculo del tiempo de juego
		tics ++;
		if (tics > 59) {
			tics = 0;
			time ++;
		}

		// Renderiza todos los graficos
		Puzzle03GfxEngine();

	}

	// Si se ha resuelto el puzzle, calcula los resultados
	if (result == 0) Puzzle03Results(puzzle, time, moves);

	// Deten la bola
	P03WORLD.ball_touch = false;
	P03WORLD.ball_iner = false;
	P03WORLD.mov_x = 0;
	P03WORLD.mov_y = 0;

	// Devuelve el resultado
	return result;

}



// Funcion LoadPuzzle03Files();
void LoadPuzzle03Files(u8 puzzle) {

	// Variables
	char file[128];
	char lang[4];
	u8 stage = (puzzle + 1);
	u8 n = 0;
	u32 width = 0;
	u32 height = 0;

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

	// Carga la textura del techo (0)
	sprintf(file, "puzz03/tex/stg%02d/p03ceil%02d", stage, stage);
	N3_LoadTexture(file, 0, 16, 16);
	N3_LoadPal(file, 0);
	// Carga la textura del pared (1)
	sprintf(file, "puzz03/tex/stg%02d/p03wall%02d", stage, stage);
	N3_LoadTexture(file, 1, 16, 16);
	N3_LoadPal(file, 1);
	// Carga la textura del suelo (2)
	sprintf(file, "puzz03/tex/stg%02d/p03floor%02d", stage, stage);
	N3_LoadTexture(file, 2, 16, 16);
	N3_LoadPal(file, 2);
	// Carga la textura de la meta (3)
	sprintf(file, "puzz03/tex/common/p03goal");
	N3_LoadTexture(file, 3, 16, 16);
	N3_LoadPal(file, 3);
	// Carga las texturas de la bola y su paleta (31)
	sprintf(file, "puzz03/tex/common/p03ball");
	N3_LoadTexture(file, 31, 16, 16);
	N3_LoadPal(file, 31);
	// Carga los botones del menu
	for (n = 0; n < 3; n ++) {
		// Carga las texturas de los botones y sus paletas (28, 29 y 30)
		sprintf(file, "puzz03/tex/common/p03mb%d_%s", n, lang);
		N3_LoadTexture(file, (n + 28), 32, 128);
		N3_LoadPal(file, (n + 28));
	}

	// Carga los mapas
	sprintf(file, "puzz03/map/stg%02d/p03build%02d", stage, stage);
	NF_LoadColisionMap(file, 0, (PUZZLE03INFO[puzzle].width >> 1), (PUZZLE03INFO[puzzle].height >> 1));
	sprintf(file, "puzz03/map/stg%02d/p03map%02d", stage, stage);
	NF_LoadColisionMap(file, 1, PUZZLE03INFO[puzzle].width, PUZZLE03INFO[puzzle].height);

	// Carga los fondos del minimapa
	width = (((((PUZZLE03INFO[puzzle].width >> 1) - 1) >> 8) + 1) << 8);
	height = (((((PUZZLE03INFO[puzzle].height >> 1) - 1) >> 8) + 1) << 8);
	sprintf(file, "puzz03/bg/stg%02d/p03stg%02dbg3a", stage, stage);
	NF_LoadTiledBg(file, "bg3_main", width, height);
	sprintf(file, "puzz03/bg/stg%02d/p03stg%02dbg2a", stage, stage);
	NF_LoadTiledBg(file, "bg2_main", width, height);

	// Carga el fondo de la pantalla 3D
	sprintf(file, "puzz03/bg/stg%02d/p03stg%02dbg3b", stage, stage);
	NF_LoadTiledBg(file, "bg3_sub", 512, 512);

	// Carga los sprites 2D
	NF_LoadSpriteGfx("puzz03/sprite/common/miniball", 0, 16, 16);
	NF_LoadSpritePal("puzz03/sprite/common/miniball", 0);

	// Carga los fondos y fuentes para el report (comun a todos los puzzles)
	NF_LoadTiledBg("puzz01/bg/p01bg02main", "bg1_main", 256, 512);

	// Carga las fuentes
	if (GAMEOPTIONS[0].lefthanded) {
		n = 2;
	} else {
		n = 1;
	}
	NF_LoadTextFont16("font/8x16", "text", 256, 512, n);

	// Rota el material necesario, si es necesario
	if (GAMEOPTIONS[0].lefthanded) {

		// Rota los mapas
		RotateMap(0);
		RotateMap(1);

		// Rota los fondos
		RotateTiledBg("bg3_main");
		RotateTiledBg("bg1_main");
		RotateTiledBg("bg3_sub");

		// Rota las texturas
		for (n = 28; n <= 31; n ++) {
			RotateTexture(n);
		}

	}


}



// Funcion CreatePuzzle03();
void CreatePuzzle03(u8 puzzle) {

	// Variables
	u8 n = 0;

	/*
	Atencion!
	El motor 3D de DS usa como unidad el numero 16:1
	Esto significa que un pixel real equivale a 16 unidades
	Por lo que todas las coordenadas reales se conviertes
	con el siguiente bitshift (1 << 4)
	*/

	// Inicializa los datos del mundo
	if (GAMEOPTIONS[0].lefthanded) {
		P03WORLD.ball_x = (PUZZLE03INFO[puzzle].start_x[1] << 4);	// Posicion de la bola zurdos
		P03WORLD.ball_y = (PUZZLE03INFO[puzzle].start_y[1] << 4);
		P03WORLD.ball_fx = (PUZZLE03INFO[puzzle].start_x[1] << 8);	// Posicion de la bola zurdos (Fixed << 8)
		P03WORLD.ball_fy = (PUZZLE03INFO[puzzle].start_y[1] << 8);
		P03WORLD.background = (256 << 8);
	} else {
		P03WORLD.ball_x = (PUZZLE03INFO[puzzle].start_x[0] << 4);	// Posicion de la bola diestros
		P03WORLD.ball_y = (PUZZLE03INFO[puzzle].start_y[0] << 4);
		P03WORLD.ball_fx = (PUZZLE03INFO[puzzle].start_x[0] << 8);	// Posicion de la bola diestros (Fixed << 8)
		P03WORLD.ball_fy = (PUZZLE03INFO[puzzle].start_y[0] << 8);
		P03WORLD.background = (128 << 8);
	}
	P03WORLD.ball_z = 0;			// Altura de la bola
	P03WORLD.ball_fz = 0;

	P03WORLD.mov_x = 0;				// Factores de desplazamiento
	P03WORLD.mov_y = 0;

	P03WORLD.cam_z = (64 << 4);		// Altura inicial de la camara (en juego es 128 << 4)

	P03WORLD.width = (PUZZLE03INFO[puzzle].width << 4);		// Medidas del decorado
	P03WORLD.height = (PUZZLE03INFO[puzzle].height << 4);

	P03WORLD.is_touch = false;		// Flag de pantalla tocada
	P03WORLD.ball_touch = false;	// Flag de bola atada

	// Transfiere las texturas y paletas a la VRAM
	for (n = 0; n <= 3; n ++) {
		N3_VramTexture(n, n, false);
		N3_VramPalette(n, n);
	}
	for (n = 28; n <=31; n ++) {
		N3_VramTexture(n, n, false);
		N3_VramPalette(n, n);
	}

	// Crea los fondos
	NF_CreateTiledBg(1, 3, "bg3_main");
	NF_CreateTiledBg(1, 2, "bg2_main");
	NF_CreateTiledBg(1, 1, "bg1_main");
	NF_CreateTiledBg(0, 3, "bg3_sub");

	// Compensa el primer tile del warfog
	NF_SetTileOfMap(1, 2, 0, 0, 1);

	// Crea la capa de texto
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTextLayer16(1, 0, 2, "text");
	} else {
		NF_CreateTextLayer16(1, 0, 1, "text");
	}
	NF_DefineTextColor(1, 0, 1, 31, 31, 31);
	NF_SetTextColor(1, 0, 1);

	// Habilita el alpha
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG0 | (BLEND_DST_BG1 + BLEND_DST_BG2 + BLEND_DST_BG3);
	REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_BG3 + BLEND_DST_SPRITE);
	REG_BLDALPHA_SUB = 0x1F | (0x05 << 8);

	// Ajusta los fondos del report si es zurdo
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(1, 1, 0, (320 - 192));
		NF_ScrollBg(1, 0, 0, (320 - 192));
	} else {
		NF_ScrollBg(1, 1, 0, 192);
		NF_ScrollBg(1, 0, 0, 192);
	}

	// Crea los sprites 2D
	NF_VramSpriteGfx(1, 0, 0, false);
	NF_VramSpritePal(1, 0, 0);
	NF_CreateSprite(1, 0, 0, 0, 128, 96);
	NF_SpriteLayer(1, 0, 3);

	// Inicializa el aura
	srand(time(NULL));
	P03WORLD.aura_angle = 0;
	P03WORLD.aura_alpha = 1;
	for (n = 0; n < 8; n ++) {
		P03WORLD.aura_peak[n] = (((int)(rand() % (7 << 4))) + (9 << 4));
		if (((int)(rand() % 100)) > 50) {
			P03WORLD.aura_peak_speed[n] = 4;
		} else {
			P03WORLD.aura_peak_speed[n] = -4;
		}
	}

	// Dibuja todos los graficos
	Puzzle03GfxEngine();

}



// Funcion Puzzle03Render3dWorld();
void Puzzle03Render3dWorld(void) {

	// Variables
	s32 x1 = 0;			// Viewport
	s32 x2 = 0;
	s32 y1 = 0;
	s32 y2 = 0;

	s32 pos_x = 0;		// Posicion de dibujado
	s32 pos_y = 0;

	u16 tile = 0;		// Tile
	u16 wall = 0;		// Debes de dibujar una pared?

	s32 dx = 0;			// Coordenadas de dibujado del poligono
	s32 dy = 0;
	s32 dz = 0;

	s32 dw = (16 << 4);		// Medidas de cada poligono
	s32 dh = (16 << 4);

	// Calcula la posicion de la camara (se invierte el EJE Y) en base a la bola en el decorado
	P03WORLD.cam_x = P03WORLD.ball_x;
	P03WORLD.cam_y = P03WORLD.ball_y;

	// Calcula el viewport del dibujado
	x1 = (((P03WORLD.cam_x >> 8) << 8) - (128 << 4));
	if (x1 < 0) x1 = 0;
	x2 = (((P03WORLD.cam_x >> 8) << 8) + (128 << 4));
	if (x2 > P03WORLD.width) x2 = P03WORLD.width;
	y1 = (((P03WORLD.cam_y >> 8) << 8) - (96 << 4));
	if (y1 < 0) y1 = 0;
	y2 = (((P03WORLD.cam_y >> 8) << 8) + (96 << 4));
	if (y2 > P03WORLD.height) y2 = P03WORLD.height;

	// Por defecto todos los poligonos son opacos
	glPolyFmt(POLY_ALPHA(31) | POLY_ID(2) | POLY_CULL_NONE);

	// Primero de todo, guarda la matriz
	glPushMatrix();

	// Y posiciona la camara
	gluLookAtf32(	P03WORLD.cam_x, (P03WORLD.height - P03WORLD.cam_y), P03WORLD.cam_z,
					P03WORLD.cam_x, (P03WORLD.height - P03WORLD.cam_y), 0,
					0, (128 << 4), 0);

	// Lee el mapa de construccion
	for (pos_y = y1; pos_y <= y2; pos_y += (16 << 4)) {
		for (pos_x = x1; pos_x <= x2; pos_x +=(16 << 4)) {

			// Obten el Tile
			tile = NF_GetTile(0, (pos_x >> 5), (pos_y >> 5));

			// Limites fuera del laberinto
			if ((tile == 0) && ((pos_x >= P03WORLD.width) || (pos_y >= P03WORLD.height))) tile = 255;

			// Dependiendo del tile...
			switch (tile) {
			
				case 0:		// Suelo
					dx = pos_x;
					dy = (P03WORLD.height - pos_y);
					dz = N3_RealToV16(-8);
					N3_TexturedQuad(dx, dy, dz,
									dx, (dy - dh), dz,
									(dx + dw), (dy - dh), dz,
									(dx + dw), dy, dz,
									2, 2);
					// Verifica el lado derecho
					if ((pos_x + (16 << 4)) <= x2) {
						wall = NF_GetTile(0, ((pos_x + (16 << 4)) >> 5), (pos_y >> 5));
						// Dibuja un muro si hay techo al lado
						if (wall == 1) {
							dx = (pos_x + (16 << 4));
							dy = (P03WORLD.height - pos_y);
							dz = (8 << 4);
							N3_TexturedQuad(dx, dy, dz,
											dx, dy, (dz - dw),
											dx, (dy - dh), (dz - dw),
											dx, (dy - dh), dz,
											1, 1);
						}
					}
					// Verifica el lado inferior
					if ((pos_y + (16 << 4)) <= y2) {
						wall = NF_GetTile(0, (pos_x >> 5), ((pos_y + (16 << 4)) >> 5));
						// Dibuja un muro si hay techo al lado
						if (wall == 1) {
							dx = pos_x;
							dy = (P03WORLD.height - (pos_y + (16 << 4)));
							dz = (8 << 4);
							N3_TexturedQuad((dx + dw), dy, dz,
											(dx + dw), dy, (dz - dh),
											dx, dy, (dz - dh),
											dx, dy, dz,
											1, 1);
						}
					}
					break;

				case 1:		// "Techo"
					dx = pos_x;
					dy = (P03WORLD.height - pos_y);
					dz = (8 << 4);
					N3_TexturedQuad(dx, dy, dz,
									dx, (dy - dh), dz,
									(dx + dw), (dy - dh), dz,
									(dx + dw), dy, dz,
									0, 0);
					// Verifica el lado derecho
					if ((pos_x + (16 << 4)) <= x2) {
						wall = NF_GetTile(0, ((pos_x + (16 << 4)) >> 5), (pos_y >> 5));
						// Dibuja un muro si no hay techo al lado
						if (wall != 1) {
							dx = (pos_x + (16 << 4));
							dy = (P03WORLD.height - pos_y);
							dz = (8 << 4);
							N3_TexturedQuad(dx, dy, dz,
											dx, dy, (dz - dw),
											dx, (dy - dh), (dz - dw),
											dx, (dy - dh), dz,
											1, 1);
						}
					}
					// Verifica el lado inferior
					if ((pos_y + (16 << 4)) <= y2) {
						wall = NF_GetTile(0, (pos_x >> 5), ((pos_y + (16 << 4)) >> 5));
						// Dibuja un muro si no hay techo al lado
						if (wall != 1) {
							dx = pos_x;
							dy = (P03WORLD.height - (pos_y + (16 << 4)));
							dz = (8 << 4);
							N3_TexturedQuad((dx + dw), dy, dz,
											(dx + dw), dy, (dz - dh),
											dx, dy, (dz - dh),
											dx, dy, dz,
											1, 1);
						}
					}
					break;

				case 2:		// Meta
					dx = pos_x;
					dy = (P03WORLD.height - pos_y);
					dz = N3_RealToV16(-8);
					N3_TexturedQuad(dx, dy, dz,
									dx, (dy - dh), dz,
									(dx + dw), (dy - dh), dz,
									(dx + dw), dy, dz,
									3, 3);

				case 3:		// Sin suelo, pero con muros
					// Verifica el lado derecho
					if ((pos_x + (16 << 4)) <= x2) {
						wall = NF_GetTile(0, ((pos_x + (16 << 4)) >> 5), (pos_y >> 5));
						// Dibuja un muro si hay techo al lado
						if (wall == 1) {
							dx = (pos_x + (16 << 4));
							dy = (P03WORLD.height - pos_y);
							dz = (8 << 4);
							N3_TexturedQuad(dx, dy, dz,
											dx, dy, (dz - dw),
											dx, (dy - dh), (dz - dw),
											dx, (dy - dh), dz,
											1, 1);
						}
					}
					// Verifica el lado inferior
					if ((pos_y + (16 << 4)) <= y2) {
						wall = NF_GetTile(0, (pos_x >> 5), ((pos_y + (16 << 4)) >> 5));
						// Dibuja un muro si hay techo al lado
						if (wall == 1) {
							dx = pos_x;
							dy = (P03WORLD.height - (pos_y + (16 << 4)));
							dz = (8 << 4);
							N3_TexturedQuad((dx + dw), dy, dz,
											(dx + dw), dy, (dz - dh),
											dx, dy, (dz - dh),
											dx, dy, dz,
											1, 1);
						}
					}
					break;

				default:		// Fuera de limites
					// Nada
					break;

			}

		}
	}

	// Renderiza la bola
	dx = (P03WORLD.ball_x - (8 << 4));
	dy = ((P03WORLD.height - P03WORLD.ball_y) + (8 << 4));
	dz = N3_RealToV16(P03WORLD.ball_z);
	dh = (15 << 4);
	dw = (15 << 4);
	// Dibuja la bola
	N3_TexturedQuad(dx, dy, dz,
					dx, (dy - dh), dz,
					(dx + dw), (dy - dh), dz,
					(dx + dw), dy, dz,
					31, 31);


	// Renderiza la linea
	if (P03WORLD.ball_touch) Puzzle03RenderPath();

	// Renderiza el aura de la bola
	if (!P03WORLD.ball_touch && !P03WORLD.ball_iner) { 
		if (P03WORLD.aura_alpha < P03_AURA_ALPHA) P03WORLD.aura_alpha ++;
		Puzzle03RenderAura();
	} else {
		if (P03WORLD.aura_alpha > 1) {
			P03WORLD.aura_alpha --;
			Puzzle03RenderAura();
		}
	}

	// Recupera la matriz
	glPopMatrix(1);

}


// Funcion Puzzle03RenderPath();
void Puzzle03RenderPath(void) {

	// Variables
	s32 z = -(4 << 4);
	s32 radius_o = (7 << 4);
	s32 radius_d = (1 << 4);
	s32 angle = 0;
	s32 fcos = 0;
	s32 fsin = 0;
	u8 red = 0;
	u8 green =0;
	u8 blue = 0;
	s32 dist = 0;
	s32 temp = 0;

	typedef struct {
		s32 vertex_x[4];		// Coordenadas de los vertices
		s32 vertex_y[4];
		s32 angle[4];
		s32 center_x[2];		// Centros de origen y destino
		s32 center_y[2];
	} path_info;
	path_info path;

	// Calcula los centros
	path.center_x[0] = P03WORLD.ball_x;							// Centro bola
	path.center_y[0] = (P03WORLD.height - P03WORLD.ball_y);
	path.center_x[1] = ((P03WORLD.cam_x - (128 << 4)) + (TOUCHPAD.pos_x << 4));
	path.center_y[1] = (P03WORLD.height - ((P03WORLD.cam_y - (96 << 4)) + (TOUCHPAD.pos_y << 4)));

	// Calcula el angulo entre ellas
	angle = GetAngle(path.center_x[0], path.center_y[0], path.center_x[1], path.center_y[1]);

	// Y el angulo de cada vertice
	path.angle[0] = (angle - 128);
	path.angle[1] = (angle - 128);
	path.angle[2] = (angle + 128);
	path.angle[3] = (angle + 128);

	// Calcula la posicion de cada vertice
	// 
	// 0
	fcos = (cosLerp((path.angle[0] << 6)) >> 5);
	fsin = (sinLerp((path.angle[0] << 6)) >> 5);
	path.vertex_x[0] = (path.center_x[0] + ((radius_o * fcos) >> 7));
	path.vertex_y[0] = (path.center_y[0] + ((radius_o * fsin) >> 7));
	// 1
	fcos = (cosLerp((path.angle[1] << 6)) >> 5);
	fsin = (sinLerp((path.angle[1] << 6)) >> 5);
	path.vertex_x[1] = (path.center_x[1] + ((radius_d * fcos) >> 7));
	path.vertex_y[1] = (path.center_y[1] + ((radius_d * fsin) >> 7));
	// 2
	fcos = (cosLerp((path.angle[2] << 6)) >> 5);
	fsin = (sinLerp((path.angle[2] << 6)) >> 5);
	path.vertex_x[2] = (path.center_x[1] + ((radius_d * fcos) >> 7));
	path.vertex_y[2] = (path.center_y[1] + ((radius_d * fsin) >> 7));
	// 3
	fcos = (cosLerp((path.angle[3] << 6)) >> 5);
	fsin = (sinLerp((path.angle[3] << 6)) >> 5);
	path.vertex_x[3] = (path.center_x[0] + ((radius_o * fcos) >> 7));
	path.vertex_y[3] = (path.center_y[0] + ((radius_o * fsin) >> 7));

	// Desactiva el uso de texturas
	GFX_TEX_FORMAT = 0;

	// Prepara en alpha
	glPolyFmt(POLY_ALPHA(P03_PATH_ALPHA) | POLY_ID(3) | POLY_CULL_NONE);

	// Calcula la distancia para crear el color
	dist = GetDistance(path.center_x[0], path.center_y[0], path.center_x[1], path.center_y[1]);
	dist = (dist >> 4);
	if (dist > 96) dist = 96;

	// Calcula el color dependiendo de la distancia
	if (dist <= 16) {
		red = 0;
		green = 255;
		blue = 0;
	}
	if ((dist > 16) && (dist < 48)) {
		temp = (dist - 16);
		red = (temp << 3);
		green = 255;
		blue = 0;
	}
	if ((dist >= 48) && (dist < 80)) {
		red = 255;
		temp = (80 - dist);
		if (temp > 31) temp = 31;
		green = (temp << 3);
		blue = 0;
	}
	if (dist >= 80) {
		red = 255;
		green = 0;
		blue = 0;
	}


	// Dibuja la linea con un poligono solido
	N3_SolidQuad(	path.vertex_x[0], path.vertex_y[0], z,
					path.vertex_x[1], path.vertex_y[1], z,
					path.vertex_x[2], path.vertex_y[2], z,
					path.vertex_x[3], path.vertex_y[3], z,
					red, green, blue);

	// Restaura el color de dibujado
	glColor(RGB15(31, 31, 31));

	// Restaura el alpha
	glPolyFmt(POLY_ALPHA(31) | POLY_ID(2) | POLY_CULL_NONE);

}



// Funcion Puzzle03RenderAura();
void Puzzle03RenderAura(void) {

	// Variables
	s32 n = 0;		// Uso general
	s32 m = 0;	
	s16 angle = P03WORLD.aura_angle;	// Calculo del punto segun el angulo
	s32 fcos = 0;
	s32 fsin = 0;
	s32 center_x = P03WORLD.ball_x;
	s32 center_y = (P03WORLD.height - P03WORLD.ball_y);
	s8 v1, v2, v3;
	s16 r, g, b;

	// Control del aura
	typedef struct {
		s32 vertex_x[2][8];
		s32 vertex_y[2][8];
		s32 vertex_z[2][8];
		s32 radius[2];
	} aura_info;
	aura_info aura;

	// Inicializa el radio
	aura.radius[1] = (7 << 4);

	// Y el color
	r = 255;
	g = 0;
	b = 0;

	// Calcula los vertices
	for (n = 0; n < 8; n ++) {
		aura.radius[0] = P03WORLD.aura_peak[n];
		for (m = 0; m < 2; m ++) {
			angle += 32;
			if (angle > 511) angle -= 512;
			// Calcula la X, Y y Z
			fcos = (cosLerp((angle << 6)) >> 5);
			fsin = (sinLerp((angle << 6)) >> 5);
			aura.vertex_x[m][n] = (center_x + ((aura.radius[m] * fcos) >> 7));
			aura.vertex_y[m][n] = (center_y + ((aura.radius[m] * fsin) >> 7));
			aura.vertex_z[m][n] = -(1 << 4);
		}
	}

	// Desactiva el uso de texturas
	GFX_TEX_FORMAT = 0;

	// Dibuja los triangulos
	for (n = 0; n < 8; n ++) {
		// Asigna los vertices
		v1 = n;
		v2 = (n - 1);
		if (v2 < 0) v2 += 8;
		v3 = n;
		// Calcula el color
		g = P03WORLD.aura_peak[n];
		if (g < 0) g = 0;
		if (g > 255) g = 255;
		// Prepara en alpha
		glPolyFmt(POLY_ALPHA(P03WORLD.aura_alpha) | POLY_ID((n + 10)) | POLY_CULL_NONE);
		// Dibuja el poligono
		N3_SolidTriangle(	aura.vertex_x[0][v1], aura.vertex_y[0][v1], aura.vertex_z[0][v1],
							aura.vertex_x[1][v2], aura.vertex_y[1][v2], aura.vertex_z[1][v2],
							aura.vertex_x[1][v3], aura.vertex_y[1][v3], aura.vertex_z[1][v3],
							r, g, b);
	}

	// Restaura el color de dibujado
	glColor(RGB15(31, 31, 31));

	// Restaura el alpha
	glPolyFmt(POLY_ALPHA(31) | POLY_ID(2) | POLY_CULL_NONE);

	// Calculos para el siguiente frame
	// Calcula el giro
	P03WORLD.aura_angle ++;
	if (P03WORLD.aura_angle > 511) P03WORLD.aura_angle -= 512;
	// Calcula los picos de los vertices
	for (n = 0; n < 8; n ++) {
		P03WORLD.aura_peak[n] += P03WORLD.aura_peak_speed[n];
		if ((P03WORLD.aura_peak[n] <= (9 << 4))) {
			P03WORLD.aura_peak[n] = (9 << 4);	
			P03WORLD.aura_peak_speed[n] = -P03WORLD.aura_peak_speed[n]; 
		}
		if (P03WORLD.aura_peak[n] >= (16 << 4)) {
			P03WORLD.aura_peak[n] = (16 << 4);
			P03WORLD.aura_peak_speed[n] = -P03WORLD.aura_peak_speed[n]; 
		}
	}

}



// Funcion Puzzle03Start();
void Puzzle03Start(void) {

	// Variables
	s32 n = 0;
	s8 fade = 64;

	// Zoom inicial (de 64 a 128)
	for (n = (64 << 4); n <= (128 << 4); n += (1 << 4)) {
		// Ajusta el brillo
		if (fade > 0) {
			fade --;
			setBrightness(3, (0 - (fade >> 2)));
		}
		// Calcula el movimiento del fondo
		if (GAMEOPTIONS[0].lefthanded) {
			P03WORLD.background -= 64;
			if (P03WORLD.background < 0) P03WORLD.background += (128 << 8);
		} else {
			P03WORLD.background += 64;
			if (P03WORLD.background > (128 << 8)) P03WORLD.background -= (128 << 8);
		}
		// Actualiza la posicion de la camara
		P03WORLD.cam_z = n;
		// Dibuja todos los graficos
		Puzzle03GfxEngine();
	}

	// Deja los niveles de brillos correctos
	setBrightness(3, 0);
	swiWaitForVBlank();

}



// Funcion Puzzle03Exit();
void Puzzle03Exit(u8 option) {

	// Variables
	s32 n = 0;
	s8 fade = 0;

	// Zoom inicial (de 64 a 128)
	for (n = (128 << 4); n >= (64 << 4); n -= (1 << 4)) {
		// Ajusta el brillo
		if (fade < 64) {
			fade ++;
			setBrightness(3, (0 - (fade >> 2)));
			// Ajusta el volumen de la musica si es necesario
			if (option != 0) BgmVolume(((4096 - (fade << 6)) >> 2));
		}
		// Calcula el movimiento del fondo
		if (GAMEOPTIONS[0].lefthanded) {
			P03WORLD.background -= 64;
			if (P03WORLD.background < 0) P03WORLD.background += (128 << 8);
		} else {
			P03WORLD.background += 64;
			if (P03WORLD.background > (128 << 8)) P03WORLD.background -= (128 << 8);
		}
		// Actualiza la posicion de la camara
		P03WORLD.cam_z = n;
		// Dibuja todos los graficos
		Puzzle03GfxEngine();
	}

	// Deja los niveles de brillos correctos
	setBrightness(3, -16);
	// Deten la musica si es necesario
	if (option != 0) StopBgm();
	// Sincronismo
	swiWaitForVBlank();

}



// Funcion Puzzle03MoveBall();
u8 Puzzle03MoveBall(void) {

	// Inicializa las variables
	s32 x = P03WORLD.ball_fx;		// Posicion de la bola en el mundo
	s32 y = P03WORLD.ball_fy;
	s32 px = 0;						// Posicion del puntero en el mundo
	s32 py = 0;
	s32 mov_x = P03WORLD.mov_x;		// Factor de movimiento de la bola
	s32 mov_y = P03WORLD.mov_y;
	bool collision = false;			// Colision con algun muro
	bool hit_x = false;
	bool hit_y = false;
	u8 attempt = 0;					// Intentos
	s32 temp_x = 0;					// Calculos temporales
	s32 temp_y = 0;
	s32 n = 0;

	// Si el Stylus toca la pantalla, calcula su posicion en el mundo
	if (TOUCHPAD.held) {

		// Calcula la posicion del puntero en el mundo
		px = (((P03WORLD.cam_x - (128 << 4)) + (TOUCHPAD.pos_x << 4)) << 4);
		py = (((P03WORLD.cam_y - (96 << 4)) + (TOUCHPAD.pos_y << 4)) << 4);

		// Verifica si has tocado la pelota (primer toque)
		if (!P03WORLD.is_touch && !P03WORLD.ball_iner) {
			if (
				(px >= (P03WORLD.ball_fx - (8 << 8)))
				&&
				(px <= (P03WORLD.ball_fx + (8 << 8)))
				&&
				(py >= (P03WORLD.ball_fy - (8 << 8)))
				&&
				(py <= (P03WORLD.ball_fy + (8 << 8)))
				) {
				// Flag de bola atada arriba
				P03WORLD.ball_touch = true;
				// Cuenta el intento
				attempt ++;
			}
		}

		// Si tienes la bola atada, calcula el movimiento
		if (P03WORLD.ball_touch && !P03WORLD.ball_iner) {
			// Calcula el factor de desplazamiento X
			mov_x = (px - P03WORLD.ball_fx);
			if (mov_x < 0) {
				mov_x = -mov_x;
				mov_x = (mov_x >> 5);
				mov_x = -mov_x;
			} else {
				mov_x = (mov_x >> 5);
			}
			// Calcula el factor de desplazamiento X
			mov_y = (py - P03WORLD.ball_fy);
			if (mov_y < 0) {
				mov_y = -mov_y;
				mov_y = (mov_y >> 5);
				mov_y = -mov_y;
			} else {
				mov_y = (mov_y >> 5);
			}
			// Calcula la zona muerta
			if ((mov_x >= - 32) && (mov_x <= 32)) mov_x = 0;
			if ((mov_y >= - 32) && (mov_y <= 32)) mov_y = 0;
		}

		// Flag de pantalla tocada arriba
		P03WORLD.is_touch = true;

	} else {	// Reinicia los flags dependientes de "tocando la pantalla"

		// Flag de pantalla tocada abajo
		P03WORLD.is_touch = false;

		// Flag de bola atada abajo
		P03WORLD.ball_touch = false;

		// Flag de inercia arriba?
		if ((mov_x <= -8) || (mov_x >= 8) || (mov_y <= -8) || (mov_y >= 8)) {
			P03WORLD.ball_iner = true;	
		}

	}

	// Calcula la inercia
	if (P03WORLD.ball_iner) {
		// Inercia X
		if ((mov_x <= -8) || (mov_x >= 8)) {
			if (mov_x >= 0) {
				mov_x -= (mov_x >> 7) + 2;
			} else {
				mov_x = -mov_x;
				mov_x -= (mov_x >> 7) + 2;
				mov_x = -mov_x;
			}
		} else {
			mov_x = 0;
		}
		// Inercia Y
		if ((mov_y <= -8) || (mov_y >= 8)) {
			if (mov_y >= 0) {
				mov_y -= (mov_y >> 7) + 2;
			} else {
				mov_y = -mov_y;
				mov_y -= (mov_y >> 7) + 2;
				mov_y = -mov_y;
			}
		} else {
			mov_y = 0;
		}
		// Debes poner el flag abajo?
		if ((mov_x == 0) && (mov_y == 0)) P03WORLD.ball_iner = false;
	}

	// Si te desplazas a la derecha
	if (mov_x > 0) {
		collision = false;
		for (n = 0; n < 8; n ++) {
			temp_x = (x + mov_x + (P03_BALL_SHAPE[n] << 8));
			if (
				(NF_GetTile(1, (temp_x >> 8), ((y >> 8) - (8 - n))) == P03_TILE_WALL)
				||
				(NF_GetTile(1, (temp_x >> 8), ((y >> 8) + (8 - n))) == P03_TILE_WALL)
				) {
				collision = true;
				x = (((temp_x >> 12) << 12) - (P03_BALL_SHAPE[n] << 8));
			}
		}
		if (collision) {
			hit_x = true;
			mov_x = -mov_x;
			P03WORLD.ball_touch = false;
			P03WORLD.ball_iner = true;
		}
	}

	// Si te desplazas a la izquierda
	if (mov_x < 0) {
		collision = false;
		for (n = 0; n < 8; n ++) {
			temp_x = (x + mov_x - (P03_BALL_SHAPE[n] << 8));
			if (
				(NF_GetTile(1, (temp_x >> 8), ((y >> 8) - (8 - n))) == P03_TILE_WALL)
				||
				(NF_GetTile(1, (temp_x >> 8), ((y >> 8) + (8 - n))) == P03_TILE_WALL)
				) {
				collision = true;
				x = (((temp_x >> 12) << 12) + (P03_BALL_SHAPE[n] << 8) + (16 << 8));
			}
		}
		if (collision) {
			hit_x = true;
			mov_x = -mov_x;
			P03WORLD.ball_touch = false;
			P03WORLD.ball_iner = true;
		}
	}

	// Si te desplazas abajo
	if (mov_y > 0) {
		collision = false;
		for (n = 0; n < 8; n ++) {
			temp_y = (y + mov_y + (P03_BALL_SHAPE[n] << 8));
			if (
				(NF_GetTile(1, ((x >> 8) - (8 - n)), (temp_y >> 8)) == P03_TILE_WALL)
				||
				(NF_GetTile(1, ((x >> 8) + (8 - n)), (temp_y >> 8)) == P03_TILE_WALL)
				) {
				collision = true;
				y = (((temp_y >> 12) << 12) - (P03_BALL_SHAPE[n] << 8));
			}
		}
		if (collision) {
			hit_y = true;
			mov_y = -mov_y;
			P03WORLD.ball_touch = false;
			P03WORLD.ball_iner = true;
		}
	}

	// Si te desplazas arriba
	if (mov_y < 0) {
		collision = false;
		for (n = 0; n < 8; n ++) {
			temp_y = (y + mov_y - (P03_BALL_SHAPE[n] << 8));
			if (
				(NF_GetTile(1, ((x >> 8) - (8 - n)), (temp_y >> 8)) == P03_TILE_WALL)
				||
				(NF_GetTile(1, ((x >> 8) + (8 - n)), (temp_y >> 8)) == P03_TILE_WALL)
				) {
				collision = true;
				y = (((temp_y >> 12) << 12) + (P03_BALL_SHAPE[n] << 8) + (16 << 8));
			}
		}
		if (collision) {
			hit_y = true;
			mov_y = -mov_y;
			P03WORLD.ball_touch = false;
			P03WORLD.ball_iner = true;
		}
	}
		
	// Aplica el movimiento
	if (!hit_x) x += mov_x;
	if (!hit_y) y += mov_y;

	// Si hay un rebote, contabilizalo
	if (hit_x || hit_y) attempt ++;

	// Backup de las variables globales
	P03WORLD.ball_fx = x;
	P03WORLD.ball_fy = y;
	P03WORLD.ball_x = (x >> 4);
	P03WORLD.ball_y = (y >> 4);
	P03WORLD.mov_x = mov_x;
	P03WORLD.mov_y = mov_y;

	// Devuelve si se ha realizado un nuevo intento
	return attempt;

}



// Funcion Puzzle03CheckGoal();
bool Puzzle03CheckGoal(void) {

	// Variables
	bool result = false;

	// Verificas si la bola esta sobre la zona de meta
	if (
		(NF_GetTile(1, ((P03WORLD.ball_fx >> 8) + 8), (P03WORLD.ball_fy >> 8)) == P03_TILE_GOAL)
		&&
		(NF_GetTile(1, ((P03WORLD.ball_fx >> 8) - 8), (P03WORLD.ball_fy >> 8)) == P03_TILE_GOAL)
		&&
		(NF_GetTile(1, (P03WORLD.ball_fx >> 8), ((P03WORLD.ball_fy >> 8) + 8)) == P03_TILE_GOAL)
		&&
		(NF_GetTile(1, (P03WORLD.ball_fx >> 8), ((P03WORLD.ball_fy >> 8) - 8)) == P03_TILE_GOAL)
		) {
		result = true;
	}

	// Devuelve el resultado
	return result;

}



// Puzzle03DrawMinimap();
void Puzzle03DrawMinimap(void) {

	// Variables
	s32 x = (P03WORLD.ball_fx >> 9);
	s32 y = (P03WORLD.ball_fy >> 9);
	s32 width = (P03WORLD.width >> 5);
	s32 height = (P03WORLD.height >> 5);
	s32 add_x = 0;
	s32 add_y = 0;

	// Modo Zurdo? Compensa el desplazamiento del mini-mapa
	if (GAMEOPTIONS[0].lefthanded) {
		add_x = (((((width - 1) >> 8) + 1) << 8) - width);
		add_y = (((((height - 1) >> 8) + 1) << 8) - height);
		x += add_x;
		y += add_y;
		width += add_x;
		height += add_y;
	}

	// Calcula la posicion del X fondo
	P03WORLD.minimap_x = (x - 128);
	if (P03WORLD.minimap_x < add_x) P03WORLD.minimap_x = add_x;
	if (P03WORLD.minimap_x > (width - 256)) P03WORLD.minimap_x = (width - 256);

	// Calcula la posicion del Y fondo
	P03WORLD.minimap_y = (y - 96);
	if (P03WORLD.minimap_y < add_y) P03WORLD.minimap_y = add_y;
	if (P03WORLD.minimap_y > (height - 192)) P03WORLD.minimap_y = (height - 192);

	// Calcula la posicion de la bola
	P03WORLD.miniball_x = (x - P03WORLD.minimap_x);
	P03WORLD.miniball_y = (y - P03WORLD.minimap_y);
	NF_MoveSprite(1, 0, (P03WORLD.miniball_x - 4), (P03WORLD.miniball_y - 4));

	// Actualiza la niebla de guerra
	Puzzle03WarFog(x, y, width, height);

}



// Funcion Puzzle03WarFog();
void Puzzle03WarFog(s32 x, s32 y, s32 width, s32 height) {

	// Variables
	s32 start_x = ((x >> 3) - 3);
	s32 start_y = ((y >> 3) - 3);
	s32 end_x = ((x >> 3) + 3);
	s32 end_y = ((y >> 3) + 3);
	s32 fx = 0;
	s32 fy = 0;
	bool update = false;

	// Verifica el cuadrado general
	for (fy = start_y; fy <= end_y; fy ++) {
		for (fx = start_x; fx <= end_x; fx ++) {
			// Esta en los limites?
			if (
				(fy >= 0)
				&&
				(fy < (height >> 3))
				&&
				(fx >= 0)
				&&
				(fx < (width >> 3))
				) {
				// Si el tile es niebla de guerra
				if (NF_GetTileOfMap(1, 2, fx, fy) == 1) {
					// Hazlo transparente y marca la actualizacion
					NF_SetTileOfMap(1, 2, fx, fy, 0);
					update = true;
				}
			}
		}
	}

	// Si es necesario, actualiza el mapa
	if (update) NF_UpdateVramMap(1, 2);

}



// Funcion Puzzle03GfxEngine();
void Puzzle03GfxEngine(void) {

		// Renderiza el mundo 3D
		Puzzle03Render3dWorld();

		// Actualiza el minimapa
		Puzzle03DrawMinimap();

		// Calcula el movimiento del fondo
		if (GAMEOPTIONS[0].lefthanded) {
			P03WORLD.background -= 64;
			if (P03WORLD.background < (128 << 8)) P03WORLD.background += (128 << 8);
		} else {
			P03WORLD.background += 64;
			if (P03WORLD.background > (256 << 8)) P03WORLD.background -= (128 << 8);
		}

		// Manda la escena 3D a la pantalla
		glFlush(0);

		// Actualiza el OAM de la libreria
		NF_SpriteOamSet(1);

		// Espera al sincronismo vertical
		swiWaitForVBlank();

		// Actualiza el OAM de libnds
		oamUpdate(&oamSub);

		// Mueve el fondo 2D
		NF_ScrollBg(0, 3, (P03WORLD.background >> 8), (P03WORLD.background >> 8));
		NF_ScrollBg(1, 2, P03WORLD.minimap_x, P03WORLD.minimap_y);
		NF_ScrollBg(1, 3, P03WORLD.minimap_x, P03WORLD.minimap_y);

}



// Funcion Puzzle03DrawEndMenu();
void Puzzle03DrawEndMenu(s32 bx0, s32 bx1, s32 bx2, s32 by) {

	// Variables
	s32 x[3];				// Referencia de posicion
	s32 y;
	s32 dx = 0;				// Coordenadas de dibujado del poligono
	s32 dy = 0;
	s32 dz = 0;
	s32 dw = (32 << 4);		// Medidas de cada poligono
	s32 dh = (128 << 4);
	s8 n = 0;

	// Calcula el viewport del dibujado
	s32 scr_x = (P03WORLD.cam_x - (128 << 4));
	s32 scr_y = (P03WORLD.cam_y - (96 << 4));

	// Calcula la posicion de los botones
	x[0] = (bx0 >> 4);
	x[1] = (bx1 >> 4);
	x[2] = (bx2 >> 4);
	y = (by >> 4);

	// Primero de todo, guarda la matriz
	glPushMatrix();

	// Y posiciona la camara
	gluLookAtf32(	P03WORLD.cam_x, (P03WORLD.height - P03WORLD.cam_y), P03WORLD.cam_z,
					P03WORLD.cam_x, (P03WORLD.height - P03WORLD.cam_y), 0,
					0, (128 << 4), 0);

	// Renderiza el boton
	for (n = 0; n < 3; n ++) {
		dx = (x[n] + scr_x);
		dy = (P03WORLD.height - (y + scr_y));
		dz = N3_RealToV16(9);
		N3_TexturedQuad(dx, dy, dz,
						dx, (dy - dh), dz,
						(dx + dw), (dy - dh), dz,
						(dx + dw), dy, dz,
						(28 + n), (28 + n));
	}

	// Recupera la matriz
	glPopMatrix(1);

}



// Funcion Puzzle03Results();
void Puzzle03Results(u8 puzzle, u32 time, u32 moves) {

	// Variables
	u16 points = 0;			// Puntos totales ganados
	u16 time_points = 0;	// Puntos ganados por tiempo
	u16 moves_points = 0;	// Puntos ganados por movimientos
	u16 record_points = 0;	// Puntos por record
	u16 mins = 0;			// Minutos
	u16 secs = 0;			// Segundos
	u16 factor = ((PUZZLE03INFO[puzzle].width + PUZZLE03INFO[puzzle].height) >> 3);
	char text[24];			// Buffer de texto

	// Añade un contador a puzzles resueltos
	SAVEDATA[2].solved[puzzle] ++;

	// Maximo de tiempo
	if (time > 5999) time = 5999;
	// Maximo de movimientos
	if (moves > 999) moves = 999;

	// Calcula los minutos y segundos
	mins = (int)(time / 60);
	secs = (time - (mins * 60));


	//////////////////////////////
	// Calcula las puntuaciones //
	//////////////////////////////

	// Por tiempo	(Por debajo de ((alto + ancho) / 8) segundos)
	if (time < factor) {	
		time_points = ((factor - time) << 1);
	}

	// Por movimientos	(Por debajo de ((alto + ancho) / 16) movimientos)
	if (moves < (factor >> 1)) {
		moves_points = (((factor >> 1) - moves) << 1);
	}

	// Registra los records
	if (time <= SAVEDATA[2].time[puzzle]) {
		SAVEDATA[2].time[puzzle] = time;
		record_points += 50;
	}
	if (moves <= SAVEDATA[2].moves[puzzle]) {
		SAVEDATA[2].moves[puzzle] = moves;
		record_points += 50;
	}

	// Total de puntos
	points = (100 + time_points + moves_points + record_points);

	// Registra la puntuacion
	SAVEDATA[2].points += points;
	SAVEDATA[2].credits += points;
	SAVEDATA[2].score[puzzle] += points;

	// Guardalo los datos
	WriteSavegame();

	// Genera el report
	NF_ClearTextLayer16(1, 0);

	switch (GAMEOPTIONS[0].language) {

		case 0:		// Castellano
			sprintf(text, "Tiempo: %02d:%02d", mins, secs);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Pasos: %03d", moves);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Puntuacion");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Tiempo:  %03d", time_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Pasos:   %03d", moves_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 12, text);
			break;

		case 1:		// Ingles
			sprintf(text, "Time: %02d:%02d", mins, secs);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Moves: %03d", moves);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Score");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Time:    %03d", time_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Moves:   %03d", moves_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(1, 0, (12 - (strlen(text) >> 1)), 12, text);
			break;

		default:
			break;

	}

	// Actualiza las capas de texto
	NF_UpdateTextLayers();

}



// Funcion EndPuzzle03();
void EndPuzzle03(u8 end) {

	// Variables
	s16 n = 0;
	s32 y = 0;
	s32 speed = 0;
	
	// Entrada del report, si se ha resuelto el puzzle
	if (end == 0) {
		// Pausa de 1 segundo
		for (n = 0; n < 60; n ++) {
			Puzzle03GfxEngine();
		}
		// Muestra el report
		y = (192 << 8);
		while (y > 0) {
			speed = (y >> 3);
			if (speed < 512) speed = 512;
			y -= speed;
			if (y < 0) y = 0;
			if (GAMEOPTIONS[0].lefthanded) {
				NF_ScrollBg(1, 1, 0, (320 - (y >> 8)));
				NF_ScrollBg(1, 0, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(1, 1, 0, (y >> 8));
				NF_ScrollBg(1, 0, 0, (y >> 8));
			}
			// Actualiza la pantalla
			Puzzle03GfxEngine();
		}
	}

}



// Funcion EndMenuPuzzle03();
u8 EndMenuPuzzle03(u8 end) {

	// Variables
	s16 n = 0;			// Uso general
	bool loop = false;	// Control de bucles
	s32 x[3];			// Posicion de los botones
	s32 in[4];			// Distancia de entrada
	s32 out[4];			// Distancia de salida
	s32 y = 0;			// Vertical de los botones y posicion de la capa de report
	s32 speed = 0;		// Calculo de velocidad
	bool touch = false;	// Control tactil
	u8 option = 0;		// Opcion seleccionada

	u16 button_x[3];		// Define el area tactil de los botones
	u16 button_y = 0;
	u16 button_width = 0;
	u16 button_height = 0;

	// Inicializa las posiciones y control de botones
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
	y = (32 << 8);

	// Area tactil de los botones
	if (GAMEOPTIONS[0].lefthanded) {
		button_x[0] = 51;
		button_x[1] = 106;
		button_x[2] = 161;
	} else {
		button_x[0] = 180;
		button_x[1] = 124;
		button_x[2] = 69;
	}
	button_width = 28;
	button_y = 23;
	button_height = 147;


	///////////////////////////////
	// Crea los botones del menu //
	///////////////////////////////

	// Calcula la posicion de los botones
	if (GAMEOPTIONS[0].lefthanded) {
		Puzzle03DrawEndMenu(((x[0] - in[0]) - (4 << 8)), ((x[1] - in[1]) - (4 << 8)), ((x[2] - in[2]) - (4 << 8)), y);
	} else {
		Puzzle03DrawEndMenu(((x[0] + in[0]) - (4 << 8)), ((x[1] + in[1]) - (4 << 8)), ((x[2] + in[2]) - (4 << 8)), y);
	}

	// Actualiza el contenido de las pantallas
	Puzzle03GfxEngine();


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
			}
		}
		// Dibujalos
		if (GAMEOPTIONS[0].lefthanded) {
			Puzzle03DrawEndMenu(((x[0] - in[0]) - (4 << 8)), ((x[1] - in[1]) - (4 << 8)), ((x[2] - in[2]) - (4 << 8)), y);
		} else {
			Puzzle03DrawEndMenu(((x[0] + in[0]) - (4 << 8)), ((x[1] + in[1]) - (4 << 8)), ((x[2] + in[2]) - (4 << 8)), y);
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == 0) && (in[1] == 0) && (in[2] == 0)) loop = false;
		// Actualiza el contenido de las pantallas
		Puzzle03GfxEngine();
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
						(TOUCHPAD.pos_x >= button_x[n])
						&&
						(TOUCHPAD.pos_x <= (button_x[n] + button_width))
						&&
						(TOUCHPAD.pos_y >= button_y)
						&&
						(TOUCHPAD.pos_y <= (button_y + button_height))
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
		// Dibujalos los botones del menu
		Puzzle03DrawEndMenu((x[0] - (4 << 8)), (x[1] - (4 << 8)), (x[2] - (4 << 8)), y);
		// Actualiza el contenido de las pantallas
		Puzzle03GfxEngine();
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
			}
		}
		// Dibujalos
		if (GAMEOPTIONS[0].lefthanded) {
			Puzzle03DrawEndMenu(((x[0] + in[0]) - (4 << 8)), ((x[1] + in[1]) - (4 << 8)), ((x[2] + in[2]) - (4 << 8)), y);
		} else {
			Puzzle03DrawEndMenu(((x[0] - in[0]) - (4 << 8)), ((x[1] - in[1]) - (4 << 8)), ((x[2] - in[2]) - (4 << 8)), y);
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == out[0]) && (in[1] == out[1]) && (in[2] == out[2])) loop = false;
		// Actualiza el contenido de las pantallas
		Puzzle03GfxEngine();
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
				NF_ScrollBg(1, 1, 0, (320 - (y >> 8)));
				NF_ScrollBg(1, 0, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(1, 1, 0, (y >> 8));
				NF_ScrollBg(1, 0, 0, (y >> 8));
			}
			// Actualiza el contenido de las pantallas
			Puzzle03GfxEngine();
		}
	}

	// Devuelve la opcion
	return option;

}



// Funcion RetryPuzzle03();
void RetryPuzzle03(u8 puzzle) {

	// Variables
	u16 x = 0;
	u16 y = 0;

	/*
	Atencion!
	El motor 3D de DS usa como unidad el numero 16:1
	Esto significa que un pixel real equivale a 16 unidades
	Por lo que todas las coordenadas reales se conviertes
	con el siguiente bitshift (1 << 4)
	*/

	// Inicializa los datos del mundo
	if (GAMEOPTIONS[0].lefthanded) {
		P03WORLD.ball_x = (PUZZLE03INFO[puzzle].start_x[1] << 4);	// Posicion de la bola zurdos
		P03WORLD.ball_y = (PUZZLE03INFO[puzzle].start_y[1] << 4);
		P03WORLD.ball_fx = (PUZZLE03INFO[puzzle].start_x[1] << 8);	// Posicion de la bola zurdos (Fixed << 8)
		P03WORLD.ball_fy = (PUZZLE03INFO[puzzle].start_y[1] << 8);
		P03WORLD.background = (256 << 8);
	} else {
		P03WORLD.ball_x = (PUZZLE03INFO[puzzle].start_x[0] << 4);	// Posicion de la bola diestros
		P03WORLD.ball_y = (PUZZLE03INFO[puzzle].start_y[0] << 4);
		P03WORLD.ball_fx = (PUZZLE03INFO[puzzle].start_x[0] << 8);	// Posicion de la bola diestros (Fixed << 8)
		P03WORLD.ball_fy = (PUZZLE03INFO[puzzle].start_y[0] << 8);
		P03WORLD.background = (128 << 8);
	}
	P03WORLD.ball_z = 0;			// Altura de la bola
	P03WORLD.ball_fz = 0;

	P03WORLD.mov_x = 0;				// Factores de desplazamiento
	P03WORLD.mov_y = 0;

	P03WORLD.cam_z = (64 << 4);		// Altura inicial de la camara (en juego es 128 << 4)

	P03WORLD.width = (PUZZLE03INFO[puzzle].width << 4);		// Medidas del decorado
	P03WORLD.height = (PUZZLE03INFO[puzzle].height << 4);

	P03WORLD.is_touch = false;		// Flag de pantalla tocada
	P03WORLD.ball_touch = false;	// Flag de bola atada

	// Rellena la capa de niebla
	for (y = 0; y < (PUZZLE03INFO[puzzle].height >> 4); y ++) {
		for (x = 0; x < (PUZZLE03INFO[puzzle].width >> 4); x ++) {
			NF_SetTileOfMap(1, 2, x, y, 1);
		}
	}

	// Ajusta los fondos del report si es zurdo
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(1, 1, 0, (320 - 192));
		NF_ScrollBg(1, 0, 0, (320 - 192));
	} else {
		NF_ScrollBg(1, 1, 0, 192);
		NF_ScrollBg(1, 0, 0, 192);
	}

	// Dibuja todos los graficos
	Puzzle03GfxEngine();

}
