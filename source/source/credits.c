
// Mind Maze - Funciones de Creditos
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



// Funcion CreditsMain();
void CreditsMain(void) {

	//////////////////////////
	//						//
	//  Inicializaciones	//
	//						//
	//////////////////////////

	// Reinicialo todo
	ResetAll();

	// Variables
	bool fade_in = true;		// Fade IN
	bool fade_out = false;		// Fade OUT
	s32 fade = (16 << 8);		// Valor del FADE

	bool loop = true;			// Control del bucle principal


	//////////////////////
	//					//
	//  Carga Archivos	//
	//					//
	//////////////////////

	// Muestra la pantalla de carga
	LoadingIn();
	// Carga los archivos
	CreditsLoadFiles();
	// Carga la Musica
	LoadBgm(MOD_CREDITS);
	// Oculta la pantalla de carga
	LoadingOut();


	//////////////////////
	//					//
	// Crea el entorno	//
	//					//
	//////////////////////

	// Habilita el 3D en la pantalla tactil
	N3_Set3D(1, 0);
	N3_Init3dEngine();

	// Inicializa los fondos 2D en el main engine
	NF_InitTiledBgSys(0);

	// Inicializa el modo 16 bits para el sub engine
	NF_Set2D(1, 5);

	// Inicializa el fondo BITMAP de 16 bits, para poder recibir la captura
	NF_InitBitmapBgSys(1, 1);

	// Prepara el Banco D para guardar las capturas 3D
	vramSetBankD(VRAM_D_LCD);

	// Y habilita la capa de dibujado para el 3D en el main engine, poniendola al fondo
	N3_3dLayer(3);
	NF_ShowBg(0, 0);

	// Crea el mundo en 3D
	CreditsCreateWorld();

	// Reproduce la Musica
	PlayBgm();
	BgmVolume(0);
	
	//////////////////////////
	//						//
	// Nucleo de ejecucion	//
	//						//
	//////////////////////////

	while (loop) {

		// Intercambio de pantalla para el dual 3D (false = main)
		CREDITSWORLD.screen = !CREDITSWORLD.screen;
		if (CREDITSWORLD.screen) {
			lcdMainOnBottom();	// 3D en la inferior
		} else {
			lcdMainOnTop();		// 3D en la superior
		}
		
		// Lee el teclado
		ReadKeypad();

		// Effecto "fade in"
		if (fade_in) {
			fade -= 64;
			if (fade <= 0) {
				fade = 0;
				fade_in = false;
			}
			// Ajusta el brillo
			setBrightness(3, (0 - (fade >> 8)));
			// Si es necesario, ajusta el volumen
			BgmVolume(((4096 - fade) >> 2));
		}

		// Effecto "fade out"
		if (fade_out) {
			fade += 64;
			if (fade >= (16 << 8)) {
				fade = (16 << 8);
				fade_out = false;
				loop = false;
				StopBgm();
			}
			// Ajusta el brillo
			setBrightness(3, (0 - (fade >> 8)));
			// Si es necesario, ajusta el volumen
			BgmVolume(((4096 - fade) >> 2));
		}

		// Mueve el tunel
		CreditsMoveTunnel();

		// Mueve el panel de creditos
		if (!CREDITSWORLD.panel_update && !fade_in && !fade_out) {
			fade_out = CreditsMovePanel();
		}
		if (fade_out && loop) CreditsMovePanel();

		// Renderiza el mundo en 3D
		CreditsRender3dWorld();

		// Abortar? (Salir de los creditos)
		if (!fade_in && !fade_out && AbortKeys()) {
			fade_out = true;
		}

		// Espera al sincronismo vertical
		swiWaitForVBlank();

		// Manda la captura del banco D al banco C, teniendo asi una imagen en el subengine
		NF_DmaMemCopy((void*)0x06200000, (void*)0x06860000, 98304);	// (256 x 192 pixeles)

		// Muestra o oculta el logo (fondo 2D)
		if (CREDITSWORLD.screen) {
			NF_ShowBg(0, 1);
		} else {
			NF_HideBg(0, 1);
		}

		// Espera a que el dispositivo de captura este listo
		while (REG_DISPCAPCNT & DCAP_ENABLE);

		// Captura el frame actual al banco D de VRAM
		REG_DISPCAPCNT = DCAP_BANK(3)| DCAP_ENABLE | DCAP_SIZE(3); 

		// Actualiza las texturas del panel de creditos, si es necesario
		if (CREDITSWORLD.panel_update) CreditsUpdateVramPanelTextures();

	}


}



// Funcion CreditsLoadFiles();
void CreditsLoadFiles(void) {

	// Variables
	u8 n = 0;
	char file[64];

	//////////////////////////////////
	//								//
	//  Archivos entorno 3D (Main)  //
	//								//
	//////////////////////////////////

	// Carga las texturas del suelo
	for (n = 0; n <= CREDITS_FLOOR_TEX_LASTNUM; n ++) {
		sprintf (file, "credits/tex/floor/cre_floor%02d", n);
		N3_LoadTexture(file, (n + CREDITS_FLOOR_TEX), 16, 16);
	}
	N3_LoadPal("credits/tex/floor/cre_floor", CREDITS_FLOOR_PAL);

	// Carga las texturas del techo
	for (n = 0; n <= CREDITS_CEIL_TEX_LASTNUM; n ++) {
		sprintf (file, "credits/tex/ceil/cre_ceil%02d", n);
		N3_LoadTexture(file, (n + CREDITS_CEIL_TEX), 32, 32);
	}
	N3_LoadPal("credits/tex/ceil/cre_ceil", CREDITS_CEIL_PAL);

	// Carga las texturas del muro
	for (n = 0; n <= CREDITS_WALL_TEX_LASTNUM; n ++) {
		sprintf (file, "credits/tex/wall/cre_wall%02d", n);
		N3_LoadTexture(file, (n + CREDITS_WALL_TEX), 32, 32);
	}
	N3_LoadPal("credits/tex/wall/cre_wall", CREDITS_WALL_PAL);

	// Carga los archivos para los creditos
	CreditsLoadMsgFiles();
	N3_LoadPal("credits/tex/msg/papyre", CREDITS_MSG_PAL);

	// Carga los archivos del Logo
	NF_LoadTiledBg("credits/bg/cre_logo", "logo", 256, 256);
	if (GAMEOPTIONS[0].lefthanded) {
		RotateTiledBg("logo");
	}

}



