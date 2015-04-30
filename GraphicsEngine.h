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
void GraphicsEngine_drawInitial(TopLevelState* state);
void GraphicsEngine_redraw(TopLevelState* state);

#endif