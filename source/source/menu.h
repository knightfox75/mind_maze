#ifndef __MENU_H__
#define __MENU_H__





// Mind Maze - Funciones del menu principal
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>





// Funcion Main Menu
extern u8 MainMenu(void);
// Menu principal del juego



// Funcion LoadMainMenuFiles();
void LoadMainMenuFiles(void);
// Carga todos los archivos necesarios del menu principal



// Funcion CreateMainMenu();
void CreateMainMenu(void);
// Crea el menu principal



// Funcion MainMenusSrites();
void MainMenuSprites(u8 id);
// Crea los Sprites para el menu principal



// Funcion MainMenuMove();
void MainMenuMove(s32 move);
// Mueve los iconos del menu



// Funcion MainMenuItemTouch();
extern u8 MainMenuItemTouch(s16 x, s16 y);
// Detecta si se ha tocado un icono del menu





#endif
