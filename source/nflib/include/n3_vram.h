#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_VRAM_H__
#define __N3_VRAM_H__





// NightFox 3D LIB - Funciones de gestion de VRAM
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>





// Funcion N3_VramTexture();
void N3_VramTexture(u16 ram, u16 vram, bool keepframes);
// Copia una textura cargada en RAM a la VRAM.
// Debes especificar el SLOT de origen en RAM, el de destino en la VRAM y
// si se copiaran o no todos los frames (false) o solo el primero (true)



// Funcion N3_FreeTexture();
void N3_FreeTexture(u16 vram);
// Borra la textura indicada de la VRAM y marca el slot como disponible.
// Si es necesario, desfragmenta la VRAM.



// Funcion N3_VramTextureDefrag();
void N3_VramTextureDefrag(void);
// Reordena la VRAM usada por las texturas.



// Funcion N3_VramPalette();
void N3_VramPalette(u8 ram, u8 vram);
// Copia una Paleta cargada en RAM a la VRAM.
// Debes especificar el SLOT de origen en RAM, el de destino en la VRAM.



// Funcion N3_TextureFrame();
void N3_TextureFrame(u8 texture, u8 frame);
// Cambia el frame mostrar de la textura indicada.
// Debes indicar el SLOT de la textura en VRAM y el numero de frame.



// Funcion N3_UpdateTexturesVram();
void N3_UpdateTexturesVram(void);
// Actualiza, si es necesario, las texturas en VRAM, en caso de que estas
// sean animadas y hayan cambiado de frame.
// Utiliza esta funcion despues del VBLANK.





#endif

#ifdef __cplusplus
}
#endif
