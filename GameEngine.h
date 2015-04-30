#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_
#include "stdint.h"
#include "TopLevelState.h"
// definitions
#define STATIONARY -1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

extern TopLevelState InGame;
void GameEngine_updateState();
void GameEngine_upPressed();
void GameEngine_downPressed();
void GameEngine_rightPressed();
void GameEngine_leftPressed();
void GameEngine_startPressed();
void GameEngine_playSound();
void GameEngine_drawInitial();
typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t motion;
	// four bitmaps for up, down, left, right
	const unsigned short* bmp[4];
	// TODO - include prev_x, prev_y, erase
	uint8_t width;
	uint8_t height;
} sprite;

// public functions
// requires ST7735 to be initialized
void GameEngine_Init();
void GameEngine_update();
void GameEngine_redraw();
void GameEngine_updatePlayerMotion(uint8_t dir);

#endif