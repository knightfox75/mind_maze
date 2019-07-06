#ifndef __PZ04GAME_H__
#define __PZ04GAME_H__





// Mind Maze - Funciones del Puzzle 4 - Cubos (Juego)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>





// Funcion KernelPuzzle04()
void KernelPuzzle04(void);
// Nucleo de ejecucion del puzzle



// Puzzle04FadeIn();
void Puzzle04FadeIn(void);
// Efecto FADE-IN



// Funcion Puzzle04FadeOut();
void Puzzle04FadeOut(bool exit);
// Efecto FADE-OUT



// Funcion PlayPuzzle04();
extern bool PlayPuzzle04(void);
// Rutina de juego del puzzle



// Funcion LoadPuzzle04Files();
void LoadPuzzle04Files(void);
// Carga los archivos para este puzzle



// Funcion CreatePuzzle04();
void CreatePuzzle04(void);
// Crea el puzzle



// Funcion Puzzle04InitPuzzleData();
void Puzzle04InitPuzzleData(void);
// Inicializa los datos del puzzle



// Funcion Puzzle04RenderAll();
void Puzzle04RenderAll(void);
// Renderiza todos los elementos 3D de la escena



// Funcion Puzzle04DrawCube();
void Puzzle04DrawCube(u8 id);
// Dibuja el cubo solicitado



// Funcion Puzzle04Draw3dBg();
void Puzzle04Draw3dBg(void);
// Dibuja el fondo 3D



// Funcion Puzzle04DrawMenu();
void Puzzle04DrawMenu(s32 x1, s32 y1, s32 x2, s32 y2);
// Dibuja los botones del menu



// Funcion Puzzle04Engine2D();
void Puzzle04Engine2D(void);
// Renderiza todos los elementos 2D de la escena



// Funcion Puzzle04GetCubeId();
extern u8 Puzzle04GetCubeId(void);
// Obten el ID si has tocado un cubo



// Funcion Puzzle04RotateCube();
extern bool Puzzle04RotateCube(void);
// Rota los cubos marcado



// Funcion Puzzle04ZoomInCube();
extern bool Puzzle04ZoomInCube(void);
// Zoom de los cubos marcados (IN)



// Funcion Puzzle04ZoomOutCube();
extern bool Puzzle04ZoomOutCube(void);
// Zoom de los cubos marcados (OUT)



// Funcion Puzzle04Mix();
extern bool Puzzle04Mix(void);
// Mezcla el puzzle



// Funcion Puzzle04WriteReport();
void Puzzle04WriteReport(void);
// Escribe en pantalla el report



// Funcion Puzzle04CheckRecord();
extern bool Puzzle04CheckRecord(void);
// Verifica si hay record y de ser asi, guarda los datos



// Funcion Puzzle04ExitMenu();
extern bool Puzzle04ExitMenu(void);
// Menu del fin de puzzle





#endif
