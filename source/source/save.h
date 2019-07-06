#ifndef __SAVE_H__
#define __SAVE_H__





// Mind Maze - Funciones de guardado
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010





// Includes propietarios NDS
#include <nds.h>


// Funcion CheckSavegame();
void CheckSavegame(const char* path);
// Verifica el archivo de guardado y crealo si no existe



// Funcion WriteSavegame();
void WriteSavegame(void);
// Guarda los datos del juego



// Funcion ReadSavegame();
void ReadSavegame(void);
// Carga los datos del juego



// Funcion CheckSavegameFile();
void CheckSavegameFile(void);
// Verifica si el archivo de guardado.
// Si existe, cargalo, si no, crea uno de nuevo.



// Funcion WriteSavegame();
void WriteSavegameFile(void);
// Guarda los datos en un archivo



// Funcion ReadSavegame();
void ReadSavegameFile(void);
// Carga los datos desde un archivo



// Funcion CheckSavegameSRAM();
void CheckSavegameSRAM(void);
// EMULADOR - SRAM
// Verifica si el archivo de guardado.
// Si existe, cargalo, si no, crea uno de nuevo.



// Funcion WriteSRAM();
void WriteSRAM(void);
// Guarda los datos en SRAM (Emulador)



// Funcion WriteSRAM();
void ReadSRAM(void);
// Lee los datos en SRAM (Emulador)



// Funcion CheckFATWrite();
bool CheckFATWrite(const char* path);
// Verifica si puedes escribir en FAT



// Funcion SavedataChecksum();
u32 SavedataChecksum(void);
// Verifica la integridad de los datos



// Funcion InitGameData();
void InitGameData(void);
// Inicializa todos los datos del juego



// Funcion InitAllPuzzlesSaveData();
void InitAllPuzzlesSaveData(void);
// Inicializa los datos de guardado de TODOS los puzzles



// Funcion InitSaveDataPuzzle01();
void InitSaveDataPuzzle01(void);
// Inicializa los datos del puzzle 01



// Funcion InitSaveDataPuzzle02();
void InitSaveDataPuzzle02(void);
// Inicializa los datos del puzzle 02



// Funcion InitSaveDataPuzzle03();
void InitSaveDataPuzzle03(void);
// Inicializa los datos del puzzle 03



// Funcion InitSaveDataPuzzle04();
void InitSaveDataPuzzle04(void);
// Inicializa los datos del puzzle 04





#endif
