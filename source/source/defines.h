#ifndef __DEFINES_H__
#define __DEFINES_H__





// Mind Maze - Definiciones y variables globales
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>





////////////////////////
// Defines de sistema //
////////////////////////

// Numero PI
#define PI 3.141593

// Obtiene via registro la linea actual del dibujado
#define VLINE *(vu16*)0x04000006



////////////////////////////////////
// Defines del control de entrada //
////////////////////////////////////

// Define la estructura de lectura del KeyPad
typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
	bool a;
	bool b;
	bool x;
	bool y;
	bool l;
	bool r;
	bool start;
	bool select;
} keypad_info;

// Array para almacenar las teclas "HELD"
extern keypad_info KEYHELD;

// Array para almacenar las teclas "DOWN"
extern keypad_info KEYPRESS;

// Array para almacenar las teclas "UP"
extern keypad_info KEYUP;

// Define la estructura de lectura del Touchscreen
typedef struct {
	u16 pos_x;
	u16 pos_y;
	u16 raw_x;
	u16 raw_y;
	u16 press_a;
	u16 press_b;
	bool held;
} touchscreen_info;

// Array para almacenar la info de la pantalla tactil
extern touchscreen_info TOUCHPAD;



/////////////////
// Uso general //
/////////////////

#define CLICK_MINTIME 2		// Tiempo minimo para el click valido

#define MENU_DEADZONE 4		// Zona muerta al levantar el stylus

#define MENU_MINMOVE 2		// Zona muerta del movimiento
#define MENU_MAXMOVE 8		

#define START_CREDITS 100	// Numero de creditos al empezar (100) *** Cheat ***

// Uso comun en todos los menus
typedef struct {
	s32 pos;		// Posicion del stylus
	s32 last_pos;	// Posicion anterior del stylus
	s32 first_pos;	// Primera posicion del stylus al tocar la pantalla
	s32 move;		// Factor de movimiento
	s32 height;		// Ultima posicion sobre la vertical, tocando la pantalla
	bool touch;		// Flag de si se ha tocado la pantalla con el stylus
	s16 id;			// Id del icono tocado
	bool id_enable;	// Ultima Id tocada
	s16 timer;		// Temporizador de stylus sobre la pantalla
	bool inertia;	// Flag de inercia activa
} menu_info;
extern menu_info MENU;



////////////////////
// Menu Principal //
////////////////////

#define MMITEMS 6	// Numero total de items en el menu

// Controla el ROLL del menu principal
typedef struct {
	s16 y;			// Posicion Y del sprite
	s32 fy;			// Posicion Y del sprite (<< 8)
	s8 gfx;			// Gfx del sprite
} mmroller_info;
extern mmroller_info MMROLLER[5];



//////////////////////
// Menu de opciones //
//////////////////////

#define OPTMITEMS 10		// Numero total de items en el menu

// Controla los iconos del menu de opciones
typedef struct {
	s16 x;			// Posicion X del sprite
	s16 y;			// Posicion Y del sprite
	s32 fx;			// Posicion X del sprite (<< 8)
	s32 fy;			// Posicion Y del sprite (<< 8)
	s8 gfx;			// Gfx/Pal del sprite
} optm_info;
extern optm_info OPTM[OPTMITEMS];

// Controla el menu de opciones
typedef struct {
	bool touch_down;
	u8 thread;
	s32 counter;
	bool lefthanded;
} optmcontrol_info;
extern optmcontrol_info OPTMCONTROL;



//////////////////////
// Creditos			//
//////////////////////

#define CREDITS_FLOOR_TEX 10			// Id textura suelo tunel
#define CREDITS_FLOOR_PAL 13			// Id paleta suelo tunel
#define CREDITS_FLOOR_TEX_LASTNUM 16	// Ultima textura del suelo

#define CREDITS_CEIL_TEX (CREDITS_FLOOR_TEX + CREDITS_FLOOR_TEX_LASTNUM + 1)	// Id textura techo tunel
#define CREDITS_CEIL_PAL 12				// Id paleta techo tunel
#define CREDITS_CEIL_TEX_LASTNUM 12		// Ultima textura del techo

#define CREDITS_WALL_TEX (CREDITS_CEIL_TEX + CREDITS_CEIL_TEX_LASTNUM + 1)		// Id textura pared tunel
#define CREDITS_WALL_PAL 11				// Id paleta pared tunel
#define CREDITS_WALL_TEX_LASTNUM 14		// Ultima textura de la pared

