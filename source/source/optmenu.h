#ifndef __OPTMENU_H__
#define __OPTMENU_H__





// Mind Maze - Funciones del Menu de opciones
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion OptMenu();
void OptMenu(void);
// Menu de opciones del juego



// Funcion LoadOptMenuFiles();
void LoadOptMenuFiles(void);
// Carga los archivos necesarios para el menu de opciones



// Funcion CreateOptMenu();
void CreateOptMenu(void);
// Crea el menu de opciones



// Micro kernel del menu de opciones
extern bool OPTM_Kernel(void);
// Micro kernel del menu de opciones



// Menu de opciones, menu principal
extern u8 OPTM_Main(void);
// Ejecuta el menu principal



// Funcion OPTM_LangMenIn();
extern bool OPTM_LangMenIn(u8 thread);
// Animacion entrada menu seleccion idioma



// Funcion OPTM_LangMenOut();
extern bool OPTM_LangMenOut(u8 thread);
// Animacion salida menu seleccion idioma



// Funcion OPTM_HandMenIn();
extern bool OPTM_HandMenIn(u8 thread);
// Animacion entrada menu seleccion orientacion



// Funcion OPTM_HandMenOut();
extern bool OPTM_HandMenOut(u8 thread);
// Animacion salida menu seleccion orientacion



// Funcion OPTM_DeleteMenIn();
extern bool OPTM_DeleteMenIn(u8 thread);
// Animacion entrada menu del borrado de datos



// Funcion OPTM_DeleteMenOut();
extern bool OPTM_DeleteMenOut(u8 thread);
// Animacion salida menu del borrado de datos



// Funcion GetOrientationAtBoot();
void GetOrientationAtBoot(void);
// Si noy informacion de datos de guardado, pide la orientacion
// de la consola antes de iniciar el juego





#endif
