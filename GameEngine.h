#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_
#include "stdint.h"

// define
#define STATIONARY 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t motion;
	const unsigned short* bmp;
	// TODO - include prev_x, prev_y, erase
} sprite;

// public functions
// requires ST7735 to be initialized
void GameEngine_Init();
void GameEngine_update();
void GameEngine_redraw();
void GameEngine_updatePlayerMotion(uint8_t dir);

#endif