#define CREDITS_MSG_TEX 220				// Id de la primera textura del panel de creditos
#define CREDITS_MSG_TOTAL_TEX 35		// Total de objetos
#define CREDITS_MSG_PAL 10				// Id paleta panel creditos
#define CREDITS_MSG_TEX_LASTNUM (CREDITS_MSG_TEX + CREDITS_MSG_TOTAL_TEX)	// Ultima textura del panel

#define CREDITS_BLOCK_SIZE (64 << 4)		// Tamaño de cada bloque del tunel

#define CREDITS_CAM_SPEED 8					// Velocidad del tunel

#define CREDITS_WALL_HEIGHT 4				// Medidas de la pared del tunel (en bloques)
#define CREDITS_WALL_DEEP 5

#define CREDITS_CEIL_WIDTH 3				// Medidas del techo del tunel (en bloques)
#define CREDITS_CEIL_DEEP 5

#define CREDITS_FLOOR_WIDTH 6				// Medidas del suelo del tunel (en bloques)
#define CREDITS_FLOOR_DEEP 10

#define CREDITS_WALL_MIN (4 << 4)			// Limites generacion bloques pared
#define CREDITS_WALL_MAX (12 << 4)

#define CREDITS_CEIL_MIN (4 << 4)			// Limites generacion bloques techo
#define CREDITS_CEIL_MAX (8 << 4)

#define CREDITS_FLOOR_MIN (1 << 4)			// Limites generacion bloques suelo
#define CREDITS_FLOOR_MAX (4 << 4)

#define CREDITS_PANEL_MIN_SPEED 4			// Velocidad de desplazamiento del panel
#define CREDITS_PANEL_WAVE_SPEED 4			// Velocidad de la ondulacion del panel
#define CREDITS_PANEL_SIZE (32 << 4)		// Tamaño de cada bloque del panel
#define CREDITS_PANEL_DELAY 120				// Tics de retardo para cambiar de panel

#define CREDITS_CUBE_X (96 << 4)			// Coordenadas del cubo (Arista superior izquierda)
#define CREDITS_CUBE_Y (128 << 4)
#define CREDITS_CUBE_Z -(32 << 4)
#define CREDITS_CUBE_SIZE (64 << 4)			// Tamaño del cubo (lado)


// Datos del tunel
typedef struct {
	s32 height[4][10][10];	// Mapa de alturas
	u8 texture[4][10][10];	// Textura a aplicar
	s32 cam_z;				// Posicion de la camara
	u8 panel_thread;		// Thread actual del panel
	s16 panel_cnt;			// Contador
	s16 panel_angle[8][6];	// Animacion del panel de creditos (Angulo)
	s16 panel_z[8][6];		// Valor de desplazamiento de Z de cada vertize
	s32 panel_pos;			// Posicion (Z) del panel en el tunel
	bool panel_update;		// Las texturas del panel deben ser actualizadas
	u8 panel_texdone;		// Numero de texturas actualizadas en VRAM
	bool done;				// Panel completado (ultima pagina del texto dibujada)
	bool screen;			// Pantalla de dibujado (0 main, 1 sub)
	s16 cube_angle;			// Angulo de giro del cubo
} creditsworld_info;
extern creditsworld_info CREDITSWORLD;

// Datos del panel de creditos
typedef struct {
	char* image_buffer;		// Buffer para almacenar la imagen de fondo
	u32 image_size;			// Tamaño de la imagen de fondo
	char* font_buffer;		// Buffer para almacenar la fuente en bitmap
	u32 font_size;			// Tamaño del archivo de fuentes
	char* text_buffer;		// Buffer para almacenar el archivo con los textos de los creditos
	u32 text_size;			// Tamaño del archivo de creditos
	u32 text_pos;			// Posicion del puntero de lectura
} credits_info;
extern credits_info CREDITS;




///////////////////////////////////////////
// Puzzle 01 - Puzzle deslizante (Juego) //
///////////////////////////////////////////

#define P01_ITEMS 11			// Numero total de puzzles en el juego

#define PUZZLE01_SPRITE 10		// Primer Sprite de las fichas
#define PUZZLE01_SPEED 8		// Velocidad de movimiento de las fichas
#define PUZZLE01_MIXMOVES 50	// Numero de movimientos al mezclar el puzzle (50) *** CHEAT ***

