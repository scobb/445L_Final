#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "Heartbeat.h"

#define BOARD_START_X 8
#define BOARD_END_Y 148
#define BOARD_START_Y BOARD_END_Y - BOARD_SIZE_UD * SQUARE_HEIGHT
#define BOARD_END_X BOARD_START_X + BOARD_SIZE_LR * SQUARE_WIDTH
#define BOARD_SIZE_LR_PIX 128
#define BOARD_SIZE_UD_PIX 128
#define SQUARE_HEIGHT 6
#define SQUARE_WIDTH 6
#define FILL_RECT_OFFSET_X -2
#define FILL_RECT_OFFSET_Y 2
#define FILL_RECT_MOTION_OFFSET_X -1
#define FILL_RECT_MOTION_OFFSET_Y -1
#define DRAW_BITMAP_OFFSET_X -2
#define DRAW_BITMAP_OFFSET_Y 6
extern coord directions[4];
const unsigned short pacman_r[] = {
 0x0000, 0x0208, 0x0471, 0x02EB, 0x0000, 0x0228, 0x07FF, 0x075D, 0x07FF, 0x0228, 0x0471, 0x075D, 0x0679, 0x0000, 0x0000, 0x0124,
 0x07FF, 0x077D, 0x07FF, 0x01C7, 0x0000, 0x00A2, 0x030C, 0x0165, 0x0000,
};

const unsigned short pacman_l[] = {
 0x0000, 0x03AE, 0x0575, 0x0228, 0x0000, 0x034D, 0x06FB, 0x075D, 0x079E, 0x0269, 0x0000, 0x0020, 0x04F3, 0x075D, 0x0555, 0x034D,
 0x06FB, 0x075D, 0x079E, 0x0269, 0x0000, 0x03AE, 0x0575, 0x0228, 0x0000,
};

const unsigned short pacman_d[] = {
 0x0000, 0x01C7, 0x0000, 0x01C7, 0x0000, 0x02AA, 0x06FB, 0x0000, 0x06FB, 0x02AA, 0x0575, 0x075D, 0x0430, 0x075D, 0x0575, 0x028A,
 0x079E, 0x077D, 0x079E, 0x028A, 0x0000, 0x028A, 0x0575, 0x028A, 0x0000,
};

const unsigned short pacman_u[] = {
 0x0000, 0x0186, 0x03CF, 0x0082, 0x0000, 0x0249, 0x07BE, 0x079E, 0x07BE, 0x0082, 0x05B6, 0x073C, 0x0430, 0x079E, 0x03CF, 0x028A,
 0x0679, 0x0000, 0x07DF, 0x00A2, 0x0000, 0x0082, 0x0000, 0x0082, 0x0000,
};

const unsigned short pacman_c[] = {
 0x0000, 0x028A, 0x0575, 0x028A, 0x0000, 0x028A, 0x079E, 0x071C, 0x079E, 0x028A, 0x0575, 0x071C, 0x06DB, 0x071C, 0x0575, 0x028A,
 0x079E, 0x071C, 0x079E, 0x028A, 0x0000, 0x028A, 0x0575, 0x028A, 0x0000,
};

const unsigned short apple[] = {
 0x0002, 0x0016, 0x0007, 0x0015, 0x0000, 0x0015, 0x001C, 0x001D, 0x001D, 0x000E, 0x631C, 0x001B, 0x001C, 0x001C, 0x0015, 0x0816,
 0x39DC, 0x0156, 0x001F, 0x000E, 0x0140, 0x0620, 0x0400, 0x0007, 0x0000,
};

const unsigned short cherry[] = {
 0x000A, 0x0016, 0x0001, 0x0016, 0x000A, 0x2976, 0x001F, 0x000D, 0x39BF, 0x0016, 0x0000, 0x0199, 0x0000, 0x0116, 0x0000, 0x0000,
 0x0000, 0x01D6, 0x014F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0192, 0x0043,
};

const unsigned short blue_ghost[] = {
 0x90A1, 0xC8E8, 0xC0C8, 0xC8E7, 0x90A2, 0xC0C2, 0xF929, 0xF92C, 0xF92A, 0xC0C2, 0xC0C1, 0xF93B, 0xF922, 0xF93B, 0xC0C1, 0x8081,
 0xF927, 0xF923, 0xF927, 0x8081,
};

const unsigned short orange_ghost[] = {
 0x014F, 0x01B5, 0x01B4, 0x01B4, 0x01B4, 0x01B4, 0x0ABD, 0x023C, 0x0A9C, 0x025C, 0x3B15, 0xF6B3, 0x027C, 0xD614, 0x84FB, 0x010D,
 0x12FD, 0x023B, 0x12DC, 0x01D4,
};

const unsigned short light_blue_ghost[] = {
 0x7BE0, 0xAD60, 0xAD40, 0xAD60, 0x7BE0, 0xA520, 0xEF45, 0xE700, 0xEF45, 0xA520, 0xB589, 0xAD5F, 0xEF40, 0xAD5F, 0xB589, 0x4A60,
 0xBDC0, 0xAD40, 0xBDC0, 0x4A60,
};

const unsigned short purple_ghost[] = {
 0x9092, 0xC8D9, 0xC0D8, 0xC8D9, 0x9092, 0xC0F8, 0xFA5F, 0xF8FF, 0xFA5F, 0xC0F8, 0xC2D8, 0x9F73, 0xF8DF, 0x9F73, 0xC2D8, 0x582B,
 0xD85B, 0xC0D8, 0xD85B, 0x582B,
};

