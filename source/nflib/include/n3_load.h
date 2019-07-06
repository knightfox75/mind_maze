#ifdef __cplusplus
extern "C" {
#endif

#ifndef __N3_LOAD_H__
#define __N3_LOAD_H__





// NightFox 3D LIB - Funciones de carga de archivos
// Requiere DevkitARM R30
// Codigo por NightFox
// http://www.nightfoxandco.com/
// Version 20111011 - BETA



// Includes devKitPro
#include <nds.h>





// Funcion N3_InitBuffers();
void N3_InitBuffers(void);
// Inicializa los buffers en RAM para el alamcenamiento de texturas y paletas.
// Esta funcion debe ser usada 1 SOLA VEZ antes de cargar ningun archivo.



// Funcion N3_ResetBuffers();
void N3_ResetBuffers(void);
// Vacia todos los buffers (libera la RAM usada) e inicializa todas las
// estructuras de controls de estos buffers.



// Funcion N3_RootPath();
void N3_RootPath(const char* path);
// Define el ROOT del sistema de archivos.
// Si se especifica "NITROFS" como root, se usara NitroFS como sistema de archivos.
// En caso contrario, se usara FAT con la ruta especificada.
// Esta ruta no puede tener mas de 32 caracteres de longitud.
// Esta funcion NO INICIALIZA el sistema de archivos. Usala si ya has iniciado el
// sistema de archivos desde otra funcion o libreria.



// Funcion N3_InitFilesystem();
void N3_InitFilesystem(const char* path);
// Define el ROOT del sistema de archivos.
// Si se especifica "NITROFS" como root, se usara NitroFS como sistema de archivos.
// En caso contrario, se usara FAT con la ruta especificada.
// Esta ruta no puede tener mas de 32 caracteres de longitud.
// Esta funcion INICIALIZA el sistema de archivos. Usala si no has iniciado el
// sistema de archivos desde otra funcion o libreria.



// Funcion N3_LoadTexture();
void N3_LoadTexture(const char* file, u16 id,  u16 width, u16 height);
// Carga una textura en RAM desde el sistema de archivos.
// Debes especificar la ruta, slot en RAM donde cargarla y las medidas
// en pixeles de la textura.



// Funcion N3_UnloadTexture();
void N3_UnloadTexture(u16 id);
// Borra de la RAM la textura cargada en el SLOT indicado.



// Funcion N3_LoadPal();
void N3_LoadPal(const char* file, u16 id);
// Carga una paleta en RAM desde el sistema de archivos.
// Debes especificar la ruta y el slot en RAM donde cargarla.



// Funcion N3_UnloadPal();
void N3_UnloadPal(u16 id);
// Borra de la RAM la paleta cargada en el SLOT indicado.





#endif

#ifdef __cplusplus
}
#endif
