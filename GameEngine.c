#include "GameEngine.h"
#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

TopLevelState InGame = {
	&GameEngine_updateState,
	&GameEngine_upPressed,
	&GameEngine_downPressed,
	&GameEngine_leftPressed,
	&GameEngine_rightPressed,
	&GameEngine_startPressed,
	&GameEngine_playSound,
	&GameEngine_drawInitial,
};
typedef struct {
	int8_t hor;
	int8_t vert;
} coord;
coord directions[4] = {
	{0, 1},			// UP
	{0, -1},		// DOWN
	{-1, 0},		// LEFT
	{1, 0}			// RIGHT
};
#define EMPTY 0
#define PACMAN 1
#define GHOST 2
#define DOT 3
#define BIGDOT 4
#define WALL 5
#define FRUIT 6
#define BOARD_START_X 8
#define BOARD_END_Y 148
#define BOARD_START_Y BOARD_END_Y - BOARD_SIZE_UD * SQUARE_HEIGHT
#define BOARD_END_X BOARD_START_X + BOARD_SIZE_LR * SQUARE_WIDTH
#define BOARD_SIZE_LR 19
#define BOARD_SIZE_LR_PIX 128
#define BOARD_SIZE_UD 22
#define BOARD_SIZE_UD_PIX 128
#define SQUARE_HEIGHT 6
#define SQUARE_WIDTH 6
const unsigned short pacman[] = {
 0x0000, 0x0208, 0x0471, 0x02EB, 0x0000, 0x0228, 0x07FF, 0x075D, 0x07FF, 0x0228, 0x0471, 0x075D, 0x0679, 0x0000, 0x0000, 0x0124,
 0x07FF, 0x077D, 0x07FF, 0x01C7, 0x0000, 0x00A2, 0x030C, 0x0165, 0x0000,

};
uint8_t board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{WALL, GHOST, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, EMPTY, WALL, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, WALL, EMPTY, WALL},
{WALL, EMPTY, WALL, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, WALL, EMPTY, WALL},
{WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, EMPTY, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, EMPTY, WALL},
{WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL},
{WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, EMPTY, WALL, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, EMPTY, WALL, WALL, EMPTY, WALL},
{WALL, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, WALL},
{WALL, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, WALL},
{WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, WALL},
{WALL, PACMAN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};
const unsigned short r_ghost[] = {
 0x0008, 0x0011, 0x0010, 0x0011, 0x0008, 0x000F, 0x213E, 0x001C, 0x213E, 0x000F, 0x18D0, 0xEF57, 0x003D, 0xEF57, 0x10B0, 0x0003,
 0x0012, 0x0010, 0x0012, 0x0003,

};




sprite p = {1, 20, RIGHT, {pacman, pacman, pacman, pacman}, 5, 5};
sprite rg = {1, 1, RIGHT, {r_ghost, r_ghost, r_ghost, r_ghost}, 5, 4};

// private helper functions
void ind_to_pix(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix) {
	*x_pix = BOARD_START_X + SQUARE_WIDTH * (x_ind);
	*y_pix = BOARD_START_Y + SQUARE_HEIGHT * (y_ind );
}
void drawCommon(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix){
	ind_to_pix(x_ind, y_ind, x_pix, y_pix);
}

void drawSprite(sprite* s){
	uint8_t x_pix, y_pix;
	drawCommon(s->x, s->y, &x_pix, &y_pix);
	ST7735_DrawBitmap(x_pix - 2, y_pix + 6, s->bmp[s->motion], s->width, s->height);
}
// public functions
void GameEngine_Init(){
	// set initial position, number of ghosts, dots
	uint8_t i, j, x_pix, y_pix;
	//ST7735_DrawBitmap(BOARD_START_X, BOARD_END_Y, pacmanmap_96, BOARD_SIZE_LR_PIX, BOARD_SIZE_UD_PIX);
	for (i = 0; i < BOARD_SIZE_UD; ++i){
		for (j = 0; j < BOARD_SIZE_LR; ++j){
			if (board[i][j] == EMPTY){
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix + 4, ST7735_WHITE);
			} else if (board[i][j] == PACMAN) { 
				//drawSprite(p);
				/*ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawBitmap(x_pix - 2, y_pix + 5, pacman, p.width, p.height);*/
			} else if (board[i][j] == GHOST) {
				//ind_to_pix(j, i, &x_pix, &y_pix);
				//ST7735_DrawBitmap(x_pix - 2, y_pix + 5, r_ghost, rg.width, rg.height);
			} else if (board[i][j] == WALL) {
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_FillRect(x_pix -2, y_pix + 2, SQUARE_WIDTH, SQUARE_HEIGHT, ST7735_BLUE);
			}
		}
		drawSprite(&p);
		drawSprite(&rg);
	}
}

void GameEngine_updateState(){
	// TODO - iterate over sprite array, update matrix, check collisions, update score
	// TODO - redraw sprites that move.
}
void GameEngine_upPressed(){
	p.motion = UP;
}
void GameEngine_downPressed(){
	p.motion = DOWN;
}
void GameEngine_rightPressed(){
	p.motion = RIGHT;
}
void GameEngine_leftPressed(){
	p.motion = LEFT;
}
void GameEngine_startPressed(){
	ActiveState_set(&Paused);
}
void GameEngine_playSound(){
	// TODO - have at least one active sound wave to step through
}
void GameEngine_drawInitial(){
	// TODO - full board redraw
	// set initial position, number of ghosts, dots
	uint8_t i, j, x_pix, y_pix;
	
	//We need to first clear the screen
	ST7735_FillScreen(0);
	
	//ST7735_DrawBitmap(BOARD_START_X, BOARD_END_Y, pacmanmap_96, BOARD_SIZE_LR_PIX, BOARD_SIZE_UD_PIX);
	for (i = 0; i < BOARD_SIZE_UD; ++i){
		for (j = 0; j < BOARD_SIZE_LR; ++j){
			if (board[i][j] == EMPTY){
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix + 4, ST7735_WHITE);
			} else if (board[i][j] == PACMAN) { 
				//do nothing
			} else if (board[i][j] == GHOST) {
				//do nothing
			} else if (board[i][j] == DOT) { 
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix, ST7735_WHITE);
			} else if (board[i][j] == BIGDOT) {
				//We need to figure out the best way to draw a big dot. For now, I will leave it as a small dot
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix, ST7735_WHITE);
			} else if (board[i][j] == WALL) {
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_FillRect(x_pix -2, y_pix + 2, SQUARE_WIDTH, SQUARE_HEIGHT, ST7735_BLUE);
			} else if (board[i][j] == FRUIT) {
				//probably do nothing. we may just draw the fruit along with Pacman and the ghosts outside of this loops
			}
			//TODO - small/large blobs, fruits
		}
		drawSprite(&p);
		drawSprite(&rg);
	}
}

void GameEngine_update(){
	// update pacman's location
	// check collision against dots, make note of which to undraw
	// check collision against ghosts
	
}
void GameEngine_redraw(){
	// NOT A FULL REDRAW
	// update() should have been called, populating global variables that need to be redrawn
	
}
