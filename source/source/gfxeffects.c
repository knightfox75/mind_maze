
// Mind Maze - Funciones de effectos graficos
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
#include <n3_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion CrossDisolve();
bool CrossDisolve(u8 screen, u8 origin, u8 destination) {

	// Variables para obtener el RGB
	u16 rgb = 0;
	u8 r[3];
	u8 g[3];
	u8 b[3];

	// Variables adicionales
	u16 x = 0;
	u16 y = 0;
	u16 effect = 0;
	u16 size = 32;
	u16 speed = 8;
	u16 start = 0;
	u16 end = 0;
	bool abort = false;

	for (effect = 0; effect < (192 + size + speed); effect += speed) {
	
		// Calcula el rango de la imagen destino
		if (effect >= size) {
			memcpy(NF_16BITS_BACKBUFFER[screen], NF_BG16B[destination].buffer, ((effect << 8) << 1));
		}

		// Calcula el rango del efecto
		if (effect < size) {
			start = 0;
			end = effect;
		} else {
			start = (effect - size);
			end = effect;
		}
		// Aplica el efecto
		for (y = start; y < end; y ++) {
			for (x = 0; x < 256; x ++) {
				// Si la linea del efecto esta dentro de la pantalla
				if (y < 192) {
					// Obten los valores RGB del destino
					rgb = NF_BG16B[destination].buffer[((y << 8) + x)];
					r[0] = (((rgb & 0x1F) * (end - y)) / size);
					g[0] = ((((rgb >> 5) & 0x1F) * (end - y)) / size);
					b[0] = ((((rgb >> 10) & 0x1F) * (end - y)) / size);
					// Obten los valores RGB del origen
					rgb = NF_BG16B[origin].buffer[((y << 8) + x)];
					r[1] = (((rgb & 0x1F) * (size - (end - y))) / size);
					g[1] = ((((rgb >> 5) & 0x1F) * (size - (end - y))) / size);
					b[1] = ((((rgb >> 10) & 0x1F) * (size - (end - y))) / size);
					// Calcula el valor del fade
					r[2] = (r[0] + r[1]);
					g[2] = (g[0] + g[1]);
					b[2] = (b[0] + b[1]);
					NF_16BITS_BACKBUFFER[screen][((y << 8) + x)] = ((r[2])|((g[2]) << 5)|((b[2]) << 10)|(BIT(15)));
				}
			}
		}

		// Lee el teclado
		ReadKeypad();

		// Se ha abortado
		if (AbortKeys()) {
			abort = true;
			effect = (192 + size + speed);
		}

		// Espera al sincronismo vertical
		swiWaitForVBlank();

		// Vuelca la iamgen resultante
		NF_Flip16bitsBackBuffer(screen);

	}

	// Revuelve si se ha abortado
	return abort;

}



// Funcion Rotate16bitsImage();
void Rotate16bitsImage(u8 id) {

	// Variables
	s16 x = 0;
	s16 y = 0;

	// Crea un buffer temporal de 16 bits
	u16* temp;
	temp = NULL;
	temp = (u16*) calloc ((NF_BG16B[id].size >> 1), sizeof(u16));
	if (temp == NULL) NF_Error(102, NULL, NF_BG16B[id].size);

	// Copia los bytes del buffer original al temporal, invirtiendo el orden
	for (y = 0; y < 192; y ++) {
		for (x = 0; x < 256; x ++) {
			temp[(((191 - y) << 8) + (255 - x))] = NF_BG16B[id].buffer[((y << 8) + x)];
		}
	}

	// Copia el buffer temporal ya invertido al original
	memcpy(NF_BG16B[id].buffer, temp, NF_BG16B[id].size);

	// Libera el buffer
	free(temp);
	temp = NULL;

}



