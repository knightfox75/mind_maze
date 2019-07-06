#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_SYSTEM_H__
#define __N3_SYSTEM_H__





// NightFox 3D LIB - Funciones de sistema
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>





// Funcion N3_Set3D();
void N3_Set3D(u8 screen, u8 mode);
// Inicializa el modo 3D en la pantalla seleccionada. De seleccionarse la
// pantalla inferior para el 3D, las referencias de las funciones 2D quedaran
// invertidas, usando 0 para referirse a la pantalla inferior y 1 a la
// pantalla superior.



// Funcion N3_Init3dEngine();
void N3_Init3dEngine(void);
// Inicializa el OpenGL y bancos de memoria necesarios para las texturas.



// Funcion N3_GetTextureSize();
u16 N3_GetTextureSize(u16 textel);
// Funcion de uso interno (no documentar)
// Devuelve la BASE 2 del tamaño de la textura (0 - 7).
// Devuelve 255 en caso de ser un tamaño ilegal.



// Funcion N3_3dLayer();
void N3_3dLayer(u8 layer);
// Establece la prioridad de la capa de dibujado del 3D sobre las
// demas capas 2D. Los valores validos de 0 (por defecto) a 3.



// Funcion N3_DmaMemCopy();
void N3_DmaMemCopy(void* destination, const void* source, u32 size);
// Copia un bloque de memoria usando, si es posible, el canal DMA.
// Usalo solo para copiar bloques de RAM a VRAM y viceversa.





#endif

#ifdef __cplusplus
}
#endif