// Funcion CreditsCreateWorld();
void CreditsCreateWorld(void) {

	// Variables
	u8 n = 0;
	u8 f = 0; 
	u8 x = 0;
	u8 y = 0;

	// Inicializa el random
	srand(time(NULL));

	// Inicializa el mapa de alturas de las 2 paredes y sus texturas [0][1]
	for (n = 0; n <= 1; n ++) {	// Pared
		for (y = 0; y < CREDITS_WALL_DEEP; y ++) {	// Fondo
			for (x = 0; x < CREDITS_WALL_HEIGHT; x ++) {	// Alto
				CREDITSWORLD.height[n][y][x] = (int)((rand() % CREDITS_WALL_MAX) + CREDITS_WALL_MIN);
				CREDITSWORLD.texture[n][y][x] = (int)((rand() % CREDITS_WALL_TEX_LASTNUM) + 1 + CREDITS_WALL_TEX);
			}
		}
	}
	// Inicializa el mapa de alturas del techo y sus texturas [2]
	for (y = 0; y < CREDITS_CEIL_DEEP; y ++) {	// Fondo
		for (x = 0; x < CREDITS_CEIL_WIDTH; x ++) {	// Ancho
			CREDITSWORLD.height[2][y][x] = (int)((rand() % CREDITS_CEIL_MAX) + CREDITS_CEIL_MIN);
			CREDITSWORLD.texture[2][y][x] = (int)((rand() % CREDITS_CEIL_TEX_LASTNUM) + 1 + CREDITS_CEIL_TEX);
		}
	}
	// Inicializa el mapa de alturas del suelo y sus texturas
	for (y = 0; y < CREDITS_FLOOR_DEEP; y ++) {	// Fondo
		for (x = 0; x < CREDITS_FLOOR_WIDTH; x ++) {	// Ancho
			CREDITSWORLD.height[3][y][x] = (int)((rand() % CREDITS_FLOOR_MAX) + CREDITS_FLOOR_MIN);
			CREDITSWORLD.texture[3][y][x] = (int)((rand() % CREDITS_FLOOR_TEX_LASTNUM) + 1 + CREDITS_FLOOR_TEX);
		}
	}

	// Inicializa variables del dual 3D
	CREDITSWORLD.screen = true;

	// Y el angulo de rotacion del cubo
	CREDITSWORLD.cube_angle = 0;

	// Manda las texturas del suelo a la VRAM
	for (n = 0; n <= CREDITS_FLOOR_TEX_LASTNUM; n ++) {
		N3_VramTexture((n + CREDITS_FLOOR_TEX), (n + CREDITS_FLOOR_TEX), false);
	}
	N3_VramPalette(CREDITS_FLOOR_PAL, CREDITS_FLOOR_PAL);

	// Manda las texturas del techo a la VRAM
	for (n = 0; n <= CREDITS_CEIL_TEX_LASTNUM; n ++) {
		N3_VramTexture((n + CREDITS_CEIL_TEX), (n + CREDITS_CEIL_TEX), false);
	}
	N3_VramPalette(CREDITS_CEIL_PAL, CREDITS_CEIL_PAL);

	// Manda las texturas del muro a la VRAM
	for (n = 0; n <= CREDITS_WALL_TEX_LASTNUM; n ++) {
		N3_VramTexture((n + CREDITS_WALL_TEX), (n + CREDITS_WALL_TEX), false);
	}
	N3_VramPalette(CREDITS_WALL_PAL, CREDITS_WALL_PAL);

	// Crea el Logo
	NF_CreateTiledBg(0, 1, "logo");
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(0, 1, 0, 64);
	}

	// *** Panel de creditos ***
	// Crea el panel de creditos
	CreditsCreateMsgPanel();
	// Y manda la paleta a la VRAM
	N3_VramPalette(CREDITS_MSG_PAL, CREDITS_MSG_PAL);

	// *** Inicializa la niebla ***
	// La niebla afecta a todos los poligonos
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FOG);
	// Habilita la niebla
	glEnable(GL_FOG);
	// Define el factor de profundidad de la niebla
	glFogShift(3);
	// Color de la niebla
	glFogColor(0, 0, 0, 0);
	// Crea la tabla de densidad de la niebla
	for (f = 0; f < 31; f ++) {
		glFogDensity(f, (f << 2));
	}
	glFogDensity(31, 127);
	// Define a partir de que punto empieza la niebla
	glFogOffset(0x6000 + (128 << 4));

}



