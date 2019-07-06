
// Mind Maze - Funciones del Puzzle 2 - Parejas (Juego)
// Requiere DevkitARM
// Requiere NightFox's Lib
// Codigo por NightFox
// http://www.nightfoxandco.com
// Inicio 13 de Diciembre del 2010



// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


// Includes propietarios NDS
#include <nds.h>

// Includes NightFox's Lib
#include <nf_lib.h>

//Incluye las librerias del juego
#include "includes.h"





// Funcion KernelPuzzle02()
u8 KernelPuzzle02(u8 puzzle) {

	// Reinicialo todo
	ResetAll();

	// Variables
	u8 end = 0;			// Resultado del puzzle
	u8 option = 0;		// Opcion del menu de fin de puzzle
	s32 fade = 0;		// Brillo
	bool loop = true;	// Loop


	//////////////////////
	//					//
	//  Carga Archivos	//
	//					//
	//////////////////////

	// Pantalla "Cargando..."
	LoadingIn();
	// Carga los archivos necesarios
	LoadPuzzle02Files(puzzle);
	// Carga la musica
	LoadBgm((MOD_P02STG01 + puzzle));
	// Oculta la pantalla "Cargando..."
	LoadingOut();


	//////////////////////
	//					//
	//	Crea el Puzzle	//
	//					//
	//////////////////////

	// Habilita el 3D en la pantalla inferior
	Set3DinTouchScreen();
	// Crea el puzzle
	CreatePuzzle02(puzzle);
	// Reproduce la musica
	PlayBgm();


	//////////////////////////
	//						//
	//	Nucleo el Puzzle	//
	//						//
	//////////////////////////

	// Fade
	FadeIn(3, 64);

	// Repite mientras no pidas otro puzzle o salir
	while (loop) {

		// Suma un contador al numero de veces jugadas
		SAVEDATA[1].played[puzzle] ++;

		// Juega al puzzle
		end = PlayPuzzle02(puzzle);

		// Al terminar el puzzle...
		EndPuzzle02(end);

		// Menu de puzzle finalizado
		option = EndMenuPuzzle02(end);

		// Si has seleccionado repetir el mismo puzzle
		if (option == 0) {
			RetryPuzzle02(puzzle);
		} else {
			loop = false;
		}

	}

	// Fade out de la pantalla y musica
	loop = true;
	while (loop) {
		fade += 64;
		if (fade >= (16 << 8)) {
			fade = (16 << 8);
			loop = false;
			// Deten la musica
			StopBgm();
		}
		// Ajusta el brillo
		setBrightness(3, (0 - (fade >> 8)));
		// Ajusta el volumen
		BgmVolume(((4096 - fade) >> 2));
		// Espera al sincronismo
		swiWaitForVBlank();
	}

	// Devuelve el resultado de ejecucion
	// 0 - Repetir puzzle
	// 1 - Cambiar puzzle
	// 2 - Salir
	return option;

}



// Funcion LoadPuzzle02Files();
void LoadPuzzle02Files(u8 puzzle) {

	// Variables
	s16 n = 0;	
	char file[128];
	char lang[4];
	char side[4];
	u8 stage = (puzzle + 1);
	u8 cards = PUZZLE02INFO[puzzle].cards;

	// Orientacion
	if (GAMEOPTIONS[0].lefthanded) {
		sprintf(side, "l");
	} else {
		sprintf(side, "r");
	}

	// Seleccion de idioma
	switch (GAMEOPTIONS[0].language) {
		case 0:		// Español
			sprintf(lang, "es");
			break;
		case 1:		// Ingles
			sprintf(lang, "en");
			break;
		default:
			break;
	}

	// Carga los fondos comunes a todos los puzzles
	sprintf(file, "puzz02/bg/stg%02d/p02bg03sub%02d%s", stage, stage, side);
	NF_LoadTiledBg(file, "bg3sub", 256, 256);
	sprintf(file, "puzz02/bg/stg%02d/p02bg03main%02d%s", stage, stage, side);
	NF_LoadTiledBg(file, "bg3main", 256, 256);

	// Carga los fondos y fuentes para el report (comun a todos los puzzles)
	NF_LoadTiledBg("puzz01/bg/p01bg02main", "bg2main", 256, 512);

	// Carga las fuentes
	if (GAMEOPTIONS[0].lefthanded) {
		n = 2;
	} else {
		n = 1;
	}
	NF_LoadTextFont16("font/8x16", "text", 256, 512, n);

	// Carga los graficos de las cartas
	for (n = 0; n < cards; n ++) {
		// Carga los graficos y paletas de las cartas... (10 - (10 + MAX))
		sprintf(file, "puzz02/sprite/stg%02d/p02s%02dc%02d", stage, stage, n);
		NF_LoadSpriteGfx(file, (PUZZLE02_SPRITE + n), 64, 64);
		NF_LoadSpritePal(file, (PUZZLE02_SPRITE + n));
	}

	// Carga los graficos de los botones del menu
	for (n = 0; n < 3; n ++) {
		// Carga los graficos y paletas de los botones del menu (0 - 2)
		sprintf(file, "puzz02/sprite/common/p02mb%d_%s", n, lang);
		NF_LoadSpriteGfx(file, n, 32, 128);
		NF_LoadSpritePal(file, n);
	}

	// Carga los graficos de los marcadores
	for (n = 0; n < 4; n ++) {
		// Carga los graficos y paletas de los marcadores (3 - 6)
		sprintf(file, "puzz02/sprite/stg%02d/p02s%02dmark", stage, stage);
		NF_LoadSpriteGfx(file, (PUZZLE02_MARKSPRITE + n), 32, 32);
		NF_LoadSpritePal(file, (PUZZLE02_MARKSPRITE + n));
	}

	// Si es para zurdos...
	if (GAMEOPTIONS[0].lefthanded) {
		// Rota los fondos del report
		RotateTiledBg("bg2main");
		// Rota los sprites de las cartas
		for (n = 0; n < cards; n ++) {
			Rotate3dSprite((PUZZLE02_SPRITE + n));
		}
		// Rota los sprites de los botones del menu
		for (n = 0; n < 3; n ++) {
			Rotate3dSprite(n);
		}
	}


}



