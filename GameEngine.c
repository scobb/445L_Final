#include "GameEngine.h"
#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"
#include "Heartbeat.h"
#include "stdlib.h"
#include "WavReader.h"
#include "ScoreEngine.h"

#define VULN_TIMEOUT 30
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
coord directions[4] = {
	{0, -1},			// UP
	{0, 1},				// DOWN
	{-1, 0},			// LEFT
	{1, 0}				// RIGHT
};
uint8_t ghosts_vulnerable = FALSE;

// public functions
void GameEngine_Init(){
	GraphicsEngine_drawInitBoard();
	ActiveState_set(&InGame);
	srand(0);
}

void GameEngine_updateState(){
	uint8_t i;
	GraphicsEngine_drawScore();
	if (Heartbeat_count == 0) { 
		// update all sprite positions
		for (i = 0; i < NUM_SPRITES; ++i){
			sprites[i]->update_position(sprites[i]);
			sprites[i]->update_motion(sprites[i]);
		}
	}
	for (i = 0; i < NUM_SPRITES; ++i){
		if (sprites[i]->need_redraw) {
			drawSprite(sprites[i]);
		}
	}
}
uint8_t isValidDirection(sprite* this, uint8_t dir){
	
	uint8_t y_ind = this->y + directions[dir].vert;
	uint8_t x_ind = this->x + directions[dir].hor;
	if (this->in_motion){
		y_ind += directions[this->motion].vert;
		x_ind += directions[this->motion].hor;
	}
	return board[y_ind][x_ind] != WALL;
}
void scheduleIfValidDirection(uint8_t dir) {
	if (isValidDirection(&p, dir)){
		p.scheduled_motion = dir;
	}
}
void GameEngine_upPressed(){
	scheduleIfValidDirection(UP);
}
void GameEngine_downPressed(){
	scheduleIfValidDirection(DOWN);
}
void GameEngine_rightPressed(){
	scheduleIfValidDirection(RIGHT);
}
void GameEngine_leftPressed(){
	scheduleIfValidDirection(LEFT);
}
void GameEngine_startPressed(){
	ActiveState_set(&Paused);
}
extern uint8_t needMore;
void GameEngine_playSound(){
	// TODO - have at least one active sound wave to step through
	music_play("waka.wav");
	if(needMore){
		load_more();
	}
}
void GameEngine_drawInitial(){
	// TODO - full board redraw
	
}
void GameEngine_pacmanUpdateMotion(sprite* this) {
	this->motion = this->scheduled_motion;
}
void GameEngine_ghostUpdateMotion(sprite* this) {
	uint8_t i;
	// hack
	this->in_motion = FALSE;
	if (!isValidDirection(this, this->motion)){
		while (1){
			i = rand() % 4;
			if (isValidDirection(this, i)){
				this->motion = i;
				break;
			}
		}
	}
	this->in_motion = TRUE;
}
void GameEngine_updatePositionCommon(sprite* this) {
	// replace what was there before
	board[this->y][this->x] = this->stored_code;
	this->erase_x = this->x;
	this->erase_y = this->y;
	// first check if the way we're going will put us in the wall
	if (board[this->y + directions[this->motion].vert][this->x + directions[this->motion].hor] != WALL){
		// update the sprite
		if (this->in_motion) {
			this->x += directions[this->motion].hor;
			this->y += directions[this->motion].vert;
			// save what was there--we might use it
			this->stored_code = board[this->y][this->x];
		}
		this->need_redraw = TRUE;
		
		// check again, so we don't have smooth motion into the wall
		if (board[this->y + directions[this->motion].vert][this->x + directions[this->motion].hor] != WALL){
			this->in_motion = TRUE;
		} else {
			this->in_motion = FALSE;
		}
	} else {
		this->need_redraw = FALSE;
		this->in_motion = FALSE;
	}
}
void bigDotEaten(void){
	uint8_t i;
	for (i = 1; i < NUM_SPRITES; ++i){
		sprites[i]->vulnerable = TRUE;
		sprites[i]->vuln_count = 0;
	}
}
void GameEngine_pacmanUpdatePosition(sprite* this) {
	GameEngine_updatePositionCommon(this);
	
	// TODO - check collision, kill pacman, update score, update board
	switch (this->stored_code){
		case DOT : {
			// increment score
			ScoreEngine_update(DOT);
			
		} break;
		case BIGDOT : {
			// increment score
			// change state of ghosts
			ScoreEngine_update(BIGDOT);
			bigDotEaten();
			
		} break;
		case GHOST : {
			//ST7735_SetCursor(0,0);
			//printf("DEAD %u, %u!", this->x, this->y);
			
		} break;
	}
	// update the matrix
	board[this->y][this->x] = this->code;
	// pacman eats what was there
	this->stored_code = EMPTY;
}
void GameEngine_ghostUpdatePosition(sprite* this) {
	if (this->vulnerable){
		this->vuln_count++;
		//ST7735_SetCursor(0,0);
		//printf("vuln_count %u", this->vuln_count);
		if (this->vuln_count >= VULN_TIMEOUT){
			this->vulnerable = FALSE;
		}
	}
	GameEngine_updatePositionCommon(this);
	// TODO - check collision, kill pacman
	if (board[this->y][this->x] == PACMAN) {
		// change state
		//ST7735_SetCursor(0,0);
		//printf("GHOST SAYS DEAD!");
	}
	
	// update the matrix
	board[this->y][this->x] = this->code;
}