// Funcion CreditsMoveTunnel();
void CreditsMoveTunnel(void) {

	// Variables
	u8 id = 0;
	u8 x = 0;
	u8 y = 0;

	// Movimiento camara
	CREDITSWORLD.cam_z += CREDITS_CAM_SPEED;

	// Si la camara se ha movido 1 bloque
	if (CREDITSWORLD.cam_z > CREDITS_BLOCK_SIZE) {

		// Resetea la posicion de la camara
		CREDITSWORLD.cam_z -= CREDITS_BLOCK_SIZE;

		// Realinea los bloques de las paredes [0][1]
		for (id = 0; id <= 1; id ++) {
			for (y = 1; y < CREDITS_WALL_DEEP; y ++) {
				for (x = 0; x < CREDITS_WALL_HEIGHT; x ++) {
					CREDITSWORLD.height[id][(y - 1)][x] = CREDITSWORLD.height[id][y][x];
					CREDITSWORLD.texture[id][(y - 1)][x] = CREDITSWORLD.texture[id][y][x];
				}
			}
		}
		// Calcula la nueva fila de bloques de las paredes [0][1]
		for (id = 0; id <= 1; id ++) {
			for (x = 0; x < CREDITS_WALL_HEIGHT; x ++) {
				CREDITSWORLD.height[id][(CREDITS_WALL_DEEP - 1)][x] = (int)((rand() % CREDITS_WALL_MAX) + CREDITS_WALL_MIN);
				CREDITSWORLD.texture[id][(CREDITS_WALL_DEEP - 1)][x] = (int)((rand() % CREDITS_WALL_TEX_LASTNUM) + 1 + CREDITS_WALL_TEX);
			}
		}

		// Realinea los bloques del techo [2]
		for (y = 1; y < CREDITS_CEIL_DEEP; y ++) {
			for (x = 0; x < CREDITS_CEIL_WIDTH; x ++) {
				CREDITSWORLD.height[2][(y - 1)][x] = CREDITSWORLD.height[2][y][x];
				CREDITSWORLD.texture[2][(y - 1)][x] = CREDITSWORLD.texture[2][y][x];
			}
		}
		// Calcula la nueva fila de bloques del techo [2]
		for (x = 0; x < CREDITS_CEIL_WIDTH; x ++) {
			CREDITSWORLD.height[2][(CREDITS_CEIL_DEEP - 1)][x] = (int)((rand() % CREDITS_CEIL_MAX) + CREDITS_CEIL_MIN);
			CREDITSWORLD.texture[2][(CREDITS_CEIL_DEEP - 1)][x] = (int)((rand() % CREDITS_CEIL_TEX_LASTNUM) + 1 + CREDITS_CEIL_TEX);
		}

		// Suelo [3]
		for (id = 0; id < 2; id ++ ) {
			// Realinea los bloques del suelo
			for (y = 1; y < CREDITS_FLOOR_DEEP; y ++) {
				for (x = 0; x < CREDITS_FLOOR_WIDTH; x ++) {
					CREDITSWORLD.height[3][(y - 1)][x] = CREDITSWORLD.height[3][y][x];
					CREDITSWORLD.texture[3][(y - 1)][x] = CREDITSWORLD.texture[3][y][x];
				}
			}
			// Calcula la nueva fila de bloques del suelo
			for (x = 0; x < CREDITS_FLOOR_WIDTH; x ++) {
				CREDITSWORLD.height[3][(CREDITS_FLOOR_DEEP - 1)][x] = (int)((rand() % CREDITS_FLOOR_MAX) + CREDITS_FLOOR_MIN);
				CREDITSWORLD.texture[3][(CREDITS_FLOOR_DEEP - 1)][x] = (int)((rand() % CREDITS_FLOOR_TEX_LASTNUM) + 1 + CREDITS_FLOOR_TEX);
			}
		}

	}

}



// Funcion CreditsRender3dWorld
void CreditsRender3dWorld(void) {

	// Variables locales
	s32 up = 0;		// Orientacion

	// Define donde esta el techo
	if (GAMEOPTIONS[0].lefthanded) {
		up = -(128 << 4);
	} else {
		up = (128 << 4);
	}


	////////////////////////////////////////////////////
	// Renderizaciones en el angulo de camara general //
	////////////////////////////////////////////////////

	// Primero de todo, guarda la matriz
	glPushMatrix();

	// Y posiciona la camara
	gluLookAtf32(	(128 << 4), (96 << 4), (128 << 4),
					(128 << 4), (96 << 4), 0,
					0 , up, 0);

	// Ejecuta los renders necesarios
	CreditsRenderTunnel();		// Renderiza el tunel
	CreditsRenderMsgPanel();	// Renderiza el panel de creditos

	// Recupera la matriz
	glPopMatrix(1);



	//////////////////////////////////////////////
	// Renderizaciones en otro angulo de camara //
	//////////////////////////////////////////////

	// Renderiza el cubo del subscreen
	CreditsRenderCube(up);



	////////////////////////////////
	// Mandalo todo a la pantalla //
	////////////////////////////////

	// Manda la escena 3D a la pantalla
	glFlush(0);

}



