#ifndef __GFXEFFECTS_H__
#define __GFXEFFECTS_H__





// Mind Maze - Funciones de effectos graficos
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>



// Funcion CrossDisolve();
extern bool CrossDisolve(u8 screen, u8 origin, u8 destination);
// Efecto cross disolve entre pantallas


// Funcion Rotate16bitsImage();
void Rotate16bitsImage(u8 id);
// Rota 180º la imagen de 16 bits del slot indicado


// Funcion RotateTiledBg();
void RotateTiledBg(const char* name);
// Rota 180º los tiles del fondo con el nombre indicado


// Funcion RotateSprite();
void RotateSprite(u16 id);
// Rota 180º el grafico del sprite indicado


// Funcion Rotate3dSprite();
void Rotate3dSprite(u16 id);
// Rota 180º el grafico del sprite 3D indicado


// Funcion DesaturateSprite();
void DesaturateSprite(u8 screen, u8 pal, u8 brightness);
// Desatura el sprite, pudiendo especificar una compensacion de brillo (0-31)


// Funcion Desaturate3dSprite();
void Desaturate3dSprite(u8 pal, u8 brightness);
// Desatura el sprite 3D, pudiendo especificar una compensacion de brillo (0-31)


// Funcion void RotateMap();
void RotateMap(u8 slot);
// Rota 180º un mapa de colisiones


// Funcion RotateTexture();
void RotateTexture(u16 id);
// Rota 180º la textura





#endif
