#ifndef __PUZZ03MENU_H__
#define __PUZZ03MENU_H__





// Mind Maze - Funciones del Puzzle 3 - Laberinto (Menu)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion MenuPuzzle03();
u8 MenuPuzzle03(void);
// Menu para seleccionar el primer puzzle


// Funcion LoadPuzzle03MenuFiles();
void LoadPuzzle03MenuFiles(void);
// Carga los archivos necesarios


// Funcion CreatePuzzle03Menu();
void CreatePuzzle03Menu(void);
// Crea el menu del primer puzzle


// Funcion Puzzle03MenuSprites();
void Puzzle03MenuSprites(u8 id);
// Crea los Sprites para el menu del primer puzzle


// Funcion Puzzle03MenuMove();
void Puzzle03MenuMove(s32 move);
// Mueve los iconos del menu


// Funcion Puzzle03MenuItemTouch();
extern u8 Puzzle03MenuItemTouch(s16 x, s16 y);
// Detecta si se ha tocado un icono del menu


// Funcion Puzzle03MenuText();
void Puzzle03MenuText(u8 id, u8 page);
// Textos del menu


// Funcion Puzzle03Buy();
extern bool Puzzle03Buy(u8 id);
// Compra el puzzle seleccionado si hay credito suficiente





#endif
