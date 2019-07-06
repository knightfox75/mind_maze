#ifndef __PZ02GAME_H__
#define __PZ02GAME_H__





// Mind Maze - Funciones del Puzzle 2 - Parejas (Juego)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion KernelPuzzle02()
extern u8 KernelPuzzle02(u8 puzzle);
// Nucleo de ejecucion del puzzle


// Funcion LoadPuzzle02Files();
void LoadPuzzle02Files(u8 puzzle);
// Carga los archivos del Puzzle 02


// Funcion CreatePuzzle02();
void CreatePuzzle02(u8 puzzle);
// Crea el Puzzle 02



// Funcion InitPuzzle02Data();
void InitPuzzle02Data(u8 puzzle);
// Inicializa los datos para el puzzle 02



// Funcion GetPuzzle02SpriteId();
extern u8 GetPuzzle02SpriteId(void);
// Obtiene la ID del sprite tocado, 255 si no se ha tocado ninguno



// Funcion PlayPuzzle02();
extern u8 PlayPuzzle02(u8 puzzle);
// Nucleo de la partida del Puzzle 02



// Funcion Puzzle02MakeDeck();
void Puzzle02MakeDeck(void);
// Crea un mazo de cartas



// Funcion Puzzle02Results();
void Puzzle02Results(u8 puzzle, u32 time, u32 moves);
// Verifica los resultados al terminar el puzzle



// Funcion EndPuzzle02();
void EndPuzzle02(u8 end);
// Al finalizar el puzzle, muestra el report si es necesario y espera 5 segundos



// Funcion EndMenuPuzzle02();
extern u8 EndMenuPuzzle02(u8 end);
// Menu de repeticion del puzzle



// Funcion RetryPuzzle02();
void RetryPuzzle02(u8 puzzle);
// Prepara el puzzle para ser rejugado



// Puzzle02UpdateMarkers();
void Puzzle02UpdateMarkers(void);
// Controla los marcadores de rondas completadas





#endif
