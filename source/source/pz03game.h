#ifndef __PZ03GAME_H__
#define __PZ03GAME_H__





// Mind Maze - Funciones del Puzzle 3 - Labrinto (Juego)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion KernelPuzzle03()
extern u8 KernelPuzzle03(u8 puzzle);
// Nucleo de ejecucion del puzzle



// Funcion PlayPuzzle03();
extern u8 PlayPuzzle03(u8 puzzle);
// Rutina de juego del Puzzle 03



// Funcion LoadPuzzle03Files();
void LoadPuzzle03Files(u8 puzzle);
// Carga los archivos necesarios para el puzzle



// Funcion CreatePuzzle03();
void CreatePuzzle03(u8 puzzle);
// Crea el puzzle 03



// Funcion Puzzle03Render3dWorld();
void Puzzle03Render3dWorld(void);
// Crea el mundo en 3D segun el mapa de colisiones dado



// Funcion Puzzle03RenderPath();
void Puzzle03RenderPath(void);
// Renderiza la linea entre la bola y el stylus



// Funcion Puzzle03RenderAura();
void Puzzle03RenderAura(void);
// Renderiza el efecto "aura" de la bola



// Funcion Puzzle03Start();
void Puzzle03Start(void);
// Animacion de inicio del laberinto



// Funcion Puzzle03Exit();
void Puzzle03Exit(u8 option);
// Animacion de salida del laberinto



// Funcion Puzzle03MoveBall();
extern u8 Puzzle03MoveBall(void);
// Mueve la bola por el escenario



// Funcion Puzzle03CheckGoal();
extern bool Puzzle03CheckGoal(void);
// Verifica y devuelve si la bola ha alcanzado la meta



// Puzzle03DrawMinimap();
void Puzzle03DrawMinimap(void);
// Dibuja el minimapa



// Funcion Puzzle03WarFog();
void Puzzle03WarFog(s32 x, s32 y, s32 width, s32 height);
// Descubre el minimapa (Niebla de guerra)



// Funcion Puzzle03GfxEngine();
void Puzzle03GfxEngine(void);
// Renderiza todos los graficos necesarios para el juego



// Funcion Puzzle03DrawEndMenu();
void Puzzle03DrawEndMenu(s32 bx0, s32 bx1, s32 bx2, s32 by);
// Renderiza los botones del menu



// Funcion Puzzle03Results();
void Puzzle03Results(u8 puzzle, u32 time, u32 moves);
// Calcula los resultados del puzzle



// Funcion EndPuzzle03();
void EndPuzzle03(u8 end);
// Operacion a la finalizacion del puzzle



// Funcion EndMenuPuzzle03();
u8 EndMenuPuzzle03(u8 end);
// Menu de fin de puzzle



// Funcion RetryPuzzle03();
void RetryPuzzle03(u8 puzzle);
// Reinicia el puzzle 03





#endif
