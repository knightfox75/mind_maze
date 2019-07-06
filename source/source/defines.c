
// Mind Maze - Definiciones y variables globales
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





////////////////////////////////////
// Defines del control de entrada //
////////////////////////////////////

// Array para almacenar las teclas "HELD"
keypad_info KEYHELD;

// Array para almacenar las teclas "DOWN"
keypad_info KEYPRESS;

// Array para almacenar las teclas "UP"
keypad_info KEYUP;

// Array para almacenar la info de la pantalla tactil
touchscreen_info TOUCHPAD;


/////////////////
// Uso general //
/////////////////

// Uso comun en todos los menus
menu_info MENU;


////////////////////
// Menu Principal //
////////////////////

// Controla el ROLL del menu principal
mmroller_info MMROLLER[5];



//////////////////////
// Menu de opciones //
//////////////////////

// Controla los iconos del menu de opciones
optm_info OPTM[OPTMITEMS];

// Controla el menu de opciones
optmcontrol_info OPTMCONTROL;



//////////////////////
// Creditos			//
//////////////////////

// Datos del tunel
creditsworld_info CREDITSWORLD;

// Datos del panel de creditos
credits_info CREDITS;




///////////////////////////////////////////
// Puzzle 01 - Puzzle deslizante (Juego) //
///////////////////////////////////////////

// Gestion de las fichas del puzzle
puzzle01spr_info PUZZLE01SPR[12];

// Gestion del movimiento de las fichas
puzzle01mov_info PUZZLE01MOV;



///////////////////////////////////////////
// Puzzle 01 - Puzzle deslizante (Menu)  //
///////////////////////////////////////////

// Controla el ROLL del menu del puzzle
p01mroller_info P01MROLLER[5];

// Textos en el menu
p01text_info P01TEXT[P01_ITEMS];



/////////////////////////////////
// Puzzle 02 - Parejas (Juego) //
/////////////////////////////////

// Informacion de cada mazo
puzzle02info_info PUZZLE02INFO[P02_ITEMS];

// Informacion de las cartas
puzzle02spr_info PUZZLE02SPR[9];

// Informacion de las cartas del mazo actual
puzzle02deck_info PUZZLE02DECK;



/////////////////////////////////
// Puzzle 02 - Parejas (Menu)  //
/////////////////////////////////

// Controla el ROLL del menu del puzzle
p02mroller_info P02MROLLER[5];



///////////////////////////////////
// Puzzle 03 - Laberinto (Juego) //
///////////////////////////////////

// Informacion de cada laberinto
puzzle03info_info PUZZLE03INFO[P03_ITEMS];

// Controla el mundo 3D
p03world_info P03WORLD;

// Shape de la bola
s32 P03_BALL_SHAPE[8];


///////////////////////////////////
// Puzzle 03 - Laberinto (Menu)  //
///////////////////////////////////

// Controla el ROLL del menu del puzzle
p03mroller_info P03MROLLER[5];



////////////////////////////////
// Puzzle 04 - Cubos (Juego) //
////////////////////////////////

// Control del render 3D
p04world_info P04WORLD;



//////////////////////////////////
// Datos a guardar del savegame //
//////////////////////////////////

char SAVEFILEPATH[256];		// Guarda el PATH del savegame
bool IS_EMULATOR;			// Guarda si es la consola o un emulador

// Checksum del savedata	
checksum_info CHECKSUM[1];

// Opciones del juego
gameoptions_info GAMEOPTIONS[1];

// Datos de guardado de los puzzles
savedata_info SAVEDATA[SG_MINIGAMES];



//////////////////////////////////////
// Variables del control de sonidos //
//////////////////////////////////////

// Escructura de control de la musica
soundtrack_info SOUNDTRACK;		// BGM que esta sonando

// Escructura de control de los sonidos
sfx_info SFX[SFX_MAXNUM];		// Control de los SFX
