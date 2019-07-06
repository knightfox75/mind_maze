
// NightFox's Animation codec
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 01 de Agosto del 2011





// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propietarios NDS
#include <nds.h>

// Includes NightFox's Lib
#include <nf_lib.h>

//Include propio
#include "nf_video.h"





// Control de la animacion
NF_TYPE_ANIM_INFO NF_ANIMATION;





// Funcion NF_InitAnimationBuffers();
void NF_InitAnimationBuffers(void) {

	// Inicializa los buffers
	NF_ANIMATION.data_buffer = NULL;
	NF_ANIMATION.pal_buffer = NULL;

}



// Funcion NF_EnableAnimationEngine();
void NF_EnableAnimationEngine(u8 screen, u8 mode) {

	// Segun el modo seleccionado...
	if (mode == 1) {
		// Inicializa el motor 3D en modo BITMAP
		NF_Set3D(screen, 5);
	} else {
		// Inicializa el motor 2D en modo BITMAP
		NF_Set2D(screen, 5);
	}

	// Inicializa los fondos en modo "BITMAP 8 bits"
	if (mode == 1) {
		NF_InitMixedBgSys(0);
	} else {
		NF_InitMixedBgSys(screen);
	}

	// Reserva el SLOT 15 de bitmaps de 8 bits para el frame de video
	// Crea el buffer de los datos de imagen
	NF_BG8B[15].data = (u8*) calloc (49152, sizeof(u8));
	if (NF_BG8B[15].data == NULL) {
		NF_Error(102, NULL, 49152);
	}
	NF_BG8B[15].data_size = 49152;
	// Crea el buffer de la paleta
	NF_BG8B[15].pal = (u16*) calloc (256, sizeof(u16));
	if (NF_BG8B[15].pal == NULL) {
		NF_Error(102, NULL, 512);
	}
	NF_BG8B[15].pal_size  = 512;
	// Marca el slot como en uso
	NF_BG8B[15].inuse = true;

	// Reserva la memoria para los buffers de lectura
	NF_ANIMATION.data_buffer = (char*) calloc (49152, sizeof(char));
	if (NF_ANIMATION.data_buffer == NULL) {
		NF_Error(102, NULL, 49152);
	}
	NF_ANIMATION.pal_buffer = (char*) calloc (1024, sizeof(char));
	if (NF_ANIMATION.pal_buffer == NULL) {
		NF_Error(102, NULL, 1024);
	}

	// Guarda los parametros de inicializacion
	NF_ANIMATION.screen = screen;
	NF_ANIMATION.mode = mode;

}



// Funcion NF_OpenAnimationStream();
void NF_OpenAnimationStream(const char* file, bool loop) {

	// Variables
	char filename[256];

	// Nombre de archivo
	sprintf(filename, "%s/%s.ani", NF_ROOTFOLDER, file);

	// Abre el archivo .ANI
	NF_ANIMATION.file_id = fopen(filename, "rb");
	if (NF_ANIMATION.file_id) {	// Si el archivo existe...
		// Obten el tamaño del archivo
		fseek(NF_ANIMATION.file_id, 0, SEEK_END);
		NF_ANIMATION.stream_size = ftell(NF_ANIMATION.file_id);
		rewind(NF_ANIMATION.file_id);
	} else {	// Si el archivo no existe...
		fclose(NF_ANIMATION.file_id);		// Cierra el archivo
		NF_Error(101, "error", 0);
	}

	// Resetea el contador de posicion
	NF_ANIMATION.stream_pos = 0;

	// el thread de ejecucion
	NF_ANIMATION.thread = 0;

	// Flag de playback arriba
	NF_ANIMATION.play = true;

	// Y guarda el valor del loop
	NF_ANIMATION.loop = loop;

}



