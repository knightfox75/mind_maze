#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_DRAW_H__
#define __N3_DRAW_H__





// NightFox 3D LIB - Funciones de dibujado de poligonos
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>





// Funcion N3_SolidTriangle();
void N3_SolidTriangle(	s16 x1, s16 y1, s16 z1,		// Coordenadas Vertice 1 (arriba)
						s16 x2, s16 y2, s16 z2,		// Coordenadas Vertice 2 (abajo / izquierda)
						s16 x3, s16 y3, s16 z3,		// Coordenadas Vertice 3 (abajo / derecha)
						u8 r, u8 g, u8 b			// Color RGB (8 bits canal)
);
// Dibuja un triangulo solido con las coordenadas y el color especificado.



// Funcion N3_TexturedTriangle();
void N3_TexturedTriangle(	s16 x1, s16 y1, s16 z1,		// Coordenadas Vertice 1 (arriba / izquierda)
							s16 x2, s16 y2, s16 z2,		// Coordenadas Vertice 2 (abajo / izquierda)
							s16 x3, s16 y3, s16 z3,		// Coordenadas Vertice 3 (abajo / derecha)
							u16 tex, u8 pal				// Textura y paleta
);
// Dibuja un triangulo con las coordenadas y la textura y paleta especificados.



// Funcion N3_SolidQuad();
void N3_SolidQuad(	s16 x1, s16 y1, s16 z1,		// Coordenadas Vertice 1 (arriba / izquierda)
					s16 x2, s16 y2, s16 z2,		// Coordenadas Vertice 2 (abajo / izquierda)
					s16 x3, s16 y3, s16 z3,		// Coordenadas Vertice 3 (abajo / derecha)
					s16 x4, s16 y4, s16 z4,		// Coordenadas Vertice 4 (arriba / derecha)
					u8 r, u8 g, u8 b			// Color RGB (8 bits canal)
);
// Dibuja un cuadrado solido con las coordenadas y el color especificado.



// Funcion N3_TexturedQuad();
void N3_TexturedQuad(	s16 x1, s16 y1, s16 z1,		// Coordenadas Vertice 1 (arriba / izquierda)
						s16 x2, s16 y2, s16 z2,		// Coordenadas Vertice 2 (abajo / izquierda)
						s16 x3, s16 y3, s16 z3,		// Coordenadas Vertice 3 (abajo / derecha)
						s16 x4, s16 y4, s16 z4,		// Coordenadas Vertice 4 (arriba / derecha)
						u16 tex, u8 pal				// Textura y paleta
);
// Dibuja un cuadrado con las coordenadas y la textura y paleta especificados.





#endif

#ifdef __cplusplus
}
#endif