const unsigned short r_ghost[] = {
 0x0008, 0x0011, 0x0010, 0x0011, 0x0008, 0x000F, 0x213E, 0x001C, 0x213E, 0x000F, 0x18D0, 0xEF57, 0x003D, 0xEF57, 0x10B0, 0x0003,
 0x0012, 0x0010, 0x0012, 0x0003,
};

const unsigned short ghost_eyes[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x2945, 0x0000, 0x2945, 0x0020, 0x4A49, 0xFFDF, 0x0000, 0xFFDF, 0x4A49, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000,
};

const uint8_t init_board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{WALL, GHOST, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL, WALL, WALL, WALL, WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, WALL},
{WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL},
{WALL, PACMAN, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};

uint8_t board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{WALL, GHOST, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL, WALL, WALL, WALL, WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, WALL},
{WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL},
{WALL, PACMAN, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};


sprite p = {1, BOARD_SIZE_UD - 2, RIGHT, {pacman_u, pacman_d, pacman_l, pacman_r}, 5, 5, RIGHT, 
						&GameEngine_pacmanUpdateMotion,1, BOARD_SIZE_UD - 2, &GameEngine_pacmanUpdatePosition, TRUE, TRUE, PACMAN, EMPTY};
sprite rg = {1, 1, RIGHT, {r_ghost, r_ghost, r_ghost, r_ghost}, 5, 4, RIGHT,
						&GameEngine_ghostUpdateMotion,1, 1,&GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT};
sprite bg = {BOARD_SIZE_LR - 2, 1, RIGHT,
						 {blue_ghost, blue_ghost, blue_ghost, blue_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, 1, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT};
sprite og = {BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, RIGHT, {orange_ghost, orange_ghost, orange_ghost, orange_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT};
sprite pg = {BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, RIGHT, {purple_ghost, purple_ghost, purple_ghost, purple_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT};
sprite* sprites[NUM_SPRITES] = {&p, &rg, &bg, &og, &pg};
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

void drawDot(uint8_t x, uint8_t y){
	uint8_t x_pix, y_pix;
	ind_to_pix(x, y, &x_pix, &y_pix);
	ST7735_DrawPixel(x_pix, y_pix + 4, ST7735_WHITE);
}
void ind_to_pix(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix) {
	*x_pix = BOARD_START_X + SQUARE_WIDTH * (x_ind);
	*y_pix = BOARD_START_Y + SQUARE_HEIGHT * (y_ind );
}

void drawCommon(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix){
	ind_to_pix(x_ind, y_ind, x_pix, y_pix);
}

void drawSprite(sprite* s){
	uint8_t erase_x_pix, erase_y_pix, x_pix, y_pix;
	drawCommon(s->erase_x, s->erase_y, &erase_x_pix, &erase_y_pix);
	drawCommon(s->x, s->y, &x_pix, &y_pix);
	// draw lower layer
	if (board[s->erase_y][s->erase_x] == DOT){
		drawDot(s->erase_x, s->erase_y);
	}
	// erase
	ST7735_FillRect(erase_x_pix + FILL_RECT_OFFSET_X + Heartbeat_count * (x_pix - erase_x_pix) / 5,		
								  erase_y_pix + FILL_RECT_OFFSET_Y + Heartbeat_count * (y_pix - erase_y_pix) / 5,	
									s->width,			
									s->height + 1,		
									0);						// debug - WHITE. TODO - replace with black
	
	// draw
	ST7735_DrawBitmap(x_pix + DRAW_BITMAP_OFFSET_X + s->in_motion * Heartbeat_count * directions[s->motion].hor,
										y_pix + DRAW_BITMAP_OFFSET_Y + s->in_motion * Heartbeat_count * directions[s->motion].vert,
										s->bmp[s->motion], s->width, s->height);
}
void GraphicsEngine_drawInitBoard(){
	// set initial position, number of ghosts, dots
	uint8_t i, j, x_pix, y_pix;
	
	//We need to first clear the screen
	ST7735_FillScreen(0);

	for (i = 0; i < BOARD_SIZE_UD; ++i){
		for (j = 0; j < BOARD_SIZE_LR; ++j){
			if (init_board[i][j] == EMPTY){
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix + 4, ST7735_WHITE);
			} else if (init_board[i][j] == PACMAN) { 
				//do nothing
			} else if (init_board[i][j] == GHOST) {
				//do nothing
			} else if (init_board[i][j] == DOT) { 
				drawDot(j, i);
			} else if (init_board[i][j] == BIGDOT) {
				//We need to figure out the best way to draw a big dot. For now, I will leave it as a small dot
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_DrawPixel(x_pix, y_pix, ST7735_WHITE);
			} else if (init_board[i][j] == WALL) {
				ind_to_pix(j, i, &x_pix, &y_pix);
				ST7735_FillRect(x_pix -2, y_pix + 2, SQUARE_WIDTH, SQUARE_HEIGHT, ST7735_BLUE);
			} else if (init_board[i][j] == FRUIT) {
				//probably do nothing. we may just draw the fruit along with Pacman and the ghosts outside of this loops
			}
			//TODO - small/large blobs, fruits
		}
	}
	for (i = 0; i < NUM_SPRITES; ++i){
		drawSprite(sprites[i]);
	}
}
void GraphicsEngine_drawBoard(){
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
				drawDot(j, i);
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
		}
	}
}
