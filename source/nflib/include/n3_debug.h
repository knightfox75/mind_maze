#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_DEBUG_H__
#define __N3_DEBUG_H__





// NightFox 3D LIB - Funciones de DEBUG
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>





// Funcion N3_Error();
void N3_Error(u16 code, const char* text, u32 value);
// Maneja los codigos de error de la libreria y detiene la ejecucion.





#endif

#ifdef __cplusplus
}
#endif
