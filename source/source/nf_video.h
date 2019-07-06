#ifndef __NF_VIDEO_H__
#define __NF_VIDEO_H__





// NightFox's Animation codec
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 01 de Agosto del 2011





// Includes propietarios NDS
#include <nds.h>





// Estructuras de control de la animacion
typedef struct {
	char* data_buffer;		// Buffer para almacenar la imagen
	u32 data_size;			// Tamaño de la imagen (comprimida)
	char* pal_buffer;		// Buffer para alamacenar la paleta
	u32 pal_size;			// Tamaño de la paleta (comprimida)
	u32 stream_size;		// Tamaño total del archivo
	u32 stream_pos;			// Posicion en el archivo
	FILE* file_id;			// Id del archivo abierto
	bool loop;				// Loop del video
	u8 thread;				// Thread de ejecucion del Stream update
	bool play;				// Flag de playback
	u8 screen;				// Pantalla
	u8 mode;				// Modo (2D/3D)
} NF_TYPE_ANIM_INFO;
extern NF_TYPE_ANIM_INFO NF_ANIMATION;





// Funcion NF_InitAnimationBuffers();
void NF_InitAnimationBuffers(void);
// Inicializa los buffers de lectura de datos del stream.
// Usarlo solo una vez antes de usar los buffers.



// Funcion NF_EnableAnimationEngine();
void NF_EnableAnimationEngine(u8 screen, u8 mode);
// Inicializa el engine de animacion en la pantalla indicada.
// Inicializa el modo grafico, buffers etc.



// Funcion NF_OpenAnimationStream();
void NF_OpenAnimationStream(const char* file, bool loop);
// Abre un archivo de animacion para su streaming,
// puedes indicar tambien si debe realizar bucle.



// Funcion NF_UpdateAnimationStream();
extern bool NF_UpdateAnimacionStream(void);
// Actualiza el stream de la animacion.
// Usar 1 vez por frame.
// Devuelve TRUE si aun quedan datos por reproducir.



// Funcion NF_CloseAnimationStream();
void NF_CloseAnimationStream(void);
// Cierra el archivo de animacion.



// Funcion NF_DisableAnimationEngine();
void NF_DisableAnimationEngine(void);
// Vacia todos los buffers usados por el engine.





#endif
