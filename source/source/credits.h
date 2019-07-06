#ifndef __CREDITS_H__
#define __CREDITS_H__





// Mind Maze - Funciones de Creditos
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>


// Funcion CreditsMain();
void CreditsMain(void);
// Funcion global de los creditos


// Funcion CreditsLoadFiles();
void CreditsLoadFiles(void);
// Carga los archivos necesarios


// Funcion CreditsCreateWorld();
void CreditsCreateWorld(void);
// Crea el entorno grafico


// Funcion CreditsMoveTunnel();
void CreditsMoveTunnel(void);
// Mueve la camara y regenera el decorado


// Funcion CreditsRender3dWorld();
void CreditsRender3dWorld(void);
// Renderiza los graficos en 3D


// Funcion CreditsRenderTunnel();
void CreditsRenderTunnel(void);
// Renderiza el tunel


// Funcion CreditsLoadMsgFiles();
void CreditsLoadMsgFiles(void);
// Carga los archivos necesarios para los creditos


// Funcion CreditsCreateMsgPanel();
void CreditsCreateMsgPanel(void);
// Crea el panel de creditos


// Funcion CreditsUpdateMsgPanel();
bool CreditsUpdateMsgPanel(void);
// Actualiza el panel de creditos con un nuevo texto


// Funcion CreditsUpdateVramPanelTextures();
void CreditsUpdateVramPanelTextures(void);
// Actualiza las texturas en VRAM


// Funcion CreditsDefaultMsgPanelTextures();
void CreditsDefaultMsgPanelTextures(void);
// Actualiza las 36 texturas del panel de creditos


// CreditsWriteTextPage();
extern bool CreditsWriteTextPage(void);
// Prepara una pagina de texto


// Funcion CreditsWriteTextOnTexture();
void CreditsWriteTextOnTexture(const char* text, u16 pos_y);
// Escribe el texto en las texturas correspondientes


// Funcion CreditsRenderMsgPanel();
void CreditsRenderMsgPanel(void);
// Dibuja el panel de creditos


// Funcion CreditsMovePanel();
extern bool CreditsMovePanel(void);
// Mueve el panel de creditos


// Funcion CreditsRenderCube();
void CreditsRenderCube(s32 up);
// Renderiza el cubo





#endif
