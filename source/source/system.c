
// Mind Maze - Funciones de sistema
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
#include <math.h>

// Includes propietarios NDS
#include <nds.h>

// Includes NightFox's Lib
#include <nf_lib.h>
#include <n3_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion BootGame();
void BootGame(void) {

	// Pantalla de espera inicializando NitroFS
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);	
	consoleDemoInit();
	if (NF_GetLanguage() == 5) {
		iprintf("\n Iniciando NitroFS,\n por favor, espere.\n");
	} else {
		iprintf("\n NitroFS init. Please wait.\n");
	}
	swiWaitForVBlank();

	// Define el ROOT e inicializa el sistema de archivos
	NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NitroFS
	N3_RootPath("NITROFS");

	// Inicializa el motor 2D
	NF_Set2D(0, 0);				// Modo 2D_0 en ambas pantallas
	NF_Set2D(1, 0);	

	// Inicializa el sonido
	InitSound();

	// Inicializa los fondos tileados
	NF_InitTiledBgBuffers();	// Inicializa los buffers para almacenar fondos
	NF_InitTiledBgSys(0);		// Inicializa los fondos Tileados para la pantalla superior
	NF_InitTiledBgSys(1);		// Iniciliaza los fondos Tileados para la pantalla inferior

	// Inicializa los Sprites
	NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
	NF_InitSpriteSys(0, 128);		// Inicializa los sprites para la pantalla superior
	NF_InitSpriteSys(1, 128);		// Inicializa los sprites para la pantalla inferior

	// Inicializa los buffers de texturas
	N3_InitBuffers();

	// Inicializa los mapas de colision
	NF_InitCmapBuffers();

	// Inicializa el engine de texto
	NF_InitTextSys(0);
	NF_InitTextSys(1);

	// Inicializa los buffers para guardar fondos en formato BITMAP
	NF_Init16bitsBgBuffers();
	NF_Init8bitsBgBuffers();

	// Inicializa el BackBuffer de 16 bits
	// Usalo solo una vez antes de habilitarlo
	NF_Init16bitsBackBuffer(0);
	NF_Init16bitsBackBuffer(1);

	// Define el color del backdrop a negro en ambas pantallas
	setBackdropColor(0);
	setBackdropColorSub(0);


	//////////////////////////////////////////////////
	//												//
	// Inicializaciones especificas de Mind Maze	//
	//												//
	//////////////////////////////////////////////////

	// Inicializa los buffers de video
	NF_InitAnimationBuffers();

	// Inicializa las estructuras
	InitCustomStructs();

	// Inicializa los datos de las variables globales
	LoadGlobalDefaults();

}



// Funcion InitCustomStructs
void InitCustomStructs(void) {

	// Estructuras usadas en los creditos
	CREDITS.font_buffer = NULL;
	CREDITS.font_size = 0;
	CREDITS.image_buffer = NULL;
	CREDITS.image_size = 0;
	CREDITS.text_buffer = NULL;
	CREDITS.text_size = 0;

}




