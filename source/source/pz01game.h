#ifndef __PZ01GAME_H__
#define __PZ01GAME_H__





// Mind Maze - Funciones del Puzzle 1 - Puzzle deslizante (Juego)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion KernelPuzzle01()
extern u8 KernelPuzzle01(u8 puzzle);
// Nucleo de ejecucion del puzzle


// Funcion MixPuzzle01()
void MixPuzzle01(u8 amount);
// Mezcla las fichas del puzzle


// Funcion PlayPuzzle01()
extern u8 PlayPuzzle01(u8 puzzle);
// "Juega" al puzzle 01


// Funcion EndPuzzle01();
void EndPuzzle01(u8 end);
// Una vez resuleto el puzzle 01...


// Funcion EndMenuPuzzle01();
extern u8 EndMenuPuzzle01(u8 end);
// Menu del final de puzzle


// Funcion RetryPuzzle01();
void RetryPuzzle01(void);
// Prepara el puzzle para repetirlo


// Funcion LoadPuzzle01Files();
void LoadPuzzle01Files(u8 puzzle);
// Carga los archivos del Puzzle 01


// Funcion CreatePuzzle01();
void CreatePuzzle01(void);
// Crea el Puzzle 01


// Funcion InitPuzzle01Data();
void InitPuzzle01Data(void);
// Inicializa los datos para el puzzle 01


// Funcion GetPuzzle01SpriteId();
extern u8 GetPuzzle01SpriteId(void);
// Obten la ID del sprite tocado


// Funcion GetPuzzle01Move();
extern u8 GetPuzzle01Move(u8 id);
// Obten en que direccion puede moverse la ficha


// Funcion Puzzle01CheckGoodPlace();
extern u8 Puzzle01CheckGoodPlace(void);
// Verifica las posicion de cada ficha, marca las correctas y devuelve su numero


// Funcion Puzzle01Results();
void Puzzle01Results(u8 puzzle, u32 time, u32 pmoves);
// Si se ha resuleto el puzzle, calcula las puntuaciones y crea el informe





#endif