// Funcion CreatePuzzle02();
void CreatePuzzle02(u8 puzzle) {

	// Variables
	u8 n = 0;
	u8 cards = PUZZLE02INFO[puzzle].cards;

	// Inicializa los datos del puzzle
	InitPuzzle02Data(puzzle);

	// Crea los fondos
	NF_CreateTiledBg(0, 3, "bg3sub");	// Fondo pantalla tactil
	NF_CreateTiledBg(1, 3, "bg3main");	// Fondo pantalla superior
	NF_CreateTiledBg(1, 2, "bg2main");	// Pantalla normal

	// Crea la capa de texto
	if (GAMEOPTIONS[0].lefthanded) {
		NF_CreateTextLayer16(1, 1, 2, "text");
	} else {
		NF_CreateTextLayer16(1, 1, 1, "text");
	}
	NF_DefineTextColor(1, 1, 1, 31, 31, 31);
	NF_SetTextColor(1, 1, 1);

	// Habilita el alpha
	REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);
	REG_BLDALPHA_SUB = 0x1F | (0x05 << 8);

	// Ajusta los fondos si es zurdo
	if (GAMEOPTIONS[0].lefthanded) {
		NF_ScrollBg(1, 2, 0, (320 - 192));
		NF_ScrollBg(1, 1, 0, (320 - 192));
	} else {
		NF_ScrollBg(1, 2, 0, 192);
		NF_ScrollBg(1, 1, 0, 192);
	}

	// Transfiere la paletas y graficos de...
	// Las cartas
	for (n = 0; n < cards; n ++) {
		NF_Vram3dSpritePal((n + PUZZLE02_SPRITE), (n + PUZZLE02_SPRITE));
		NF_Vram3dSpriteGfx((n + PUZZLE02_SPRITE), (n + PUZZLE02_SPRITE), false);
	}
	// Los botones del menu
	for (n = 0; n < 3; n ++) {
		NF_Vram3dSpritePal(n, n);
		NF_Vram3dSpriteGfx(n, n, false);
	}
	// Los marcadores
	for (n = 0; n < 4; n ++) {
		NF_VramSpritePal(1, (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n));
		NF_VramSpriteGfx(1, (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n), true);
	}

	// Crea el set de cartas sobre el tablero (0 - 8)
	for (n = 0; n < 9; n ++) {
		NF_Create3dSprite((n + PUZZLE02_SPRITE), (PUZZLE02_SPRITE + (cards - 1)), (PUZZLE02_SPRITE + (cards - 1)), PUZZLE02SPR[n].x, (PUZZLE02SPR[n].y - 8));
	}

	// Crea los botones del menu
	for (n = 0; n < 3; n ++) {
		NF_Create3dSprite(n, n, n, 0, 0);
		NF_Show3dSprite(n, false);
	}

	// Ordenalos por ID
	NF_Sort3dSprites();

	// Crea los marcadores
	for (n = 0; n < 4; n ++) {
		if (GAMEOPTIONS[0].lefthanded) {
			NF_CreateSprite(1, (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n), PUZZLE02INFO[puzzle].mark_x[n][1], PUZZLE02INFO[puzzle].mark_y[n][1]);
			NF_HflipSprite(1, (PUZZLE02_MARKSPRITE + n), true);
			NF_VflipSprite(1, (PUZZLE02_MARKSPRITE + n), true);
		} else {
			NF_CreateSprite(1, (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n), (PUZZLE02_MARKSPRITE + n), PUZZLE02INFO[puzzle].mark_x[n][0], PUZZLE02INFO[puzzle].mark_y[n][0]);
		}
		NF_SpriteLayer(1, (PUZZLE02_MARKSPRITE + n), 3);
		NF_SpriteFrame(1, (PUZZLE02_MARKSPRITE + n), 0);
	}

	// Actualiza el contenido de la pantalla
	NF_Draw3dSprites();
	glFlush(0);
	NF_SpriteOamSet(1);
	swiWaitForVBlank();
	oamUpdate(&oamSub);

}