// Gestion de las fichas del puzzle
typedef struct {
	u8 id;		// ID de la pieza
	u8 gfx;		// Grafico que usa
	u8 pal;		// Paleta que usa
	bool bn;	// Esta en blanco y negro?
	s16 x;		// Coordenada X
	s16 y;		// Coordenada Y
	u8 gx;		// Posicion X en la parrilla
	u8 gy;		// Posicion Y en la parrilla
} puzzle01spr_info;
extern puzzle01spr_info PUZZLE01SPR[12];

// Gestion del movimiento de las fichas
typedef struct {
	u8 id [3][4];	// Matriz de control del puzzle
	u8 target;		// Pieza objetivo
	u8 move;		// Direccion del movimiento
	s16 count;		// Contador del movimiento
	s8 add_x;		// Factor de desplazamiento X de la ficha
	s8 add_y;		// Factor de desplazamiento y de la ficha
} puzzle01mov_info;
extern puzzle01mov_info PUZZLE01MOV;



///////////////////////////////////////////
// Puzzle 01 - Puzzle deslizante (Menu)  //
///////////////////////////////////////////

// Controla el ROLL del menu del puzzle
typedef struct {
	s16 y;			// Posicion Y del sprite
	s32 fy;			// Posicion Y del sprite (<< 8)
	s8 gfx;			// Gfx del sprite
	bool bn;		// Control del blanco y negro
} p01mroller_info;
extern p01mroller_info P01MROLLER[5];

// Textos en el menu
typedef struct {
	char name[16];		// Nombre del puzzle
	char author[16];	// Autor
} p01text_info;
extern p01text_info P01TEXT[P01_ITEMS];



/////////////////////////////////
// Puzzle 02 - Parejas (Juego) //
/////////////////////////////////

#define P02_ITEMS 6				// Numero de mazos en el juego

#define PUZZLE02_SPRITE 10		// Primer Sprite de las cartas
#define PUZZLE02_MARKSPRITE 3	// Primer Sprite de los marcadores
#define PUZZLE02_FLIP_SPEED 8	// Velocidad del efecto FLIP
#define PUZZLE02_STARTROUND 0	// Ronda en la que empiezas (0) *** CHEAT ***

// Informacion de cada mazo
typedef struct {
	char name[16];		// Nombre del mazo
	char author[16];	// Autor
	u8 cards;			// Numero de cartas del mazo, incluida el jocker y el reverso
	s16 start_x[2];		// Posicion X inicial en el tapete (0 - diestros / 1 - Zurdos)
	s16 start_y[2];		// Posicion Y inicial en el tapete (0 - diestros / 1 - Zurdos)
	s16 mark_x[4][2];	// Posicion X del marcador
	s16 mark_y[4][2];	// Posicion Y del marcador
	s16 timer;			// Tiempo de animacion
	s16 delay;			// Tiempo entre frames
	s16 frames;			// Frames de la animacion
} puzzle02info_info;
extern puzzle02info_info PUZZLE02INFO[P02_ITEMS];

// Informacion de las cartas en juego
typedef struct {
	u8 card;	// ID de la carta (dibujo oculto que contiene)
	bool tap;	// Esta la carta cubierta?
	s16 x;		// Coordenada X
	s16 y;		// Coordenada Y
} puzzle02spr_info;
extern puzzle02spr_info PUZZLE02SPR[9];

// Informacion de las cartas del mazo actual
typedef struct {
	u8 cards;			// Numero de cartas del mazo actual
	s16 timer[4];		// Temporizador animacion
	s16 delay[4];		// Retardo entre frames
	s16 frame[4];		// nº de frame actual
	s16 timer_end;		// Limite del temporizador
	s16 delay_end;		// Limite del retardo de frames
	s16 last_frame;		// Ultimo frame de la animacion
	u8 rounds;			// Rondas jugadas
} puzzle02deck_info;
extern puzzle02deck_info PUZZLE02DECK;



//////////////////////////////////
// Puzzle 02 - Parejas (Menu)  //
//////////////////////////////////

// Controla el ROLL del menu del puzzle
typedef struct {
	s16 y;			// Posicion Y del sprite
	s32 fy;			// Posicion Y del sprite (<< 8)
	s8 gfx;			// Gfx del sprite
	bool bn;		// Control del blanco y negro
} p02mroller_info;
extern p02mroller_info P02MROLLER[5];