// Funcion NF_UpdateAnimationStream();
bool NF_UpdateAnimacionStream(void) {

	// Variables locales
	bool result = true;

	// Variables locales
	typedef struct {
		int data_size;
		int pal_size;
	} stream_info;
	stream_info stream;

	// Segun el thread de ejecucion
	switch (NF_ANIMATION.thread) {

		case 0: // Lectura de cabecera y creacion de los buffers temporales
			// Lee los metadatos
			memset((void*)&stream, 0, sizeof(stream));
			fread((void*)&stream, sizeof(stream), 1, NF_ANIMATION.file_id);
			// Y almacenalos
			NF_ANIMATION.data_size = stream.data_size;
			NF_ANIMATION.pal_size = stream.pal_size;
			break;

		case 1:	// Carga los archivos a RAM
			// Lee el frame actual
			fread(NF_ANIMATION.data_buffer, 1, NF_ANIMATION.data_size, NF_ANIMATION.file_id);
			fread(NF_ANIMATION.pal_buffer, 1, NF_ANIMATION.pal_size, NF_ANIMATION.file_id);
			break;

		case 2: // Descomprime los datos
			// Descomprime los datos usando la BIOS
			decompress(NF_ANIMATION.data_buffer, NF_BG8B[15].data, LZ77);
			decompress(NF_ANIMATION.pal_buffer, NF_BG8B[15].pal, LZ77);
			break;

		case 3:	// Actualiza el contador de posicion y manda la imagen descomprimida a la VRAM
			// Calcula el puntero al siguiente frame
			NF_ANIMATION.stream_pos += (8 + NF_ANIMATION.data_size + NF_ANIMATION.pal_size);
			// Si es el fin de archivo
			if (NF_ANIMATION.stream_pos >= (NF_ANIMATION.stream_size - (8 + NF_ANIMATION.data_size + NF_ANIMATION.pal_size))) {
				// Si el loop esta activado
				if (NF_ANIMATION.loop) {
					rewind(NF_ANIMATION.file_id);
					NF_ANIMATION.stream_pos = 0;
				} else {
					NF_ANIMATION.play = false;
					NF_ANIMATION.thread = 255;
					result = false;
				}
			}
			// Manda el frame a la VRAM
			if (NF_ANIMATION.mode == 1) {
				NF_Copy8bitsBuffer(0, 1, 15);
			} else {
				NF_Copy8bitsBuffer(NF_ANIMATION.screen, 1, 15);
			}
			break;

		default:	// Por defecto, nada
			break;

	}

	// Actualiza el thread de ejecucion
	if (NF_ANIMATION.play) {
		NF_ANIMATION.thread ++;
		if (NF_ANIMATION.thread > 3) NF_ANIMATION.thread = 0;
	}

	// Devuelve el resultado de ejecucion
	return result;

}



// Funcion NF_CloseAnimationStream();
void NF_CloseAnimationStream(void) {

	// Cierra el archivo de animacion
	fclose(NF_ANIMATION.file_id);

}



// Funcion NF_DisableAnimationEngine();
void NF_DisableAnimationEngine(void) {

	// Resetea todos los buffers usados por el engine
	free(NF_BG8B[15].data);
	NF_BG8B[15].data = NULL;
	NF_BG8B[15].data_size = 0;
	//
	free(NF_BG8B[15].pal);
	NF_BG8B[15].pal = NULL;
	NF_BG8B[15].pal_size  = 0;
	//
	free(NF_ANIMATION.data_buffer);
	NF_ANIMATION.data_buffer = NULL;
	//
	free(NF_ANIMATION.pal_buffer);
	NF_ANIMATION.pal_buffer = NULL;

	// Marca el slot como libre
	NF_BG8B[15].inuse = false;

	// Restaura a negro el color del backdrop
	if (NF_ANIMATION.mode == 1) {
		setBackdropColor(0);
	} else {
		if (NF_ANIMATION.screen == 0) {
			setBackdropColor(0);
		} else {
			setBackdropColorSub(0);
		}
	}

}