// Funcion InitPuzzle02Data();
void InitPuzzle02Data(u8 puzzle) {

	// Variables
	s16 x = 0;			// Posicion X
	s16 y = 0;			// Posicion Y
	u8 id = 0;			// nº de carta en el tablero (0 - 8)
	s16 start_x = 0;	// Posiciones iniciales
	s16 start_y = 0;
	s16 add_x = 0;		// Valor de desplazamiento
	s16 add_y = 0;
	u8 n = 0;			// Uso general
	u8 m = 0;

	// Inicializa el random
	srand(time(NULL));

	// Segun la orientacion
	if (GAMEOPTIONS[0].lefthanded) {
		// Zurdo
		start_x = PUZZLE02INFO[puzzle].start_x[1];
		start_y = PUZZLE02INFO[puzzle].start_y[1];
		add_x = 80;
		add_y = -56;
	} else {
		// Diestro
		start_x = PUZZLE02INFO[puzzle].start_x[0];
		start_y = PUZZLE02INFO[puzzle].start_y[0];
		add_x = -80;
		add_y = 56;
	}

	// Inicializa las posiciones y flags de las cartas
	x = start_x;
	for (n = 0; n < 3; n ++) {
		y = start_y;
		for (m = 0; m < 3; m ++) {
			// Asigna los valores
			PUZZLE02SPR[id].x = x;
			PUZZLE02SPR[id].y = y;
			PUZZLE02SPR[id].tap = true;
			id ++;
			// Coordenada la siguiente posicion de columna
			y += add_y;
		}
		// Coordenada la siguiente fila
		x += add_x;
	}

	// Carga la informacion sobre el mazo
	PUZZLE02DECK.cards = PUZZLE02INFO[puzzle].cards;

	// Carga la informacion de los marcadores
	for (n = 0; n < 4; n ++) {
		PUZZLE02DECK.delay[n] = PUZZLE02INFO[puzzle].delay;
		PUZZLE02DECK.frame[n] = 0;
		PUZZLE02DECK.timer[n] = PUZZLE02INFO[puzzle].timer;
	}
	PUZZLE02DECK.delay_end = PUZZLE02INFO[puzzle].delay;
	PUZZLE02DECK.timer_end = PUZZLE02INFO[puzzle].timer;
	PUZZLE02DECK.last_frame = PUZZLE02INFO[puzzle].frames;

}


// Funcion GetPuzzle02SpriteId();
u8 GetPuzzle02SpriteId(void) {

	// Variables
	u8 id = 255;
	u8 n = 0;

	// Obten la ID si se toca una ficha
	for (n = 0; n < 9; n ++) {
		if (
			(TOUCHPAD.pos_x >= PUZZLE02SPR[n].x)
			&&
			(TOUCHPAD.pos_x <= (PUZZLE02SPR[n].x + 64))
			&&
			(TOUCHPAD.pos_y >= PUZZLE02SPR[n].y)
			&&
			(TOUCHPAD.pos_y <= (PUZZLE02SPR[n].y + 48))
			) {
			id = n;
		}
	}

	// Devuelve el valor
	return id;

}