// Funcion RotateTiledBg();
void RotateTiledBg(const char* name) {

	// Variables
	s32 n = 0;			// Bucle
	s32 z = 0;
	s32 address = 0;	// Direccion
	u8 slot = 255;		// Slot seleccionado
	char bg[32];		// Nombre

	// Busca el fondo solicitado
	sprintf(bg, "%s", name);							// Obten el nombre del fondo a buscar
	for (n = 0; n < NF_SLOTS_TBG; n ++) {				// Busca en todos los slots
		if (strcmp(bg, NF_TILEDBG[n].name) == 0) {		// Si lo encuentras
			slot = n;									// Guarda el slot a usar
			n = NF_SLOTS_TBG;							// Deja de buscar
		}
	}
	// Si no se encuentra, error
	if (slot == 255) {
		NF_Error(104, name, 0);
	}

	//////////////////////
	// Invierte el mapa //
	//////////////////////

	// Crea el buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (NF_TILEDBG[slot].mapsize, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_TILEDBG[slot].mapsize);

	// Copia el buffer del mapa al temporal, invirtiendo el orden
	for (n = 0; n < NF_TILEDBG[slot].mapsize; n += 2) {
		address = ((NF_TILEDBG[slot].mapsize - 2) - n);
		temp[address] = *(NF_BUFFER_BGMAP[slot] + n);
		temp[(address + 1)] = *(NF_BUFFER_BGMAP[slot] + (n + 1));
	}

	// Copia el buffer temporal, ya invertido, al original
	memcpy(NF_BUFFER_BGMAP[slot], temp, NF_TILEDBG[slot].mapsize);

	// Borra el buffer temporal
	free(temp);
	temp = NULL;

	/////////////////////////
	// Invierte el tileset //
	/////////////////////////

	// Crea el buffer temporal
	temp = (char*) calloc (NF_TILEDBG[slot].tilesize, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_TILEDBG[slot].tilesize);

	// Copia el buffer del tileset al temporal, invirtiendo el orden
	for (n = 0; n < NF_TILEDBG[slot].tilesize; n += 64) {
		for (z = 0; z < 64; z ++) {
			temp[(n + (63 - z))] = *(NF_BUFFER_BGTILES[slot] + (n + z));
		}
	}

	// Copia el buffer temporal, ya invertido, al original
	memcpy(NF_BUFFER_BGTILES[slot], temp, NF_TILEDBG[slot].tilesize);

	// Borra el buffer temporal
	free(temp);
	temp = NULL;

}



// Funcion RotateSprite();
void RotateSprite(u16 id) {

	// Variables
	s32 n = 0;			// Bucle
	s32 address = 0;

	// Crea el buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (NF_SPR256GFX[id].size, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_SPR256GFX[id].size);

	// Copia el buffer del sprite al temporal, invirtiendo el orden
	for (n = 0; n < NF_SPR256GFX[id].size; n += 2) {
		address = ((NF_SPR256GFX[id].size - 2) - n);
		temp[address] = *(NF_BUFFER_SPR256GFX[id] + (n + 1));
		temp[(address + 1)] = *(NF_BUFFER_SPR256GFX[id] + n);
	}

	// Copia el buffer temporal, ya invertido, al original
	memcpy(NF_BUFFER_SPR256GFX[id], temp, NF_SPR256GFX[id].size);

	// Borra el buffer temporal
	free(temp);
	temp = NULL;

}



// Funcion Rotate3dSprite();
void Rotate3dSprite(u16 id) {

	// Variables
	s32 n = 0;			// Bucle

	// Crea el buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (NF_SPR256GFX[id].size, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_SPR256GFX[id].size);

	// Copia el buffer del sprite al temporal, invirtiendo el orden
	for (n = 0; n < NF_SPR256GFX[id].size; n ++) {
		temp[((NF_SPR256GFX[id].size - 1) - n)] = *(NF_BUFFER_SPR256GFX[id] + n);
	}

	// Copia el buffer temporal, ya invertido, al original
	memcpy(NF_BUFFER_SPR256GFX[id], temp, NF_SPR256GFX[id].size);

	// Borra el buffer temporal
	free(temp);
	temp = NULL;

}



// Funcion DesaturateSprite();
void DesaturateSprite(u8 screen, u8 pal, u8 brightness) {

	// Variables
	u8 slot = 0;
	u16 n = 0;
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 m = 0;

	// Obten el slot donde esta la paleta en RAM
	slot = NF_SPRPALSLOT[screen][pal].ramslot;

	// Crea un buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (NF_SPR256PAL[slot].size, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_SPR256PAL[slot].size);

	// Copia la paleta original al buffer temporal
	memcpy(temp, NF_BUFFER_SPR256PAL[slot], NF_SPR256PAL[slot].size);

	// Desatura todos los colores de la paleta
	for (n = 0; n < 256; n ++) {
		// Obten el color actual
		NF_SpriteGetPalColor(screen, pal, n, &r, &g, &b);
		// Recombina el color para blanco y negro
		m = ((r + g + b + brightness) >> 2);
		// Asigna el nuevo color
		NF_SpriteEditPalColor(screen, pal, n, m, m, m);
	}

	// Actualiza la paleta en VRAM
	NF_SpriteUpdatePalette(screen, pal);

	// Restaura la paleta a su estado original
	memcpy(NF_BUFFER_SPR256PAL[slot], temp, NF_SPR256PAL[slot].size);

}



