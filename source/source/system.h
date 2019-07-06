#ifndef __SYSTEM_H__
#define __SYSTEM_H__





// Mind Maze - Funciones de sistema
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>





// Funcion BootGame();
void BootGame(void);
// Inicializa el engine del juego



// Funcion InitCustomStructs
void InitCustomStructs(void);
// Inicializa las estructuras especificas de este juego



// Funcion ReadKeypad();
void ReadKeypad(void);
// Lee el keypad y actualiza el array de datos del mismo



// Funcion ReadTouchpad();
void ReadTouchpad(void);
// Lee el TOUCHPAD y actualiza el array de datos del mismo



// Funcion FadeIn();
void FadeIn(u8 screen, u16 speed);
// Efecto FadeIn en ambas pantallas
// Toma el control del programa hasta finalizar la ejecuccion



// Funcion FadeOut();
void FadeOut(u8 screen, u16 speed);
// Efecto FadeOut en ambas pantallas
// Toma el control del programa hasta finalizar la ejecuccion



// Funcion WaitTime();
void WaitTime(u16 wait);
// Espera los frames indicados
// Toma el control del programa hasta finalizar la ejecucion



// Funcion ResetAll();
void ResetAll(void);
// Vacia todos los buffers



// Funcions UpdateBothScreens2D();
void UpdateBothScreens2D(void);
// Actualiza los Sprites (2D) y espera al sincronismo



// Funcion Set3DinTouchScreen()
void Set3DinTouchScreen(void);
// Habilita en Engine 3D en la pantalla tactil



// Funcion UpdateSprite();
void UpdateSprite(u8 screen, u8 sprite, u8 gfx, u8 pal);
// Cambia el grafico y paleta para el Sprite dado



// Funcion GetCurrentDate();
void GetCurrentDate(u32* day, u32* month, u32* year);
// Obten la fecha actual



// Funcion GetAngle();
s16 GetAngle(s32 x1, s32 y1, s32 x2, s32 y2);
// Obten el angulo entre dos puntos



// Funcion GetDistance();
u32 GetDistance(s32 x1, s32 y1, s32 x2, s32 y2);
// Devuelve la distancia entre dos puntos



// Funcion LoadingIn();
void LoadingIn(void);
// Muestra la pantalla de "Cargando..."



// Funcion LoadingOut();
void LoadingOut(void);
// Elimina la pantalla de "Cargando..."



// Funcion AbortKeys();
extern bool AbortKeys(void);
// Devuelve si se ha pulsado alguna tecla de abortar





#endif