// Funcion PlayPuzzle02();
u8 PlayPuzzle02(u8 puzzle) {

	// Variables
	u8 thread = 0;			// Hilo de ejecucion
	bool loop = true;		// Bucle de ejecucion
	bool touch = false;		// Se esta tocando la pantalla?
	bool click = false;		// Click valido?
	u8 id = 0;				// Id de la carta tocada
	s16 flip = 0;			// Angulo para el efecto FLIP
	s16 zoom = 0;			// Zoom
	u8 sprite = 0;			// Sprite activo
	s16 card = 0;			// Grafico de la carta
	u8 untaped = 0;			// Numero de cartas destapadas
	u8 untap[2];			// Cartas destapadas (pareja actual)
	u8 untap_id[2];			// Id de las cartas destapadas (pareja actual)
	u8 pairs = 0;			// Parejas realizadas
	u8 timer = 0;			// Temporizador de uso general
	u8 result = 0;			// Resultado de la ejecucion del puzzle
	u8 tics = 0;			// Control del tiempo de juego
	u32 time = 0;			// Tiempo de juego
	u32 moves = 0;			// Movimientos realizados
	u8 rounds = PUZZLE02_STARTROUND;			// Rondas completadas con exito (4)

	// Bucle de ejecucion
	while (loop) {

		// Lee el Touchpad y teclado
		ReadKeypad();
		ReadTouchpad();

		// Detecta un click valido
		click = false;
		if (TOUCHPAD.held) {
			if (!touch) {
				click = true;
				touch = true;
			}
		} else {
			touch = false;
		}


		///////////////////////
		// Threads del juego //
		///////////////////////

		switch (thread) {

			//////////////////////////////////////////////////////////////////////////////////////////
			// Crea un nuevo mazo																	//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Calcula un nuevo mazo
			case 0:		// #MAKE_DECK
				// Obten 4 parejas de cartas
				Puzzle02MakeDeck();
				// Inicializa las variables
				untaped = 0;		// Contador de cartas destapadas
				untap[0] = 255;		// Carta A
				untap[2] = 255;		// Carta B
				pairs = 0;			// Parejas realizadas
				// Siguiente thread
				thread ++;
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Espera un click sobre una carta valida												//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Espera a que se pulse sobre una carta valida
			case 1:		// #WAIT_CLICK
				// Si se hace "click"
				if (click) {
					// Obten la ID de la carta tocada
					id = GetPuzzle02SpriteId();
					// Si es una ID valida
					if (id != 255) {
						// Y la carta esta tapeada
						if (PUZZLE02SPR[id].tap) {
							// Calcula el nº de sprite
							sprite = (PUZZLE02_SPRITE + id);
							// Manda el sprite de la carta al frente
							NF_3dSpriteSetDeep(sprite, -512);
							// Prepara los datos para el siguiente thread
							flip = 0;	// Efecto rotacion
							// Suma 1 al contador de movimientos
							moves ++;
							// Salta al siguiente thread
							thread ++;
						}
					}
				}
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Gira la carta clicada desde el reverso a su imagen									//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Efecto "levantar la carta" (reverso/carta)
			case 2:
				// Suma la rotacion
				flip += PUZZLE02_FLIP_SPEED;
				// Limite del efecto
				if (flip >= 128) {
					flip = 128;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + flip);
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			// Cambiar el grafico de la carta (reverso / carta)
			case 3:
				// Borra el Sprite actual
				NF_Delete3dSprite(sprite);
				// Calcula la ID del grafico
				card = PUZZLE02SPR[id].card;
				// Crealo de nuevo con el grafico actualizado
				NF_Create3dSprite(sprite, (PUZZLE02_SPRITE + card), (PUZZLE02_SPRITE + card), PUZZLE02SPR[id].x, (PUZZLE02SPR[id].y - 8));
				// Reordena la cola de Sprites
				NF_Sort3dSprites();
				// Asegurate que esta en primer plano
				NF_3dSpriteSetDeep(sprite, -512);
				// Actualiza la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				// Siguiente thread
				thread ++;
				break;

			// Efecto "posar la carta" (reverso / carta)
			case 4:
				// Continua la rotacion
				flip += PUZZLE02_FLIP_SPEED;
				// Limite del efecto
				if (flip >= 256) {
					flip = 256;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + (256 - flip));
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 256);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Verifica si hay una pareja, si es valida y si se ha encontrado al jocker				//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Al terminar de girar la carta para mostrarla...
			case 5:

				// Manda el sprite de la carta al centro
				NF_3dSpriteSetDeep(sprite, 0);
				// Restaura la rotacion y zoom
				NF_Rotate3dSprite(sprite, 0, 0, 0);
				NF_Scale3dSprite(sprite, 64, 64);
				// Marca la carta como destapada
				PUZZLE02SPR[id].tap = false;
				// Registra la carta
				untap[untaped] = PUZZLE02SPR[id].card;
				untap_id[untaped] = id;

				// Si la carta levantada, no es el jocker...
				if (card != 0) {

					// Suma 1 al numero de cartas levantadas
					untaped ++;

					// Decide que hacer segun la carta levantada
					switch (untaped) {

						// Solo hay una carta destapada?
						case 1:
							// Ve al thread de "espera a que se levante una carta"
							thread = 1;
							break;

						// Si hay dos cartas levantadas, verifica si la pareja es correcta
						case 2:
							if (untap[0] == untap[1]) {
								// Si la pareja es correcta, registralo
								pairs ++;
								// Y salta a thread "espera que se pulse en una carta valida"
								thread = 1;	// #WAIT_CLICK
							} else {
								// Mandalo al thread "Vuelve a cubrir las cartas"
								thread = 6;	// #TAP_TWO
							}
							// Resetea el contador de cartas levantadas
							untaped = 0;
							untap[0] = 255;
							untap[1] = 255;
							break;

						// Nada, error trap
						default:
							break;

					}

				} else {	// Si la carta era el Jocker

					// Resetea el contador
					timer = 0;

					// Salta al thread correspondiente
					thread = 17;	// #JOCKER

				}

				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Si habia dos cartas destapadas y no eran pareja, tapalas								//
			//////////////////////////////////////////////////////////////////////////////////////////

			// No es una pareja valida, vuelve a tapar las dos cartas
			case 6:		// #TAP_TWO
				// Calcula el nº de sprite e ID de carta
				sprite = (PUZZLE02_SPRITE + untap_id[untaped]);
				id = untap_id[untaped];
				// Manda el sprite de la carta al frente
				NF_3dSpriteSetDeep(sprite, -512);
				// Prepara los datos para el siguiente thread
				flip = 0;	// Efecto rotacion
				// Salta al siguiente thread
				thread ++;
				break;

			// Efecto "levantar la carta" (carta / reverso)
			case 7:
				// Suma la rotacion
				flip += PUZZLE02_FLIP_SPEED;
				// Limite del efecto
				if (flip >= 128) {
					flip = 128;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + flip);
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			// Cambiar el grafico de la carta (carta / reverso)
			case 8:
				// Borra el Sprite actual
				NF_Delete3dSprite(sprite);
				// Calcula la ID del grafico
				card = (PUZZLE02DECK.cards - 1);
				// Crealo de nuevo con el grafico actualizado
				NF_Create3dSprite(sprite, (PUZZLE02_SPRITE + card), (PUZZLE02_SPRITE + card), PUZZLE02SPR[id].x, (PUZZLE02SPR[id].y - 8));
				// Reordena la cola de Sprites
				NF_Sort3dSprites();
				// Asegurate que esta en primer plano
				NF_3dSpriteSetDeep(sprite, -512);
				// Actualiza la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				// Siguiente thread
				thread ++;
				break;

			// Efecto "posar la carta" (carta / reverso)
			case 9:
				// Continua la rotacion
				flip += PUZZLE02_FLIP_SPEED;
				// Limite del efecto
				if (flip >= 256) {
					flip = 256;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + (256 - flip));
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 256);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			// Verifica si ya se han tapado las dos cartas
			case 10:
				// Manda el sprite de la carta al centro
				NF_3dSpriteSetDeep(sprite, 0);
				// Restaura la rotacion y zoom
				NF_Rotate3dSprite(sprite, 0, 0, 0);
				NF_Scale3dSprite(sprite, 64, 64);
				// Marca la carta como tapada
				PUZZLE02SPR[id].tap = true;
				// Registra que se ha girado la carta
				untaped ++;
				// Si ya se han tapado las dos cartas...
				if (untaped == 2) {
					// Resetea el contador de cartas levantadas
					untaped = 0;
					untap[0] = 255;
					untap[1] = 255;
					// Vuelve al thread de espera a que se pulse sobre una carta
					thread = 1;	// #WAIT_CLICK
				} else {
					// Ve al thread que tapea la siguienta carta
					thread = 6;	// #TAP_TWO
				}
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Ha salido el jocker, tapea todas las cartas											//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Tapea todas las cartas
			case 11:	// #TAP_ALL
				// Verifica si la carta esta destapeada
				untaped = 0;
				// Mientras no encuentres una carta destapada
				while (untaped == 0) {
					// Si ya esta tapada...
					if (PUZZLE02SPR[id].tap == true) {
						// Salta a la siguiente carta
						id ++;
						// Si era la ultima, sal igualmente
						if (id >= 9) untaped = 1;
					} else {
						// Si esta destapada, indica que debe girarse
						untaped = 1;
					}
				}
				// Si ya has revisado todas las cartas, resetea el tapeo masivo
				if (id >= 9) {
					thread = 16;
				} else {	// Si aun quedan cartas por tapar...
					// Guarda el numero de sprite
					sprite = (PUZZLE02_SPRITE + id);
					// Manda el sprite de la carta al frente
					NF_3dSpriteSetDeep(sprite, -512);
					// Prepara los datos para el siguiente thread
					flip = 0;	// Efecto rotacion
					// Salta al siguiente thread
					thread ++;
				}
				break;

			// Efecto "levantar la carta" (carta / reverso) rapido
			case 12:
				// Suma la rotacion
				flip += (PUZZLE02_FLIP_SPEED << 1);
				// Limite del efecto
				if (flip >= 128) {
					flip = 128;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + flip);
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			// Cambiar el grafico de la carta (carta / reverso) rapido
			case 13:
				// Borra el Sprite actual
				NF_Delete3dSprite(sprite);
				// Calcula la ID del grafico
				card = (PUZZLE02DECK.cards - 1);
				// Crealo de nuevo con el grafico actualizado
				NF_Create3dSprite(sprite, (PUZZLE02_SPRITE + card), (PUZZLE02_SPRITE + card), PUZZLE02SPR[id].x, (PUZZLE02SPR[id].y - 8));
				// Reordena la cola de Sprites
				NF_Sort3dSprites();
				// Asegurate que esta en primer plano
				NF_3dSpriteSetDeep(sprite, -512);
				// Actualiza la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 0);
				NF_Scale3dSprite(sprite, zoom, zoom);
				// Siguiente thread
				thread ++;
				break;

			// Efecto "posar la carta" (carta / reverso) rapido
			case 14:
				// Continua la rotacion
				flip += (PUZZLE02_FLIP_SPEED << 1);
				// Limite del efecto
				if (flip >= 256) {
					flip = 256;
					thread ++;
				}
				// Calcula el ZOOM
				zoom = (64 + (256 - flip));
				// Aplica la rotacion y zoom
				NF_Rotate3dSprite(sprite, flip, flip, 256);
				NF_Scale3dSprite(sprite, zoom, zoom);
				break;

			// Se ha terminado de tapar la carta...
			case 15:
				// Manda el sprite de la carta al centro
				NF_3dSpriteSetDeep(sprite, 0);
				// Restaura la rotacion y zoom
				NF_Rotate3dSprite(sprite, 0, 0, 0);
				NF_Scale3dSprite(sprite, 64, 64);
				// Marca la carta como tapada
				PUZZLE02SPR[id].tap = true;
				// Siguiente ID
				id ++;
				// Si era la ultima ID
				if (id >= 9) {
					// Salta al thread de finalizacion de tapeo
					thread = 16;	// #END_ROUND
				} else {	// Si no...
					// Sigue tapando las cartas restantes
					thread = 11;	// #TAP_ALL
				}
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Es el final de la ronda?																//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Reseteo despues del tapeo masivo
			case 16:	// #END_ROUND
				untaped = 0;		// Contador de cartas destapadas
				untap[0] = 255;		// Carta A
				untap[2] = 255;		// Carta B
				id = 255;			// Id de carta
				// Dependiendo de las parejas completadas
				if (pairs == 4) {
					// Si el jocker era la ultima carta, cambia de mazo para la siguiente ronda
					thread = 0;	// #MAKE_DECK
				} else {
					// Si no lo era, borra el numero de parejas completadas
					pairs = 0;
					// Y vuelve al thread de espera
					thread = 1;	// #WAIT_CLICK
				}
				break;

			//////////////////////////////////////////////////////////////////////////////////////////
			// Si se ha descubierto el jocker, que hacer?											//
			//////////////////////////////////////////////////////////////////////////////////////////

			// Retardo al mostrar el jocker
			case 17:
				timer ++;
				if (timer >= 30) thread ++;
				break;

			// Decide que hacer si se ha tocado el Jocker
			case 18:	// #JOCKER
				// Si es la ultima carta de la ronda...
				if (pairs == 4) {
					rounds ++;
				}
				// Resetea las variables necesarias
				id = 0;
				// Salta al thread correspondiente
				thread = 11;	// #TAP_ALL
				break;

			// Nada, ERROR trap
			default:
				thread = 0;
				break;

		}


		//////////////////////////////////////////////////////////////////////////////////////////
		// Procesos comunes																		//
		//////////////////////////////////////////////////////////////////////////////////////////

		// Si has pulsado la tecla de cancelar
		if ((thread == 1) && AbortKeys()) {
			loop = false;
			result = 1;
		}

		// Si se ha resuelto el puzzle, sal
		if (rounds == 4) {
			loop = false;
			result = 0;
		}

		// Calculo del tiempo
		tics ++;
		if (tics > 59) {
			tics = 0;
			time ++;
		}

		// Actualiza el contador de rondas
		PUZZLE02DECK.rounds = rounds;
		Puzzle02UpdateMarkers();

		// Actualiza el contenido de la pantalla
		NF_Draw3dSprites();
		glFlush(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);

	}

	// Si se ha resuelto el puzzle, calcula los resultados
	if (result == 0) Puzzle02Results(puzzle, time, moves);

	// Devuelve el resultado de la ejecucion
	// 0 - Completado
	// 1 - Abortado
	return result;

}