// Funcion ReadKeypad();
void ReadKeypad(void) {

	// Variables
	u16 keys = 0;

	// Lee el teclado via Libnds
	scanKeys();

	// Teclas "HELD"
	keys = keysHeld();
	if (keys & KEY_UP) {KEYHELD.up = true;} else {KEYHELD.up = false;}
	if (keys & KEY_DOWN) {KEYHELD.down = true;} else {KEYHELD.down = false;}
	if (keys & KEY_LEFT) {KEYHELD.left = true;} else {KEYHELD.left = false;}
	if (keys & KEY_RIGHT) {KEYHELD.right = true;} else {KEYHELD.right = false;}
	if (keys & KEY_A) {KEYHELD.a = true;} else {KEYHELD.a = false;}
	if (keys & KEY_B) {KEYHELD.b = true;} else {KEYHELD.b = false;}
	if (keys & KEY_X) {KEYHELD.x = true;} else {KEYHELD.x = false;}
	if (keys & KEY_Y) {KEYHELD.y = true;} else {KEYHELD.y = false;}
	if (keys & KEY_L) {KEYHELD.l = true;} else {KEYHELD.l = false;}
	if (keys & KEY_R) {KEYHELD.r = true;} else {KEYHELD.r = false;}
	if (keys & KEY_START) {KEYHELD.start = true;} else {KEYHELD.start = false;}
	if (keys & KEY_SELECT) {KEYHELD.select = true;} else {KEYHELD.select = false;}

	// Teclas "PRESS"
	keys = keysDown();
	if (keys & KEY_UP) {KEYPRESS.up = true;} else {KEYPRESS.up = false;}
	if (keys & KEY_DOWN) {KEYPRESS.down = true;} else {KEYPRESS.down = false;}
	if (keys & KEY_LEFT) {KEYPRESS.left = true;} else {KEYPRESS.left = false;}
	if (keys & KEY_RIGHT) {KEYPRESS.right = true;} else {KEYPRESS.right = false;}
	if (keys & KEY_A) {KEYPRESS.a = true;} else {KEYPRESS.a = false;}
	if (keys & KEY_B) {KEYPRESS.b = true;} else {KEYPRESS.b = false;}
	if (keys & KEY_X) {KEYPRESS.x = true;} else {KEYPRESS.x = false;}
	if (keys & KEY_Y) {KEYPRESS.y = true;} else {KEYPRESS.y = false;}
	if (keys & KEY_L) {KEYPRESS.l = true;} else {KEYPRESS.l = false;}
	if (keys & KEY_R) {KEYPRESS.r = true;} else {KEYPRESS.r = false;}
	if (keys & KEY_START) {KEYPRESS.start = true;} else {KEYPRESS.start = false;}
	if (keys & KEY_SELECT) {KEYPRESS.select = true;} else {KEYPRESS.select = false;}

	// Teclas "UP"
	keys = keysUp();
	if (keys & KEY_UP) {KEYUP.up = true;} else {KEYUP.up = false;}
	if (keys & KEY_DOWN) {KEYUP.down = true;} else {KEYUP.down = false;}
	if (keys & KEY_LEFT) {KEYUP.left = true;} else {KEYUP.left = false;}
	if (keys & KEY_RIGHT) {KEYUP.right = true;} else {KEYUP.right = false;}
	if (keys & KEY_A) {KEYUP.a = true;} else {KEYUP.a = false;}
	if (keys & KEY_B) {KEYUP.b = true;} else {KEYUP.b = false;}
	if (keys & KEY_X) {KEYUP.x = true;} else {KEYUP.x = false;}
	if (keys & KEY_Y) {KEYUP.y = true;} else {KEYUP.y = false;}
	if (keys & KEY_L) {KEYUP.l = true;} else {KEYUP.l = false;}
	if (keys & KEY_R) {KEYUP.r = true;} else {KEYUP.r = false;}
	if (keys & KEY_START) {KEYUP.start = true;} else {KEYUP.start = false;}
	if (keys & KEY_SELECT) {KEYUP.select = true;} else {KEYUP.select = false;}

}



// Funcion ReadTouchpad();
void ReadTouchpad(void) {

	// Variables
	u16 keys = 0;
	touchPosition touchscreen;

	// Lee el teclado via Libnds
	scanKeys();

	// Verifica el estado del touchscreen
	keys = keysHeld();
	if (keys & KEY_TOUCH) {TOUCHPAD.held = true;} else {TOUCHPAD.held = false;}

	// Lee el TOUCHPAD via Libnds
	touchRead(&touchscreen); 

	// Guarda el estado del touchscreen
	TOUCHPAD.pos_x = touchscreen.px;
	TOUCHPAD.pos_y = touchscreen.py;
	TOUCHPAD.raw_x = touchscreen.rawx;
	TOUCHPAD.raw_y = touchscreen.rawy;
	TOUCHPAD.press_a = touchscreen.z1;
	TOUCHPAD.press_b = touchscreen.z2;

}



