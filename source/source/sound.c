
// Mind Maze - Funciones de sonido (MAXMOD)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010



// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propietarios NDS
#include <nds.h>

// Includes de MAXMOD
#include <maxmod9.h>

// Include del banco de sonidos
#include "sound_index.h"

// Includes NightFox's Lib
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"




// Funcion InitSound();
void InitSound(void) {

	// Inicializa MAXMOD con en banco de sonido seleccionado
	mmInitDefault("snd/mindmaze.sbf");
	swiWaitForVBlank();	

	// Modo Hardware
	mmSelectMode(MM_MODE_A);
	swiWaitForVBlank();

	// No esta sonando ningun MOD
	SOUNDTRACK.playback = false;
	SOUNDTRACK.track_id = -1;

}



// Funcion LoadBgm();
void LoadBgm(u32 bgm) {

	// Guarda el ID del track
	SOUNDTRACK.track_id = bgm;

	// Carga el track
	mmLoad(bgm);
	swiWaitForVBlank();	

}



// Funcion PlayBgm();
void PlayBgm(void) {

	// Reproduce el mod
	mmStart(SOUNDTRACK.track_id, MM_PLAY_LOOP);
	swiWaitForVBlank();	

	// Ajusta y guarda el volumen
	BgmVolume(1024);
	SOUNDTRACK.volume = 1024;

	// Y marca que se esta reproduciendo un MOD
	SOUNDTRACK.playback = true;

}



// Funcion StopBgm();
void StopBgm(void) {

	// Deten el BGM actual
	mmStop();
	swiWaitForVBlank();

	// Descargalo de la memoria
	mmUnload(SOUNDTRACK.track_id);
	swiWaitForVBlank();	

	// Y marca que no esta sonando ningun MOD
	SOUNDTRACK.playback = false;
	SOUNDTRACK.track_id = -1;
	
}



// Funcion LoadSfx();
void LoadSfx(u32 id) {

	// Guarda el ID del SFX
	SFX[id].sfx_id = id;

	// Carga el SFX en RAM
	mmLoadEffect(id);
	swiWaitForVBlank();	

}



// Funcion UnloadSfx();
void UnloadSfx(u32 id) {

	// Descarga el SFX en RAM
	mmUnloadEffect(SFX[id].sfx_id);
	swiWaitForVBlank();	

}



// Funcion PlaySfx();
void PlaySfx(u32 id) {

	// Reproduce el SFX del ID indicado
	SFX[id].handle = mmEffect(SFX[id].sfx_id);

	// Al volumen indicado
	SfxVolume(id, 1024);

}



// Funcion StopSfx();
void StopSfx(u32 id) {

	// Deten el SFX del ID indicado
	mmEffectCancel(SFX[id].handle);

}



// Funcion StopAllSfx();
void StopAllSfx(void) {

	// Deten todos los SFX
	mmEffectCancelAll();
	swiWaitForVBlank();

}



// Funcion SfxFreq();
void SfxFreq(u32 id, u32 freq) {

	// Cambia la frecuencia del sample
	mmEffectRate(SFX[id].handle, freq);

}



// Funcion BgmVolume();
void BgmVolume(u32 volume) {

	// Variables
	s32 level = volume;

	// Fix del volumen
	if (level < 0) level = 0;
	if (level > 1024) level = 1024;

	// Aplica el volumen
	mmSetModuleVolume(level);

	// Guarda el volumen actual
	SOUNDTRACK.volume = volume;

}



// Funcion SfxVolume();
void SfxVolume(u32 id, u32 volume) {

	// Variables
	s32 level = volume;

	// Fix del volumen
	if (level < 0) level = 0;
	if (level > 255) level = 255;

	// Aplica el volumen
	mmEffectVolume(SFX[id].handle, level);

	// Guarda el volumen actual
	SFX[id].volume = volume;

}