// Funcion Puzzle02MakeDeck();
void Puzzle02MakeDeck(void) {

	// Variables
	u8 set[4];					// Set de cartas seleccionado
	u8 card[9];					// Guarda las ids de las cartas seleccionas
	u8 id = 0;					// Id, uso general
	u8 good = 0;				// Cartas bien asignadas
	bool duplicated = false;	// Flag de duplicidad
	s16 n = 0;					// Uso general
	s16 z = 0;

	// Inicializa el set de cartas
	for (n = 0; n < 4; n ++) {
		set[n] = 255;
	}

	// Inicializa el mazo
	for (n = 0; n < 9; n ++) {
		card[n] = 255;
	}

	// Selecciona 4 cartas al azar
	id = 0;
	while (id < 4) {
		// Random dentro el mazo
		z = (int)((rand() % (PUZZLE02DECK.cards - 2)) + 1);
		// Verifica si esta duplicada la carta
		duplicated = false;
		for (n = 0; n < 4; n ++) {
			if (set[n] == z) duplicated = true;
		}
		// Si no esta duplicada, registrala y continua
		if (!duplicated) {
			set[id] = z;
			id ++;
		}
	}

	// Carta Jocker
	z = (int)(rand() % 9);
	card[z] = 0;

	// Asigna el resto de cartas
	n = 1;
	id = 0;
	good = 0;
	while (n < 9) {
		// Random Slot
		z = (int)(rand() % 9);
		// Verifica si esta libre
		if (card[z] == 255) {
			card[z] = set[id];	// Asigna la carta al slot
			n ++;				
			good ++;
			if (good == 2) {	// Si ya has asignado una pareja, pasa a la siguiente
				good = 0;
				id ++;
			}
		}
	}

	// Asigna el nuevo mazo
	for (n = 0; n < 9; n ++) {
		PUZZLE02SPR[n].card = card[n];
	}

}



