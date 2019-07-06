#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_DEFINES_H__
#define __N3_DEFINES_H__





// NightFox 3D LIB - Definiciones y variables globales
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>



// Conversiones de OpenGL, convierte coordenadas reales a v16 (1 << 12)
#define N3_RealToV16(n) ((v16)(n * (1 << 4)))
// Conversiones de OpenGL, convierte coordenadas reales a f32 (1 << 12)
#define N3_RealToF32(n) ((int)(n * (1 << 4)))


// Define el numero de slots de texturas y paletas
#define N3_TEX_GFX_SLOTS 256	// Slots en RAM para cargar texturas
#define N3_TEX_PAL_SLOTS 64		// Slots en RAM para cargar paletas
#define N3_VRAM_TEX_SLOTS 256	// Slots en VRAM para almacenar texturas
#define N3_VRAM_PAL_SLOTS 32	// Slots en VRAM para almacenar paletas

// Path del filesystem (Maximo 32 caracteres)
extern char N3_ROOTFOLDER[32];

// Define la estructura de datos de los buffers de texturas en RAM
typedef struct {
	u32 size;			// Tamaño (en bytes) de la Textura
	u16 width;			// Ancho de la Textura
	u16 height;			// Altura de la Textura
	bool available;		// Disponibilidat del Slot
	char* buffer;
} N3_TYPE_TEX_GFX_INFO;
extern N3_TYPE_TEX_GFX_INFO N3_TEX_GFX[N3_TEX_GFX_SLOTS];

// Define la estructura de datos de los buffers de paletas en RAM
typedef struct {
	u32 size;			// Tamaño (en bytes) de la paleta
	bool available;		// Disponibilidat del Slot
	char* buffer;		// Buffer donde almacenar la paleta
} N3_TYPE_TEX_PAL_INFO;
extern N3_TYPE_TEX_PAL_INFO N3_TEX_PAL[N3_TEX_PAL_SLOTS];

// Define la estructura de gestion de la VRAM para texturas	(Gestion dinamica de la VRAM
typedef struct {
	s32 free;						// Memoria VRAM libre
	u32 next;						// Siguiente posicion libre
	u32 last;						// Ultima posicion usada
	u32 pos[N3_VRAM_TEX_SLOTS];		// Posicion en VRAM para reusar despues de un borrado
	u32 size[N3_VRAM_TEX_SLOTS];	// Tamaño del bloque libre para reusar
	u16 deleted;					// Numero de bloques borrados
	s32 fragmented;					// Memoria VRAM fragmentada
	s32 inarow;						// Memoria VRAM contigua
	bool texupdate;					// Alguna textura debe ser actualizada despues del VBLANK (RAM -> VRAM, keepframes == TRUE)
} N3_TYPE_TEXVRAM_INFO;
extern N3_TYPE_TEXVRAM_INFO N3_TEXVRAM;

// Define la estructura de control de las Texturas cargadas en VRAM
typedef struct {
	u32 size;			// Tamaño (en bytes) de la Textura
	u16 width;			// Ancho de la textura
	u16 height;			// Altura de la textura
	u32 address;		// Posicion en la VRAM
	u16 ramid;			// Numero de Slot en RAM del que provienes
	u16 framesize;		// Tamaño del frame (en bytes)
	u16 lastframe;		// Ultimo frame
	u32 format;			// Formato de textura
	u8 frame;			// Frame actual de la textura
	u8 newframe;		// Frame al que se actualizara la textura
	bool keepframes;	// Si es una textura animada, debes de mantener los frames en RAM ?
	bool inuse;			// Disponibilidat del Slot
} N3_TYPE_TEXTURE_INFO;
extern N3_TYPE_TEXTURE_INFO N3_TEXTURE[N3_VRAM_TEX_SLOTS];

// Define la estructura de control de las Paletas cargadas en VRAM
typedef struct {
	bool inuse;		// Slot en uso
	u8 ramslot;		// Paleta original en RAM
	u32 format;		// Formato de la paleta
} N3_TYPE_PALVRAM_INFO;
extern N3_TYPE_PALVRAM_INFO N3_PALETTE[N3_VRAM_PAL_SLOTS];





#endif

#ifdef __cplusplus
}
#endif
