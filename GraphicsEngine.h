#ifndef _GRAPHICS_ENGINE_H_
#define _GRAPHICS_ENGINE_H_
#include "stdint.h"
#include "TopLevelState.h"

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

extern sprite p;
extern sprite rg;
void ind_to_pix(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix);
void drawCommon(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix);
void drawSprite(sprite* s);
void GraphicsEngine_drawSprite(sprite* character);
void GraphicsEngine_drawInitBoard();
void GraphicsEngine_drawBoard();

#endif

