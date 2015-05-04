#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_
#include "stdint.h"
#include "TopLevelState.h"
#include "GraphicsEngine.h"

// definitions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define STATIONARY 4

#define NUM_SPRITES 5
// data structures
typedef struct {
	int8_t hor;
	int8_t vert;
} coord;

// variables
extern TopLevelState InGame;
extern uint8_t eat_ghost;

// interface methods
void GameEngine_updateState(void);
void GameEngine_upPressed(void);
void GameEngine_downPressed(void);
void GameEngine_rightPressed(void);
void GameEngine_leftPressed(void);
void GameEngine_startPressed(void);
void GameEngine_playSound(void);
void GameEngine_drawInitial(void);


// public functions
// requires ST7735 to be initialized
void GameEngine_Init(void);
void GameEngine_update(void);
void GameEngine_redraw(void);
void GameEngine_updatePlayerMotion(uint8_t dir);
void GameEngine_pacmanUpdateMotion(sprite* s);
void GameEngine_pacmanUpdatePosition(sprite* s);
void GameEngine_ghostUpdateMotion(sprite* s);
void GameEngine_ghostUpdatePosition(sprite* s);

#endif