// Funcion FadeIn();
void FadeIn(u8 screen, u16 speed) {
	// Brillo Inicial
	s16 brillo = (16 << 8);
	// Mientras el brillo no llegue a 0
	while (brillo > 0) {
		brillo -= speed;
		// Si ya has subido el brillo
		if (brillo <= 0) {
			brillo = 0;
		}
		// Aplica el valor del brillo 
		setBrightness(screen, (0 - (brillo >> 8)));
		swiWaitForVBlank();
	}
}



// Funcion FadeOut();
void FadeOut(u8 screen, u16 speed) {
	// Brillo Inicial
	s16 brillo = 0;
	// Mientras el brillo no llegue a 0
	while (brillo < (16 << 8)) {
		brillo += speed;
		// Si ya has subido el brillo
		if (brillo >= (16 << 8)) {
			brillo = (16 << 8);
		}
		// Aplica el valor del brillo 
		setBrightness(screen, (0 - (brillo >> 8)));
		swiWaitForVBlank();
	}
}



// Funcion WaitTime();
void WaitTime(u16 wait) {
	s16 timer = 0;
	while (timer < wait) {
		timer ++;
		if (timer >= wait) {
			timer = wait;
		}
		swiWaitForVBlank();
	}
}



// Funcion ResetBuffers();
void ResetAll(void) {

	// Variables locales
	u8 screen = 0;
	u8 layer = 0;

	// La pantalla principal es la superior
	lcdMainOnTop();

	// Resetea el modo grafico a 0 en ambas pantallas
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

	// Borra los buffers de Fondos
	NF_ResetTiledBgBuffers();

	// Borra los buffers de Sprites
	NF_ResetSpriteBuffers();

	// Reinicializa los fondos
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);

	// Reinicializa los Sprites
	NF_InitSpriteSys(0, 128);
	NF_InitSpriteSys(1, 128);

	// Reinicia los buffers de texturas
	N3_ResetBuffers();

	// Reinicia los mapas de colisiones
	NF_ResetCmapBuffers();

	// Reinicializa las capas de texto
	NF_InitTextSys(0);
	NF_InitTextSys(1);

	// Elimina los posibles efectos de Alpha Blending
	REG_BLDCNT = BLEND_NONE;
	REG_BLDCNT_SUB = BLEND_NONE;
	REG_BLDALPHA = 0x0F | (0x0F << 8);
	REG_BLDALPHA_SUB = 0x0F | (0x0F << 8);

	// Restaura el color del backdrop a negro en ambas pantallas
	setBackdropColor(0);
	setBackdropColorSub(0);

	// Resetea la posicion de todas las capas de fondo
	for (screen = 0; screen < 2; screen ++) {
		for (layer = 0; layer < 4; layer ++) {
			NF_ScrollBg(screen, layer, 0, 0);
		}
	}

}



// Funcions UpdateBothScreens2D();
void UpdateBothScreens2D(void) {
	NF_SpriteOamSet(0);
	NF_SpriteOamSet(1);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
}



// Funcion Set3DinTouchScreen()
void Set3DinTouchScreen(void) {

	// La pantalla tactil tiene que estar en modo 3D
	// A partir de este momento, la pantalla 0 es la inferior y la 1 la superior
	// Pon el modo 3D
	NF_Set3D(1, 0);
	// Reinicializa los fondos tileados
	NF_InitTiledBgSys(0);
	// Inicializa los Sprites en 3D
	NF_Init3dSpriteSys();
	// Habilita el ALPHA de los Sprites 3D sobre fondos 2D
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG0 | (BLEND_DST_BG1 + BLEND_DST_BG2 + BLEND_DST_BG3);

}



// Funcion UpdateSprite();
void UpdateSprite(u8 screen, u8 sprite, u8 gfx, u8 pal) {

	// Guarda las coordenadas
	s16 x = NF_SPRITEOAM[screen][sprite].x;
	s16 y = NF_SPRITEOAM[screen][sprite].y;

	// Borra el Sprite actual
	NF_DeleteSprite(screen, sprite);

	// Crea el nuevo Sprite
	NF_CreateSprite(screen, sprite, gfx, pal, x, y);

}



