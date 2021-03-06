#ifndef _GRAPHICS_ENGINE_H_
#define _GRAPHICS_ENGINE_H_
#include "stdint.h"
#include "TopLevelState.h"

#define VULN_FLASH 20
typedef struct spriteStruct{
	uint8_t x;
	uint8_t y;
	uint8_t motion;
	// four bitmaps for up, down, left, right
	const unsigned short* bmp[4];
	// TODO - include prev_x, prev_y, erase
	uint8_t width;
	uint8_t height;
	uint8_t scheduled_motion;
	void (*update_motion)(struct spriteStruct*);
	uint8_t erase_x;
	uint8_t erase_y;
	void (*update_position) (struct spriteStruct*);
	uint8_t need_redraw;
	uint8_t in_motion;
	uint8_t code;
	uint8_t stored_code;
	uint8_t vulnerable;
	const unsigned short* vuln_bmp;
	uint8_t vuln_count;
	uint8_t eyes;
	const unsigned short* eyes_bmp;
	
} sprite;
#define NUM_SPRITES 5
#define BOARD_SIZE_LR 19
#define BOARD_SIZE_UD 22

#define EMPTY 0
#define PACMAN 1
#define GHOST 2
#define DOT 3
#define BIGDOT 4
#define WALL 5
#define FRUIT 6
#define PACMAN_INITIAL_X 1
#define PACMAN_INITIAL_Y BOARD_SIZE_UD - 2
extern sprite* sprites[NUM_SPRITES];
extern sprite p;
extern sprite rg;
void ind_to_pix(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix);
void drawCommon(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix);
void drawSprite(sprite* s);
void GraphicsEngine_initBoard(void);
void GraphicsEngine_drawBoard(void);
void GraphicsEngine_drawScore(void);
void GraphicsEngine_drawTitle(void);
void GraphicsEngine_drawCursor(uint8_t);
extern uint8_t board[BOARD_SIZE_UD][BOARD_SIZE_LR];
#endif