// Funcion CreditsRenderTunnel();
void CreditsRenderTunnel(void) {

	// Variables locales
	s32 a = 0;		// Control del bucle
	s32 b = 0;

	s32 x = 0;		// Coordenadas de dibujado
	s32 y = 0;
	s32 z = 0;

	u8 idx = 0;		// Incides de alturas
	u8 idy = 0;

	s32 size = CREDITS_BLOCK_SIZE;	// Tamaño de las piezas
	s32 side = 0;
	u16 tex = 0;					// Textura

	// Vertices
	typedef struct {
		s32 x;
		s32 y;
		s32 z;
	} vertex_info;
	vertex_info vertex[8];


	///////////////////////////////
	// Dibuja el lateral derecho //
	// Id = 0					 //
	///////////////////////////////

	// Inicializa
	y = 0;
	z = CREDITSWORLD.cam_z;
	idx = 0;
	idy = 0;

	for (a = 0; a < CREDITS_WALL_DEEP; a ++) {

		// Resetea la X
		x = 0;
		idx = 0;

		for (b = 0; b < CREDITS_WALL_HEIGHT; b ++) {

			// Obten la altura de la pieza
			side = CREDITSWORLD.height[0][idy][idx];

			// Y la textura
			tex = CREDITSWORLD.texture[0][idy][idx];

			// Calcula los vertices
			vertex[0].x = (x + size);
			vertex[0].y = y;
			vertex[0].z = (z - size);

			vertex[1].x = x;
			vertex[1].y = y;
			vertex[1].z = (z - size);

			vertex[2].x = x;
			vertex[2].y = y;
			vertex[2].z = z;

			vertex[3].x = (x + size);
			vertex[3].y = y;
			vertex[3].z = z;

			vertex[4].x = ((x + size) - side);
			vertex[4].y = (y + side);
			vertex[4].z = ((z - size) + side);

			vertex[5].x = (x + side);
			vertex[5].y = (y + side);
			vertex[5].z = ((z - size) + side);

			vertex[6].x = (x + side);
			vertex[6].y = (y + side);
			vertex[6].z = (z - side);

			vertex[7].x = ((x + size) - side);
			vertex[7].y = (y + side);
			vertex[7].z = (z - side);

			// Dibuja la cara superior [0]
			N3_TexturedQuad(	vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								tex, CREDITS_WALL_PAL);
			// Dibuja borde 3 - 0 [1]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 0 - 1 [2]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 1 - 2 [3]
			N3_TexturedQuad(	vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 2 - 3 [4]
			N3_TexturedQuad(	vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);

			// Calcula el siguiente bloque
			x += size;
			idx += 1;

		}

		// Calcula la siguiente fila
		z -= size;
		idy += 1;

	}



	/////////////////////////////////
	// Dibuja el lateral Izquierdo //
	// Id = 1					   //
	/////////////////////////////////

	// Inicializa
	y = (191 << 4);
	z = CREDITSWORLD.cam_z;
	idx = 0;
	idy = 0;

	for (a = 0; a < CREDITS_WALL_DEEP; a ++) {

		// Resetea la X
		x = 0;
		idx = 0;

		for (b = 0; b < CREDITS_WALL_HEIGHT; b ++) {

			// Obten la altura de la pieza
			side = CREDITSWORLD.height[1][idy][idx];

			// Y la textura
			tex = CREDITSWORLD.texture[1][idy][idx];

			// Calcula los vertices
			vertex[0].x = (x + size);
			vertex[0].y = y;
			vertex[0].z = z;

			vertex[1].x = x;
			vertex[1].y = y;
			vertex[1].z = z;

			vertex[2].x = x;
			vertex[2].y = y;
			vertex[2].z = (z - size);

			vertex[3].x = (x + size);
			vertex[3].y = y;
			vertex[3].z = (z - size);

			vertex[4].x = ((x + size) - side);
			vertex[4].y = (y - side);
			vertex[4].z = (z - side);

			vertex[5].x = (x + side);
			vertex[5].y = (y - side);
			vertex[5].z = (z - side);

			vertex[6].x = (x + side);
			vertex[6].y = (y - side);
			vertex[6].z = ((z - size) + side);

			vertex[7].x = ((x + size) - side);
			vertex[7].y = (y - side);
			vertex[7].z = ((z - size) + side);

			// Dibuja la cara superior [0]
			N3_TexturedQuad(	vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								tex, CREDITS_WALL_PAL);
			// Dibuja borde 3 - 0 [1]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 0 - 1 [2]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 1 - 2 [3]
			N3_TexturedQuad(	vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);
			// Dibuja borde 2 - 3 [4]
			N3_TexturedQuad(	vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_WALL_TEX, CREDITS_WALL_PAL);

			// Calcula el siguiente bloque
			x += size;
			idx += 1;

		}

		// Calcula la siguiente fila
		z -= size;
		idy += 1;

	}


	/////////////////////
	// Dibuja el techo //
	// Id = 2		   //
	/////////////////////

	// Inicializa
	x = (255 << 4);
	z = CREDITSWORLD.cam_z;
	idx = 0;
	idy = 0;

	for (a = 0; a < CREDITS_CEIL_DEEP; a ++) {

		// Resetea la Y
		y = 0;
		idx = 0;

		for (b = 0; b < CREDITS_CEIL_WIDTH; b ++) {

			// Obten la altura de la pieza
			side = CREDITSWORLD.height[2][idy][idx];

			// Y la textura
			tex = CREDITSWORLD.texture[2][idy][idx];

			// Calcula los vertices
			vertex[0].x = x;
			vertex[0].y = (y + size);
			vertex[0].z = z;

			vertex[1].x = x;
			vertex[1].y = (y + size);
			vertex[1].z = (z - size);

			vertex[2].x = x;
			vertex[2].y = y;
			vertex[2].z = (z - size);

			vertex[3].x = x;
			vertex[3].y = y;
			vertex[3].z = z;

			vertex[4].x = (x - side);
			vertex[4].y = ((y + size) - side);
			vertex[4].z = (z - side);

			vertex[5].x = (x - side);
			vertex[5].y = ((y + size) - side);
			vertex[5].z = ((z - size) + side);

			vertex[6].x = (x - side);
			vertex[6].y = (y + side);
			vertex[6].z = ((z - size) + side);

			vertex[7].x = (x - side);
			vertex[7].y = (y + side);
			vertex[7].z = (z - side);

			// Dibuja la cara superior [0]
			N3_TexturedQuad(	vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								tex, CREDITS_CEIL_PAL);
			// Dibuja borde 3 - 0 [1]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_CEIL_TEX, CREDITS_CEIL_PAL);
			// Dibuja borde 0 - 1 [2]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								CREDITS_CEIL_TEX, CREDITS_CEIL_PAL);
			// Dibuja borde 1 - 2 [3]
			N3_TexturedQuad(	vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								CREDITS_CEIL_TEX, CREDITS_CEIL_PAL);
			// Dibuja borde 2 - 3 [4]
			N3_TexturedQuad(	vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_CEIL_TEX, CREDITS_CEIL_PAL);

			// Calcula el siguiente bloque
			y += size;
			idx += 1;

		}

		// Calcula la siguiente fila
		z -= size;
		idy += 1;

	}


	/////////////////////
	// Dibuja el suelo //
	// Id = 3		   //
	/////////////////////

	// Inicializa
	x = 0;
	z = CREDITSWORLD.cam_z;
	idx = 0;
	idy = 0;
	size = (size >> 1);

	for (a = 0; a < CREDITS_FLOOR_DEEP; a ++) {

		// Resetea la Y
		y = 0;
		idx = 0;

		for (b = 0; b < CREDITS_FLOOR_WIDTH; b ++) {

			// Obten la altura de la pieza
			side = CREDITSWORLD.height[3][idy][idx];

			// Y la textura
			tex = CREDITSWORLD.texture[3][idy][idx];

			// Calcula los vertices
			vertex[0].x = x;
			vertex[0].y = (y + size);
			vertex[0].z = (z - size);

			vertex[1].x = x;
			vertex[1].y = (y + size);
			vertex[1].z = z;

			vertex[2].x = x;
			vertex[2].y = y;
			vertex[2].z = z;

			vertex[3].x = x;
			vertex[3].y = y;
			vertex[3].z = (z - size);

			vertex[4].x = (x + side);
			vertex[4].y = ((y + size) - side);
			vertex[4].z = ((z - size) + side);

			vertex[5].x = (x + side);
			vertex[5].y = ((y + size) - side);
			vertex[5].z = (z - side);

			vertex[6].x = (x + side);
			vertex[6].y = (y + side);
			vertex[6].z = (z - side);

			vertex[7].x = (x + side);
			vertex[7].y = (y + side);
			vertex[7].z = ((z - size) + side);

			// Dibuja la cara superior [0]
			N3_TexturedQuad(	vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								tex, CREDITS_FLOOR_PAL);
			// Dibuja borde 3 - 0 [1]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_FLOOR_TEX, CREDITS_FLOOR_PAL);
			// Dibuja borde 0 - 1 [2]
			N3_TexturedQuad(	vertex[0].x, vertex[0].y, vertex[0].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[4].x, vertex[4].y, vertex[4].z,
								CREDITS_FLOOR_TEX, CREDITS_FLOOR_PAL);
			// Dibuja borde 1 - 2 [3]
			N3_TexturedQuad(	vertex[5].x, vertex[5].y, vertex[5].z,
								vertex[1].x, vertex[1].y, vertex[1].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								CREDITS_FLOOR_TEX, CREDITS_FLOOR_PAL);
			// Dibuja borde 2 - 3 [4]
			N3_TexturedQuad(	vertex[7].x, vertex[7].y, vertex[7].z,
								vertex[6].x, vertex[6].y, vertex[6].z,
								vertex[2].x, vertex[2].y, vertex[2].z,
								vertex[3].x, vertex[3].y, vertex[3].z,
								CREDITS_FLOOR_TEX, CREDITS_FLOOR_PAL);

			// Calcula el siguiente
			y += size;
			idx += 1;

		}

		// Calcula el siguiente
		z -= size;
		idy += 1;

	}

}



// Funcion CreditsLoadMsgFiles();
void CreditsLoadMsgFiles(void) {

	// Variables
	FILE* file_id;			// Puntero de fichero
	char filename[128];		// Variable para almacenar el path al archivo
	char lang[4];			// Idioma

	// Inicializa los buffers
	free(CREDITS.font_buffer);
	CREDITS.font_buffer = NULL;
	free(CREDITS.image_buffer);
	CREDITS.image_buffer = NULL;
	free(CREDITS.text_buffer);
	CREDITS.text_buffer = NULL;
	CREDITS.text_pos = 0;

	// Carga el archivo de fuentes
	sprintf(filename, "%s/credits/tex/msg/font.img", NF_ROOTFOLDER);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		CREDITS.font_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		CREDITS.font_buffer = (char*) calloc (CREDITS.font_size, sizeof(char));
		if (CREDITS.font_buffer == NULL) {		// Si no hay suficiente RAM libre
			NF_Error(102, NULL, CREDITS.font_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(CREDITS.font_buffer, 1, CREDITS.font_size, file_id);
	} else {	// Si el archivo no existe...
		NF_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Carga el archivo de texturas
	sprintf(filename, "%s/credits/tex/msg/papyre.img", NF_ROOTFOLDER);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		CREDITS.image_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		CREDITS.image_buffer = (char*) calloc (CREDITS.image_size, sizeof(char));
		if (CREDITS.image_buffer == NULL) {		// Si no hay suficiente RAM libre
			NF_Error(102, NULL, CREDITS.image_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(CREDITS.image_buffer, 1, CREDITS.image_size, file_id);
	} else {	// Si el archivo no existe...
		NF_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Carga el archivo de creditos
	if (GAMEOPTIONS[0].language) {
		sprintf(lang, "en");
	} else {
		sprintf(lang, "es");
	}
	sprintf(filename, "%s/credits/text/credits_%s.txt", NF_ROOTFOLDER, lang);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(file_id, 0, SEEK_END);
		CREDITS.text_size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		CREDITS.text_buffer = (char*) calloc (CREDITS.text_size, sizeof(char));
		if (CREDITS.text_buffer == NULL) {		// Si no hay suficiente RAM libre
			NF_Error(102, NULL, CREDITS.text_size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(CREDITS.text_buffer, 1, CREDITS.text_size, file_id);
	} else {	// Si el archivo no existe...
		NF_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

}



// Funcion CreditsCreateMsgPanel();
void CreditsCreateMsgPanel(void) {

	// Variables
	u16 n = 0;
	u8 row = 0;
	u8 column = 0;
	s16 angle = 0;

	// Reserva los slots de texturas de n3 Lib para las partes del panel
	for (n = CREDITS_MSG_TEX; n <= CREDITS_MSG_TEX_LASTNUM; n ++) {

		// Resetea el buffer
		free(N3_TEX_GFX[n].buffer);
		N3_TEX_GFX[n].buffer = NULL;

		// Reserva el espacio en RAM
		N3_TEX_GFX[n].size = 1024;
		N3_TEX_GFX[n].buffer = (char*) calloc (N3_TEX_GFX[n].size, sizeof(char));
		// Si no hay suficiente RAM libre, error
		if (N3_TEX_GFX[n].buffer == NULL) {
			N3_Error(104, NULL, N3_TEX_GFX[n].size);
		}

		// Guarda las medidas de la textura
		N3_TEX_GFX[n].width = 32;		// Ancho
		N3_TEX_GFX[n].height = 32;		// Alto

		// Y marca esta ID como usada
		N3_TEX_GFX[n].available = false;

		// Ahora, manda la textura vacia a la VRAM
		N3_VramTexture(n, n, false);

	}

	// Inicializa la animacion del panel
	for (row = 0; row < 8; row ++) {
		for (column = 0; column < 6; column ++) {
			CREDITSWORLD.panel_angle[row][column] = angle;
			angle += 64;
			if (angle > 511) angle -= 512;
			CREDITSWORLD.panel_z[row][column] = 0;
		}
	}

	// Variables iniciales del panel
	CREDITSWORLD.panel_thread = 0;			// Thread a ejecutar
	CREDITSWORLD.panel_cnt = 0;				// Contador generico
	CREDITSWORLD.panel_pos = -(320 << 4);	// Posicion
	CREDITSWORLD.done = false;				// Flag de finalizacion

	// Ahora, crea el panel con la primera pagina de texto
	CreditsUpdateMsgPanel();

}


// Funcion CreditsUpdateMsgPanel();
bool CreditsUpdateMsgPanel(void) {

	// Variables
	bool status = false;	// Estado de los creditos

	// Resetea las texturas del fondo del panel
	CreditsDefaultMsgPanelTextures();

	// Renderiza el texto correspondiende desde el archivo
	status = CreditsWriteTextPage();

	// Marca que se deben actualizar las texturas en VRAM
	CREDITSWORLD.panel_update = true;
	CREDITSWORLD.panel_texdone = 0;

	// Devuelve si se han finalizado los creditos
	return status;

}



// Funcion CreditsUpdateVramPanelTextures();
void CreditsUpdateVramPanelTextures(void) {

	// Variables
	s16 id = (CREDITSWORLD.panel_texdone + CREDITS_MSG_TEX);

	// Actualiza la textura en la VRAM
	N3_FreeTexture(id);
	N3_VramTexture(id, id, false);

	// Calcula la siguiente textura a actualizar
	CREDITSWORLD.panel_texdone ++;

	// Si ya se han actualizado todas, indicalo
	if (CREDITSWORLD.panel_texdone == 35) {
		CREDITSWORLD.panel_update = false;
		CREDITSWORLD.panel_texdone = 0;
	}

}



// Funcion CreditsDefaultMsgPanelTextures();
void CreditsDefaultMsgPanelTextures(void) {

	// Variables
	u16 id = CREDITS_MSG_TEX;		// Id de la textura de destino
	u16 row = 0;					// Filas
	u16 column = 0;					// Columnas
	u32 origin = 0;					// Desplazamiento en buffer origen
	u32 destination = 0;			// Desplazamiento en buffer destino
	u16 line = 0;					// Linea de dibujado en la textura destino


	// Copia la textura base a cada buffer
	for (row = 0; row < 7; row ++) {

		for (column = 0; column < 5; column ++) {

			// Copia cada linea
			for (line = 0; line < 32; line ++) {
				// Calcula los desplazamientos dentro del buffer
				origin = ((row * 5120) + (column << 5) + (line * 160));
				destination = (line << 5);
				memcpy((N3_TEX_GFX[id].buffer + destination), (CREDITS.image_buffer + origin), 32);
			}

			// Siguiente textura
			id ++;

		}

	}

}



// CreditsWriteTextPage();
bool CreditsWriteTextPage(void) {

	// Variables
	char text[13][18];				// 13 lineas, 16 letras (16 + \n + seguridad)
	bool endofpage = false;			// Flag de fin de pagina
	u16 row = 0;					// Fila
	u16 column = 0;					// Columna
	u16 line = 0;					// Linea de impresion
	u16 y = 0;						// Posicion inicial
	bool end = false;				// Final de archivo

	// Resetea el buffer de texto
	memset(text, 0, sizeof(text));

	// Crea una pagina
	while (!endofpage) {

		// Si no es fin de linea...
		if (CREDITS.text_buffer[CREDITS.text_pos] != 0x0D) {

			// Segun el caracter leido....
			switch (CREDITS.text_buffer[CREDITS.text_pos]) {
				case 0x3E:		// >	Siguiente pagina
					// Muevete mas alla del siguiente salto de linea
					CREDITS.text_pos += 2;
					// Marca el final de pagina de texto
					endofpage = true;
					break;
				case 0x3C:		// <	Fin de texto
					endofpage = true;
					end = true;
					break;
				default:		// Imprime el caracter
					text[row][column] = CREDITS.text_buffer[CREDITS.text_pos];
					column ++;
					if (column > 15) column = 15;	// Limite de la linea (error trap)
					break;
			}

			// Siguiente caracter en el buffer
			CREDITS.text_pos ++;

		} else {

			// Salta los dos caracteres del retorno de linea
			CREDITS.text_pos += 2;
			// Y salta de linea en el buffer de texto
			row ++;
			column = 0;
			// Verifica los limites de lineas (error trap)
			if (row == 13) endofpage = true;

		}

	}

	// Imprime la pagina de texto las texturas del panel
	y = (112 - ((row << 4) >> 1));
	for (line = 0; line < row; line ++) {
		CreditsWriteTextOnTexture(text[line], y);
		y += 16;
	}

	// Devuelve el resultado de si es el final del texto
	return end;
	
}



// Funcion CreditsWriteTextOnTexture();
void CreditsWriteTextOnTexture(const char* text, u16 pos_y) {

	// Variables
	u16 n = 0;				// Variable de uso general
	s16 value = 0;			// Valor
	u16 tsize = 0;			// Calcula el numero de letras del texto
	u16 x = 0;				// Posicion X del dibujado
	u16 y = pos_y;			// Posicion Y del dibujado
	u16 temp_x = 0;			// Temporales para calculos de posicion
	u16 temp_y = 0;
	u32 origin = 0;			// Desplazamiento del puntero de origen
	u32 destination = 0;	// Desplazamiento del puntero de destino
	u16 row = 0;			// Fila
	u16 column = 0;			// Columna
	u8 line = 0;			// Linea de dibujado de la letra
	u8 pixel = 0;			// Pixel de dibujado de la letra
	u16 texture = 0;		// Textura de destino
	u8 block_x = 0;			// Bloque en el que se escribira
	u8 block_y = 0;
	u8 antialias = 0;		// Antialias del texto


	// Longitud de la linea (max 16 caracteres de 8x16)
	tsize = strlen(text);
	if (tsize > 16) tsize = 16;
	// la posicion X del texto
	x = (80 - ((tsize << 3) >> 1));

	// Crea el buffer temporal donde almacenar el texto convertido
	u8* string;
	string = NULL;
	string = (u8*) calloc (tsize, sizeof(u8));

	// Almacena en el buffer temporal el valor de los caracteres
	for (n = 0; n < tsize; n ++) {
		value = ((int)(text[n])) - 32;	// Resta 32 al valor entrado
		if (value < 0) value = 0;
		string[n] = value;
		// Si es un caracter especial...
		if (string[n] > 95) {
			// Resetea el caracter...
			string[n] = 0;
			// Verifica caracteres especiales
			switch (((int)(text[n]))) {
				// Caracteres especiales
				case 199:	// Ç
					string[n] = 96;
					break;
				case 231:	// ç
					string[n] = 97;
					break;
				case 209:	// Ñ
					string[n] = 98;
					break;
				case 241:	// ñ
					string[n] = 99;
					break;
				// Acentos Mayusculas
				case 193:	// Á
					string[n] = 100;
					break;
				case 201:	// É
					string[n] = 101;
					break;
				case 205:	// Í
					string[n] = 102;
					break;
				case 211:	// Ó
					string[n] = 103;
					break;
				case 218:	// Ú
					string[n] = 104;
					break;
				// Acentos Minusculas
				case 225:	// á
					string[n] = 105;
					break;
				case 233:	// é
					string[n] = 106;
					break;
				case 237:	// í
					string[n] = 107;
					break;
				case 243:	// ó
					string[n] = 108;
					break;
				case 250:	// ú
					string[n] = 109;
					break;
				// Dieresis
				case 239:	// ï
					string[n] = 110;
					break;
				case 252:	// ü
					string[n] = 111;
					break;
				// Admiracion e interrogante (Español)
				case 161:	// ¡
					string[n] = 112;
					break;
				case 191:	// ¿
					string[n] = 113;
					break;
				// Caracter invalido
				default:	
					string[n] = 0;
					break;
			}
		}
	}


	// Una vez analizado el texto, imprimelo en la textura
	for (n = 0; n < tsize; n ++) {
		// Calcula la fila y columna de origen
		row = (int)(string[n] >> 5);
		column = (string[n] - (row << 5));
		// Dos pases para el efecto antialias del texto
		for (antialias = 0; antialias < 2; antialias ++) {
			// Dibuja cada linea de la letra
			for (line = 0; line < 16; line ++) {
				// Dibuja cada pixel de la linea
				for (pixel = 0; pixel < 8; pixel ++) {
					// Calcula la posicion de origen
					origin = ((row << 12) + (column << 3) + (line << 8) + pixel);
					// Calcula el buffer y posicion de destino
					temp_x = (x + pixel - antialias);				// Coordenada de dibujado
					temp_y = (y + line);				
					block_y = (int)(temp_y >> 5);		// Fila y columna en el panel
					block_x = (int)(temp_x >> 5);
					texture = (CREDITS_MSG_TEX + (block_y * 5) + block_x);	// Calcula el nº de textura de destino
					destination = (((temp_y - (block_y << 5)) << 5) + (temp_x - (block_x << 5)));
					// Escribe el pixel, si es necesario, en la textura de destino
					if (CREDITS.font_buffer[origin]) N3_TEX_GFX[texture].buffer[destination] = (antialias + 1);
				}
			}
		}
		// Calcula la posicion de la siguiente letra
		x += 8;
	}


	// Una vez terminado, vacia el buffer temporal
	free(string);

}



// Funcion CreditsRenderMsgPanel();
void CreditsRenderMsgPanel(void) {

	// Variables
	u16 row = 0;					// Fila
	u16 column = 0;					// Columna
	s32 x = (240 << 4);				// Coordenadas de dibujado
	s32 y = (176 << 4);
	s32 z = CREDITSWORLD.panel_pos;
	u16 texture = CREDITS_MSG_TEX;	// Textura del poligono
	s16 sin = 0;	// Calculo del seno

	// Calcula el movimiento ondulante
	for (row = 0; row < 8; row ++) {
		for (column = 0; column < 6; column ++) {
			CREDITSWORLD.panel_angle[row][column] += CREDITS_PANEL_WAVE_SPEED;
			if (CREDITSWORLD.panel_angle[row][column] > 511) CREDITSWORLD.panel_angle[row][column] -= 512;
			sin = sinLerp((CREDITSWORLD.panel_angle[row][column] << 6));
			if (sin >= 0) {
				CREDITSWORLD.panel_z[row][column] = (sin >> 5);
			} else {
				CREDITSWORLD.panel_z[row][column] = -(-sin >> 5);
			}
		}
	}

	// Si es el main engine, renderizalo, si no, ignoralo
	if (!CREDITSWORLD.screen) {
		// Dibuja el panel de creditos
		for (row = 0; row < 7; row ++) {
			y = (176 << 4);
			for (column = 0; column < 5; column ++) {
				N3_TexturedQuad(	x, y, (z + CREDITSWORLD.panel_z[row][column]),
									(x - CREDITS_PANEL_SIZE), y, (z + CREDITSWORLD.panel_z[(row + 1)][column]),
									(x - CREDITS_PANEL_SIZE), (y - CREDITS_PANEL_SIZE), (z + CREDITSWORLD.panel_z[(row + 1)][(column + 1)]),
									x, (y - CREDITS_PANEL_SIZE), (z + CREDITSWORLD.panel_z[row][(column + 1)]),
									texture, CREDITS_MSG_PAL);
				texture ++;
				y -= (32 << 4);
			}
			x -= (32 << 4);
		}
	}

}



// Funcion CreditsMovePanel();
bool CreditsMovePanel(void) {

	// Variables
	bool result = false;
	s16 speed = 0;

	// Thread de ejecucion
	switch (CREDITSWORLD.panel_thread) {

		// Espera la entrada de creditos
		case 0:
			CREDITSWORLD.panel_cnt ++;
			if (CREDITSWORLD.panel_cnt == CREDITS_PANEL_DELAY) {
				CREDITSWORLD.panel_thread = 1;
			}
			break;

		// Acerca el panel a la camara
		case 1:
			speed = ((-CREDITSWORLD.panel_pos) >> 5);
			if (speed < CREDITS_PANEL_MIN_SPEED) speed = CREDITS_PANEL_MIN_SPEED;
			CREDITSWORLD.panel_pos += speed;
			if (CREDITSWORLD.panel_pos > 0) {
				CREDITSWORLD.panel_pos = 0;
				CREDITSWORLD.panel_cnt = 0;
				CREDITSWORLD.panel_thread = 2;
			}
			break;

		// Espera el tiempo predeterminado
		case 2:
			CREDITSWORLD.panel_cnt ++;
			if (CREDITSWORLD.panel_cnt == CREDITS_PANEL_DELAY) {
				CREDITSWORLD.panel_thread = 3;
			}
			break;

		// Aleja el panel de la camara
		case 3:
			speed = ((-CREDITSWORLD.panel_pos) >> 5);
			if (speed < CREDITS_PANEL_MIN_SPEED) speed = CREDITS_PANEL_MIN_SPEED;
			CREDITSWORLD.panel_pos -= speed;
			if (CREDITSWORLD.panel_pos < -(320 << 4)) {
				CREDITSWORLD.panel_pos = -(320 << 4);
				// Si no es la ultima pagina de texto, dibujala
				if (!CREDITSWORLD.done) {
					CREDITSWORLD.done = CreditsUpdateMsgPanel();
					CREDITSWORLD.panel_thread = 1;
				} else {
					CREDITSWORLD.panel_thread = 255;
					result = true;
				}
			}
			break;

		default:
			break;

	}

	// Devuelve el resultado de la ejecucion
	return result;

}



// Funcion CreditsRenderCube();
void CreditsRenderCube(s32 up) {

	// Variables
	s16 x = CREDITS_CUBE_X;
	s16 y = CREDITS_CUBE_Y;
	s16 z = CREDITS_CUBE_Z;
	s16 size = CREDITS_CUBE_SIZE;
	s32 cx = 0;
	s32 cy = 0;
	s32 cz = 0;
	s32 angle = 0;

	// Calculo de la rotacion
	CREDITSWORLD.cube_angle ++;
	if (CREDITSWORLD.cube_angle > 511) CREDITSWORLD.cube_angle -= 512;
	angle = (CREDITSWORLD.cube_angle << 6);

	// Si es el subengine, renderizalo
	if (CREDITSWORLD.screen) {

		// Primero de todo, guarda la matriz
		glPushMatrix();

		// Y posiciona la camara
		gluLookAtf32(	(128 << 4), (96 << 4), (128 << 4),
						(128 << 4), (96 << 4), 0,
						0 , up, 0);

		// Calcula el centro
		cx = (x - (size >> 1));
		cy = (y - (size >> 1));
		cz = (z - (size >> 1));

		// Muevete al centro
		glTranslatef32(cx, cy, cz);

		// Aplica la rotacion
		glRotateXi(angle);
		glRotateYi(angle);

		// Vuelve a la posicion original
		glTranslatef32(-cx, -cy, -cz);

		// Dibuja la cara 1 (frente)
		N3_TexturedQuad(	x, y, z,
							(x - size), y, z,
							(x - size), (y - size), z,
							x, (y - size), z,
							(CREDITS_WALL_TEX + 9), CREDITS_WALL_PAL);

		// Dibuja la cara 2 (arriba)
		N3_TexturedQuad(	x, y, z,
							x, (y - size), z,
							x, (y - size), (z - size),
							x, y, (z - size),
							(CREDITS_WALL_TEX + 10), CREDITS_WALL_PAL);

		// Dibuja la cara 3 (derecha)
		N3_TexturedQuad(	x, (y - size), z,
							(x - size), (y - size), z,
							(x - size), (y - size), (z - size),
							x, (y - size), (z - size),
							(CREDITS_WALL_TEX + 11), CREDITS_WALL_PAL);

		// Dibuja la cara 4 (abajo)
		N3_TexturedQuad(	(x - size), y, z,
							(x - size), y, (z - size),
							(x - size), (y - size), (z - size),
							(x - size), (y - size), z,
							(CREDITS_WALL_TEX + 12), CREDITS_WALL_PAL);

		// Dibuja la cara 5 (izquierda)
		N3_TexturedQuad(	x, y, z,
							x, y, (z - size),
							(x - size), y, (z - size),
							(x - size), y, z,
							(CREDITS_WALL_TEX + 13), CREDITS_WALL_PAL);

		// Dibuja la cara 6 (trasera)
		N3_TexturedQuad(	x, (y - size), (z - size),
							(x - size), (y - size), (z - size),
							(x - size), y, (z - size),
							x, y, (z - size),
							(CREDITS_WALL_TEX + 14), CREDITS_WALL_PAL);

		// Recupera la matriz
		glPopMatrix(1);

	}

}
