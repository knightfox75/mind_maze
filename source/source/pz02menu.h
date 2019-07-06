#ifndef __PUZZ02MENU_H__
#define __PUZZ02MENU_H__





// Mind Maze - Funciones del Puzzle 2 - Parejas (Menu)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion MenuPuzzle02();
u8 MenuPuzzle02(void);
// Menu para seleccionar el primer puzzle


// Funcion LoadPuzzle02MenuFiles();
void LoadPuzzle02MenuFiles(void);
// Carga los archivos necesarios


// Funcion CreatePuzzle02Menu();
void CreatePuzzle02Menu(void);
// Crea el menu del primer puzzle


// Funcion Puzzle02MenuSprites();
void Puzzle02MenuSprites(u8 id);
// Crea los Sprites para el menu del primer puzzle


// Funcion Puzzle02MenuMove();
void Puzzle02MenuMove(s32 move);
// Mueve los iconos del menu


// Funcion Puzzle02MenuItemTouch();
extern u8 Puzzle02MenuItemTouch(s16 x, s16 y);
// Detecta si se ha tocado un icono del menu


// Funcion Puzzle02MenuText();
void Puzzle02MenuText(u8 id, u8 page);
// Textos del menu


// Funcion Puzzle02Buy();
extern bool Puzzle02Buy(u8 id);
// Compra el puzzle seleccionado si hay credito suficiente





#endif