///////////////////////////////////
// Puzzle 03 - Laberinto (Juego) //
///////////////////////////////////

#define P03_ITEMS 9				// Numero de laberintos en el juego

#define P03_TILE_FLOOR 0		// Define los tiles del mapa de colisiones
#define P03_TILE_WALL 1
#define P03_TILE_GOAL 2
#define P03_TILE_VOID 3

#define P03_AURA_ALPHA 24		// Alpha de los efectos
#define P03_PATH_ALPHA 16

// Informacion de cada laberinto
typedef struct {
	char name[16];		// Nombre del laberinto
	char author[16];	// Autor
	s32 start_x[2];		// Posicion X inicial en el tapete (0 - diestros / 1 - Zurdos)
	s32 start_y[2];		// Posicion Y inicial en el tapete (0 - diestros / 1 - Zurdos)
	s32 width;			// Ancho del laberinto
	s32 height;			// Altura del laberinto
} puzzle03info_info;
extern puzzle03info_info PUZZLE03INFO[P03_ITEMS];

// Controla el mundo 3D
typedef struct {
	s32 ball_x;			// Posicion de la bola
	s32 ball_y;
	s32 ball_z;
	s32 ball_fx;		// Posicion de la bola (Fixed point << 8)
	s32 ball_fy;
	s32 ball_fz;
	s32 mov_x;			// Factor de movimiento
	s32 mov_y; 
	bool ball_touch;	// Has tocado la bola
	bool ball_iner;		// La bola esta en estado de inercia
	bool is_touch;		// Flag de primer toque
	s32 cam_x;			// Posicion de la camara
	s32 cam_y;
	s32 cam_z;
	s32 width;			// Medidas del decorado
	s32 height;
	s32 background;		// Posicion del fondo (2D)
	s32 minimap_x;		// Posicion del minimapa
	s32 minimap_y;
	s32 miniball_x;		// Posicion de la bola en el minimapa
	s32 miniball_y;
	s32 aura_angle;		// Control del aura
	s8 aura_alpha;	
	s32 aura_peak[8];
	s16 aura_peak_speed[8];
} p03world_info;
extern p03world_info P03WORLD;

// Shape de la bola
extern s32 P03_BALL_SHAPE[8];



//////////////////////////////////
// Puzzle 03 - Laberinto (Menu)  //
//////////////////////////////////

// Controla el ROLL del menu del puzzle
typedef struct {
	s16 y;			// Posicion Y del sprite
	s32 fy;			// Posicion Y del sprite (<< 8)
	s8 gfx;			// Gfx del sprite
	bool bn;		// Control del blanco y negro
} p03mroller_info;
extern p03mroller_info P03MROLLER[5];



////////////////////////////////
// Puzzle 04 - Cubos (Juego) //
////////////////////////////////

#define P04_CUBE_SIZE (48 << 4)		// Tamaño de cada cubo

#define P04_CUBE_TEXTURE 0			// Primera textura a usar para los cubos
#define P04_CUBE_PAL 0				// Primera paleta a usar para los cubos

#define P04_BG_TEXTURE (P04_CUBE_TEXTURE + 49)		// Textura para el fondo
#define P04_BG_PAL (P04_CUBE_PAL + 5)				// Paleta para el fondo

#define P04_MENU_TEXTURE (P04_BG_TEXTURE + 3)		// Textura para el menu
#define P04_MENU_PAL (P04_BG_PAL + 1)				// Paleta para el menu

#define P04_CUBE_POSX (223 << 4)	// Posicion del primer cubo
#define P04_CUBE_POSY (167 << 4)
#define P04_CUBE_POSZ (0 << 4)

#define P04_PANEL_POSX_R 223		// Coordedenada del panel
#define P04_PANEL_POSY_R 24
#define P04_PANEL_POSX_L 32
#define P04_PANEL_POSY_L 167

#define P04_CUBE_SPEED_Z (2 << 4)	// Velocidad de movimiento sobre el Eje Z
#define P04_CUBE_SPEED_ROT 8		// Velocidad de rotacion del cubo

#define P04_CUBE_MOVE 32				// Distancia para reconocer el comando
#define P04_CUBE_DEADZONE 8				// Zona muerta
#define P04_CUBE_MINTIME 3				// Minimo de tics para aceptar un click

