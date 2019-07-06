#ifndef __PUZZ01MENU_H__
#define __PUZZ01MENU_H__





// Mind Maze - Funciones del Puzzle 1 - Puzzle deslizante (Menu)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion MenuPuzzle01();
u8 MenuPuzzle01(void);
// Menu para seleccionar el primer puzzle


// Funcion LoadPuzzle01MenuFiles();
void LoadPuzzle01MenuFiles(void);
// Carga los archivos necesarios


// Funcion CreatePuzzle01Menu();
void CreatePuzzle01Menu(void);
// Crea el menu del primer puzzle


// Funcion Puzzle01MenuSprites();
void Puzzle01MenuSprites(u8 id);
// Crea los Sprites para el menu del primer puzzle


// Funcion Puzzle01MenuMove();
void Puzzle01MenuMove(s32 move);
// Mueve los iconos del menu


// Funcion Puzzle01MenuItemTouch();
extern u8 Puzzle01MenuItemTouch(s16 x, s16 y);
// Detecta si se ha tocado un icono del menu


// Funcion Puzzle01MenuText();
void Puzzle01MenuText(u8 id, u8 page);
// Textos del menu


// Funcion Puzzle01Buy();
extern bool Puzzle01Buy(u8 id);
// Compra el puzzle seleccionado si hay credito suficiente





#endif