// Funcion Puzzle02Results();
void Puzzle02Results(u8 puzzle, u32 time, u32 moves) {

	// Variables
	u16 points = 0;			// Puntos totales ganados
	u16 time_points = 0;	// Puntos ganados por tiempo
	u16 moves_points = 0;	// Puntos ganados por movimientos
	u16 record_points = 0;	// Puntos por record
	u16 mins = 0;			// Minutos
	u16 secs = 0;			// Segundos
	char text[24];			// Buffer de texto

	// Añade un contador a puzzles resueltos
	SAVEDATA[1].solved[puzzle] ++;

	// Maximo de tiempo
	if (time > 5999) time = 5999;
	// Maximo de movimientos
	if (moves > 999) moves = 999;

	// Calcula los minutos y segundos
	mins = (int)(time / 60);
	secs = (time - (mins * 60));


	//////////////////////////////
	// Calcula las puntuaciones //
	//////////////////////////////

	// Por tiempo	(Por debajo de 2:30 minutos)
	if (time < 150) {	
		time_points = ((150 - time) << 1);
	}

	// Por movimientos	(Por debajo de 100 movimientos)
	if (moves < 100) {
		moves_points = ((100 - moves) << 2);
	}

	// Registra los records
	if (time <= SAVEDATA[1].time[puzzle]) {
		SAVEDATA[1].time[puzzle] = time;
		record_points += 50;
	}
	if (moves <= SAVEDATA[1].moves[puzzle]) {
		SAVEDATA[1].moves[puzzle] = moves;
		record_points += 50;
	}

	// Total de puntos
	points = (100 + time_points + moves_points + record_points);

	// Registra la puntuacion
	SAVEDATA[1].points += points;
	SAVEDATA[1].credits += points;
	SAVEDATA[1].score[puzzle] += points;

	// Guardalo los datos
	WriteSavegame();

	// Genera el report
	NF_ClearTextLayer16(1, 1);

	switch (GAMEOPTIONS[0].language) {

		case 0:		// Castellano
			sprintf(text, "Tiempo: %02d:%02d", mins, secs);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Pasos: %03d", moves);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Puntuacion");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Tiempo:  %03d", time_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Pasos:   %03d", moves_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 12, text);
			break;

		case 1:		// Ingles
			sprintf(text, "Time: %02d:%02d", mins, secs);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 2, text);
			sprintf(text, "Moves: %03d", moves);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 3, text);
			sprintf(text, "Score");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 5, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 6, text);
			sprintf(text, "Puzzle:  100");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 7, text);
			sprintf(text, "Time:    %03d", time_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 8, text);
			sprintf(text, "Moves:   %03d", moves_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 9, text);
			sprintf(text, "Record:  %03d", record_points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 10, text);
			sprintf(text, "--------------");
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 11, text);
			sprintf(text, "Total:   %03d", points);
			NF_WriteText16(1, 1, (12 - (strlen(text) >> 1)), 12, text);
			break;

		default:
			break;

	}

	// Actualiza las capas de texto
	NF_UpdateTextLayers();

}