// Funcion Desaturate3dSprite();
void Desaturate3dSprite(u8 pal, u8 brightness) {

	// Variables
	u8 slot = 0;
	u16 n = 0;
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 m = 0;

	// Obten el slot donde esta la paleta en RAM
	slot = NF_TEXPALSLOT[pal].ramslot;

	// Crea un buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (NF_SPR256PAL[slot].size, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, NF_SPR256PAL[slot].size);

	// Copia la paleta original al buffer temporal
	memcpy(temp, NF_BUFFER_SPR256PAL[slot], NF_SPR256PAL[slot].size);

	// Desatura todos los colores de la paleta
	for (n = 0; n < 256; n ++) {
		// Obten el color actual
		NF_3dSpriteGetPalColor(pal, n, &r, &g, &b);
		// Recombina el color para blanco y negro
		m = ((r + g + b + brightness) >> 2);
		// Asigna el nuevo color
		NF_3dSpriteEditPalColor(pal, n, m, m, m);
	}

	// Actualiza la paleta en VRAM
	NF_3dSpriteUpdatePalette(pal);

	// Restaura la paleta a su estado original
	memcpy(NF_BUFFER_SPR256PAL[slot], temp, NF_SPR256PAL[slot].size);

}



// Funcion void RotateMap();
void RotateMap(u8 slot) {

	// Variables
	s32 x1 = 0;		// Coordenadas en el mapa
	s32 y1 = 0;
	s32 x2 = 0;
	s32 y2 = 0;
	s32 adr1 = 0;	// Direcciones relativas
	s32 adr2 = 0;
	s32 width = (NF_CMAP[slot].width >> 3);		// Medidas
	s32 height = (NF_CMAP[slot].height >> 3);
	s32 data1 = 0;	// Datos
	s32 data2 = 0;

	// Bucle de intercambio de datos (Mitad superior del mapa)
	for (y1 = 0; y1 < (height >> 1); y1 ++) {
		for (x1 = 0; x1 < width; x1 ++) {
			// Calcula el extremo opuesto
			y2 = ((height - y1) - 1);
			x2 = ((width - x1 ) - 1);
			// Calcula las direcciones relativas del LO-Byte
			adr1 = ((((y1 + 1) * width) + x1) << 1);
			adr2 = ((((y2 + 1) * width) + x2) << 1);
			// Intercambia los datos
			data1 = *(NF_CMAP[slot].map + adr1);
			data2 = *(NF_CMAP[slot].map + adr2);
			*(NF_CMAP[slot].map + adr1) = data2;
			*(NF_CMAP[slot].map + adr2) = data1;
			// Calcula las posiciones relativas del HI-Byte
			adr1 ++;
			adr2 ++;
			// Intercambia los datos
			data1 = *(NF_CMAP[slot].map + adr1);
			data2 = *(NF_CMAP[slot].map + adr2);
			*(NF_CMAP[slot].map + adr1) = data2;
			*(NF_CMAP[slot].map + adr2) = data1;
		}
	}

}



// Funcion RotateTexture();
void RotateTexture(u16 id) {

	// Variables
	s32 n = 0;			// Bucle

	// Crea el buffer temporal
	char* temp;
	temp = NULL;
	temp = (char*) calloc (N3_TEX_GFX[id].size, sizeof(char));
	if (temp == NULL) NF_Error(102, NULL, N3_TEX_GFX[id].size);

	// Copia el buffer del sprite al temporal, invirtiendo el orden
	for (n = 0; n < N3_TEX_GFX[id].size; n ++) {
		temp[((N3_TEX_GFX[id].size - 1) - n)] = *(N3_TEX_GFX[id].buffer + n);
	}

	// Copia el buffer temporal, ya invertido, al original
	memcpy(N3_TEX_GFX[id].buffer, temp, N3_TEX_GFX[id].size);

	// Borra el buffer temporal
	free(temp);
	temp = NULL;

}