// Funcion GetCurrentDate();
void GetCurrentDate(u32* day, u32* month, u32* year) {

	// Variables
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	// Opten la fecha
	*day = timeinfo->tm_mday;
	*month = (timeinfo->tm_mon + 1);
	*year = (timeinfo->tm_year + 1900); 

}



// Funcion GetAngle();
s16 GetAngle(s32 x1, s32 y1, s32 x2, s32 y2) {

	// Variables locales
	s16 angle = 0;
	s16 temp = 0;
	s32 fangle = 0;

	// Calcula el angulo
	fangle = atan2((y1 - y2), (x1 - x2));

	// Conviertelo a base 512
	temp = (int)((fangle * 512) / PI);

	if (temp >= 0) {
		angle = (temp >> 1);
	} else {
		temp = -temp;
		angle = (512 - (temp >> 1));
	}

	// Devuelve el valor entre 0 y 511
	return angle;

}



// Funcion GetDistance();
u32 GetDistance(s32 x1, s32 y1, s32 x2, s32 y2) {

	// Variables
	s32 xside = (x1 - x2);		// Distancia entre X
	s32 yside = (y1 - y2);		// Distancia entre Y
	s32 result = 0;				// Distancia absoluta entre puntos

	// Si uno de los dos vectores es 0...
	if ((xside == 0) || (yside == 0)) {
		if (xside != 0) result = xside;
		if (yside != 0) result = yside;
		if (result < 0) result = -result;
	} else {
		// Si los dos vectores son diferentes de 0...
		if (xside < 0) xside = -xside;
		if (yside < 0) yside = -yside;
		result = sqrt32((xside * xside) + (yside * yside));
	}

	// Devuelve la distancia entre puntos
	return result;

}



// Funcion LoadingIn();
void LoadingIn(void) {

	// Carga Archivos
	NF_LoadTiledBg("load/load_mindmaze", "_mindmaze", 256, 256);
	if (GAMEOPTIONS[0].language) {
		NF_LoadTiledBg("load/load_en", "_loading", 256, 256);
	} else {
		NF_LoadTiledBg("load/load_es", "_loading", 256, 256);
	}

	// Rota los fondos si es necesario
	if (GAMEOPTIONS[0].lefthanded) {
		RotateTiledBg("_mindmaze");
		RotateTiledBg("_loading");
	}

	// Crea los fondos
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTiledBg(1, 3, "_mindmaze");
		NF_CreateTiledBg(0, 3, "_loading");
	} else {
		NF_CreateTiledBg(0, 3, "_mindmaze");
		NF_CreateTiledBg(1, 3, "_loading");
	}

	// Si es para zurdos, ajusta la posicion de las capas
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(0, 3, 0, 64);
		NF_ScrollBg(1, 3, 0, 64);
	}

	// Fade in
	FadeIn(3, 128);

}



// Funcion LoadingOut();
void LoadingOut(void) {

	// Fade Out
	FadeOut(3, 128);

	// Elimina las capas de texto
	NF_DeleteTiledBg(0, 3);
	NF_DeleteTiledBg(1, 3);

	// Descarga los fondos
	NF_UnloadTiledBg("_mindmaze");
	NF_UnloadTiledBg("_loading");

}



// Funcion AbortKeys();
bool AbortKeys(void) {

	// Variables
	bool abort = false;

	// Abortar? (Salir del menu)
	if (GAMEOPTIONS[0].lefthanded) {
		// Si has pulsado la tecla de cancelar (Zurdos)
		if ((KEYPRESS.up || KEYPRESS.down || KEYPRESS.right || KEYPRESS.left)) {
			abort = true;
		}
	} else {
		// Si has pulsado la tecla de cancelar (Diestros)
		if ((KEYPRESS.a || KEYPRESS.b || KEYPRESS.x || KEYPRESS.y)) {
			abort = true;
		}
	}

	// Devuelve el resultado
	return abort;

}