// Funcion EndPuzzle02();
void EndPuzzle02(u8 end) {

	// Variables
	s16 n = 0;
	s32 y = 0;
	s32 speed = 0;
	
	// Entrada del report, si se ha resuelto el puzzle
	if (end == 0) {
		// Pausa de 1 segundo
		for (n = 0; n < 60; n ++) {
			// Actualiza el contador de rondas
			Puzzle02UpdateMarkers();
			// Actualiza la pantalla
			NF_SpriteOamSet(1);
			swiWaitForVBlank();
			oamUpdate(&oamSub);
		}
		// Muestra el report
		y = (192 << 8);
		while (y > 0) {
			speed = (y >> 3);
			if (speed < 512) speed = 512;
			y -= speed;
			if (y < 0) y = 0;
			if (GAMEOPTIONS[0].lefthanded) {
				NF_ScrollBg(1, 2, 0, (320 - (y >> 8)));
				NF_ScrollBg(1, 1, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(1, 2, 0, (y >> 8));
				NF_ScrollBg(1, 1, 0, (y >> 8));
			}
			// Actualiza el contador de rondas
			Puzzle02UpdateMarkers();
			// Actualiza la pantalla
			NF_SpriteOamSet(1);
			swiWaitForVBlank();
			oamUpdate(&oamSub);
		}
	}

}



// Funcion EndMenuPuzzle02();
u8 EndMenuPuzzle02(u8 end) {

	// Variables
	s16 n = 0;			// Uso general
	bool loop = false;	// Control de bucles
	s32 x[3];			// Posicion de los botones
	s32 in[4];			// Distancia de entrada
	s32 out[4];			// Distancia de salida
	s32 y = 0;			// Vertical de los botones y posicion de la capa de report
	s32 speed = 0;		// Calculo de velocidad
	bool touch = false;	// Control tactil
	u8 option = 0;		// Opcion seleccionada

	// Inicializa las variables
	if (GAMEOPTIONS[0].lefthanded) {
		x[0] = (60 << 8);					// Posicion de los botones
		x[1] = (108 << 8);
		x[2] = (156 << 8);
		in[0] = ((32 << 8) + x[0]);			// Distancia
		in[1] = ((32 << 8) + x[1]);
		in[2] = ((32 << 8) + x[2]);
		in[3] = 0;
		out[0] = ((256 << 8) - x[0]);		// Distancia
		out[1] = ((256 << 8) - x[1]);
		out[2] = ((256 << 8) - x[2]);
		out[3] = (256 << 8);
	} else {
		x[0] = (172 << 8);					// Posicion de los botones
		x[1] = (124 << 8);
		x[2] = (76 << 8);
		in[0] = ((256 << 8) - x[0]);		// Distancia
		in[1] = ((256 << 8) - x[1]);
		in[2] = ((256 << 8) - x[2]);
		in[3] = 0;
		out[0] = ((32 << 8) + x[0]);		// Distancia
		out[1] = ((32 << 8) + x[1]);
		out[2] = ((32 << 8) + x[2]);
		out[3] = (256 << 8);

	}
	y = 32;


	///////////////////////////////
	// Crea los botones del menu //
	///////////////////////////////

	// Posiciona los sprites
	for (n = 0; n < 3; n ++) {
		if (GAMEOPTIONS[0].lefthanded) {
			NF_Move3dSprite(n, (((x[n] - in[n]) >> 8) - 4), y);
		} else {
			NF_Move3dSprite(n, (((x[n] + in[n]) >> 8) - 4), y);
		}
		// Haz el sprite visible
		NF_Show3dSprite(n, true);
	}
	// Actualiza el contenido de las pantallas
	NF_Draw3dSprites();
	glFlush(0);
	NF_SpriteOamSet(1);
	swiWaitForVBlank();
	oamUpdate(&oamSub);


	////////////////////////////
	// Entrada de los botones //
	////////////////////////////

	loop = true;
	while (loop) {
		// Calcula y mueve los botones
		for (n = 2; n >= 0; n --) {
			if ((in[n] > 0) && ((in[(n + 1)] < (48 << 8)))) {
				speed = (in[n] >> 3);
				if (speed < 512) speed = 512;
				in[n] -= speed;
				if (in[n] < 0) in[n] = 0;
				if (GAMEOPTIONS[0].lefthanded) {
					NF_Move3dSprite(n, (((x[n] - in[n]) >> 8) - 4), y);
				} else {
					NF_Move3dSprite(n, (((x[n] + in[n]) >> 8) - 4), y);
				}
			}
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == 0) && (in[1] == 0) && (in[2] == 0)) loop = false;
		// Actualiza el contador de rondas
		Puzzle02UpdateMarkers();
		// Actualiza el contenido de las pantallas
		NF_Draw3dSprites();
		glFlush(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}


	////////////////////////
	// Seleccion del menu //
	////////////////////////

	loop = true;
	while (loop) {
		// Lee el tactil
		ReadTouchpad();
		// Si el lapiz toca la pantalla...
		if (TOUCHPAD.held) {
			// Si es el primer toque
			if (!touch) {
				// Marcalo
				touch = true;
				// Verifica los botones
				for (n = 0; n < 3; n ++) {
					if (
						(TOUCHPAD.pos_x >= (x[n] >> 8))
						&&
						(TOUCHPAD.pos_x <= ((x[n] >> 8) + 24))
						&&
						(TOUCHPAD.pos_y >= y)
						&&
						(TOUCHPAD.pos_y <= (y + 128))
						) {
						// Si has tocado un boton, registralo y sal
						loop = false;
						option = n;
					}
				}
			}
		} else {
			// Si no lo toca, marcalo
			touch = false;
		}
		// Actualiza el contador de rondas
		Puzzle02UpdateMarkers();
		// Actualiza el contenido de las pantallas
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}


	///////////////////////////
	// Salida de los botones //
	///////////////////////////

	loop = true;
	in[3] = out[3];
	while (loop) {
		// Calcula y mueve los botones
		for (n = 2; n >= 0; n --) {
			if ((in[n] < out[n]) && ((in[(n + 1)] > (24 << 8)))) {
				speed = (in[n] >> 3);
				if (speed < 512) speed = 512;
				in[n] += speed;
				if (in[n] > out[n]) in[n] = out[n];
				if (GAMEOPTIONS[0].lefthanded) {
					NF_Move3dSprite(n, (((x[n] + in[n]) >> 8) - 4), y);
				} else {
					NF_Move3dSprite(n, (((x[n] - in[n]) >> 8) - 4), y);
				}
			}
		}
		// Si se ha completado el movimiento, sal
		if ((in[0] == out[0]) && (in[1] == out[1]) && (in[2] == out[2])) loop = false;
		// Actualiza el contador de rondas
		Puzzle02UpdateMarkers();
		// Actualiza el contenido de las pantallas
		NF_Draw3dSprites();
		glFlush(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}
	// Oculta los sprites de los botones
	for (n = 0; n < 3; n ++) {
		NF_Show3dSprite(n, false);
	}


	////////////////////////////////////////
	// Salida del report, si es necesario //
	////////////////////////////////////////

	// Salida del report, si se ha resuelto el puzzle
	if (end == 0) {
		y = 0;
		while (y < (192 << 8)) {
			speed = (y >> 3);
			if (speed < 512) speed = 512;
			y += speed;
			if (y > (192 << 8)) y = (192 << 8);
			if (GAMEOPTIONS[0].lefthanded) {
				NF_ScrollBg(1, 2, 0, (320 - (y >> 8)));
				NF_ScrollBg(1, 1, 0, (320 - (y >> 8)));
			} else {
				NF_ScrollBg(1, 2, 0, (y >> 8));
				NF_ScrollBg(1, 1, 0, (y >> 8));
			}
			// Actualiza el contador de rondas
			Puzzle02UpdateMarkers();
			// Actualiza el contenido de las pantallas
			NF_SpriteOamSet(1);
			swiWaitForVBlank();
			oamUpdate(&oamSub);
		}
	}

	// Devuelve la opcion
	return option;

}



// Funcion RetryPuzzle02();
void RetryPuzzle02(u8 puzzle) {

	// Variables
	u8 n = 0;
	u8 cards = PUZZLE02INFO[puzzle].cards;

	// Fade out
	FadeOut(3, 64);

	// Borra todos los Sprites de las cartas
	for (n = 0; n < 9; n ++) {
		NF_Delete3dSprite((n + PUZZLE02_SPRITE));
	}

	// Inicializa los datos del puzzle
	InitPuzzle02Data(puzzle);

	// Crea de nuevo las cartas
	for (n = 0; n < 9; n ++) {
		NF_Create3dSprite((n + PUZZLE02_SPRITE), (PUZZLE02_SPRITE + (cards - 1)), (PUZZLE02_SPRITE + (cards - 1)), PUZZLE02SPR[n].x, (PUZZLE02SPR[n].y - 8));
	}

	// Actualiza el contador de rondas
	for (n = 0; n < 4; n ++) {
		NF_SpriteFrame(1, (PUZZLE02_MARKSPRITE + n), 0);
	}

	// Actualiza el contenido de la pantalla
	NF_Draw3dSprites();
	glFlush(0);
	NF_SpriteOamSet(1);
	swiWaitForVBlank();
	oamUpdate(&oamSub);

	// Fade In
	FadeIn(3, 64);

}



// Puzzle02UpdateMarkers();
void Puzzle02UpdateMarkers(void) {

	// Variables
	u8 n = 0;
	s8 id = (PUZZLE02DECK.rounds - 1);

	// Si hay alguna ronda completada...
	if (id >= 0) {
		for (n = 0; n <= id; n ++) {
			// Comprueva el temporizador
			if (PUZZLE02DECK.timer[n] <= PUZZLE02DECK.timer_end) {
				PUZZLE02DECK.timer[n] ++;
			}
			// Si se ha disparado el temporizador
			if (PUZZLE02DECK.timer[n] > PUZZLE02DECK.timer_end) {
				// Retardo entre frames
				PUZZLE02DECK.delay[n] ++;
				// Si se ha superado el tiempo de retardo
				if (PUZZLE02DECK.delay[n] >= PUZZLE02DECK.delay_end) {
					PUZZLE02DECK.delay[n] = 0;
					PUZZLE02DECK.frame[n] ++;
					// Si se ha superado el ultimo frame
					if (PUZZLE02DECK.frame[n] > PUZZLE02DECK.last_frame) {
						PUZZLE02DECK.frame[n] = 1;
						PUZZLE02DECK.timer[n] = 0;
					}
					// Actualiza el frame
					NF_SpriteFrame(1, (PUZZLE02_MARKSPRITE + n), PUZZLE02DECK.frame[n]);
				}
			}
		}
	}

}