#define P04_BG_ROW 5
#define P04_BG_COL 4
#define P04_BG_CUBENUM 20				// Cubos del fondo
#define P04_BG_CUBESIZE (64 << 4)		// Tamaño del cubo
#define P04_BG_CUBE_X (420 << 4)		// Coordenadas
#define P04_BG_CUBE_Y (320 << 4)
#define P04_BG_CUBE_Z -(192 << 4)

#define P04_PUZZLE_MIX 11				// Mezclado del puzzle (11)



// Control del render 2D/3D
typedef struct {
	s32 cube_posx[12];		// Posicion del cubo (cara frontal, esquina superior izquierda)
	s32 cube_posy[12];
	s32 cube_posz[12];
	s16 cube_angle[12];		// Angulo del cubo
	s8 cube_rot[12];		// Rotacion de este cubo, pendiente
	s16 cube_rotation;
	s8 cube_z[12];			// Zoom de los cubos
	s16 cube_zoom;
	s32 origin;				// Limites de distancia
	s32 moveto;
	s32 eye_x;				// Parametros de la camara
	s32 eye_y;
	s32 eye_z;
	s32 lookat_x;
	s32 lookat_y;
	s32 lookat_z;
	s32 cam_up;
	s32 bg_angle[P04_BG_CUBENUM];		// Angulo para la animacion del fondo
	s32 bgpos;				// Posicion del fondo 2D
	s32 moves;				// Movimientos
	s32 time;				// Tiempo de juego
	bool record;			// Se ha producido un record?
	s8 r_frame;				// Control de la animacion
	s8 r_timer;
} p04world_info;
extern p04world_info P04WORLD;



//////////////////////////////////
// Datos a guardar del savegame //
//////////////////////////////////

#define SAVEGAMEVERSION 20110831	// Version del savegame
#define USE_SRAM 1					// (0) No usar SRAM, (1) Usar SRAM
#define SG_SLOTS 20					// Numero de slots por puzzle
#define SG_MINIGAMES 10				// Numero de puzzles

extern char SAVEFILEPATH[256];		// Guarda el PATH del savegame
extern bool IS_EMULATOR;			// Guarda si es la consola o un emulador

// Checksum del savedata
typedef struct {
	u32 value;			// Valor Checksum
	u32 version;
	char magic[12];		// MINDMAZE
} checksum_info;
extern checksum_info CHECKSUM[1];	// Checksum del savedata

// Opciones del juego
typedef struct {
	bool lefthanded;		// Diestro o zurdo?
	u8 language;			// Idioma
} gameoptions_info;
extern gameoptions_info GAMEOPTIONS[1];

// Datos de guardado de los puzzles
typedef struct {
	u32 points;					// Puntos totales acumulados
	u32 credits;				// Puntos disponibles para comprar
	u32 cost;					// Coste para comprar el puzzle
	u32 played[SG_SLOTS];		// Veces jugado
	u32 solved[SG_SLOTS];		// Veces resuelto
	bool locked[SG_SLOTS];		// Bloqueado?
	u32 year[SG_SLOTS];			// Año
	u32 month[SG_SLOTS];		// Mes
	u32 day[SG_SLOTS];			// Dia
	u32 time[SG_SLOTS];			// Mejor tiempo
	u32 moves[SG_SLOTS];		// Mejor en movimientos
	u32 score[SG_SLOTS];		// Puntos acumulados con este puzzle
} savedata_info;
extern savedata_info SAVEDATA[SG_MINIGAMES];



//////////////////////////////////////
// Variables del control de sonidos //
//////////////////////////////////////

// Escructura de control de la musica
typedef struct {
	bool playback;	// Flag de si se esta reproduciendo
	s16 track_id;	// ID de la musica que esta cargada
	s16 volume;		// Volumen de la musica
} soundtrack_info;
extern soundtrack_info SOUNDTRACK;		// BGM que esta sonando

// Escructura de control de los sonidos
#define SFX_MAXNUM 10
typedef struct {
	u16 sfx_id;		// ID del SFX
	u16 handle;		// ID asignado al reproducir este SFX
	s16 volume;		// Volumen del SFX
	s16 freq;		// Frecuencia del SFX
} sfx_info;
extern sfx_info SFX[SFX_MAXNUM];		// Control de los SFX





#endif
