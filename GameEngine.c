#include "GameEngine.h"
#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"
#include "Heartbeat.h"
#include "stdlib.h"

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
	{0, 1},		// DOWN
	{-1, 0},		// LEFT
	{1, 0}			// RIGHT
};

// public functions
void GameEngine_Init(){
	//TODO: initialize the game engine
	GraphicsEngine_drawInitBoard();
	ActiveState_set(&InGame);
	srand(0);
}

void GameEngine_updateState(){
	// TODO - iterate over sprite array, update matrix, check collisions, update score
	// TODO - redraw sprites that move.
	// update pacman's location
	// check collision against dots, make note of which to undraw
	// check collision against ghosts
	uint8_t i;
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
void GameEngine_upPressed(){
	//Check to make sure that this move is valid
	p.scheduled_motion = UP;
}
void GameEngine_downPressed(){
	p.scheduled_motion = DOWN;
}
void GameEngine_rightPressed(){
	p.scheduled_motion = RIGHT;
}
void GameEngine_leftPressed(){
	p.scheduled_motion = LEFT;
}
void GameEngine_startPressed(){
	ActiveState_set(&Paused);
}
void GameEngine_playSound(){
	// TODO - have at least one active sound wave to step through
}
void GameEngine_drawInitial(){
	// TODO - full board redraw
	
}
void GameEngine_pacmanUpdateMotion(sprite* this) {
	this->motion = this->scheduled_motion;
}
void GameEngine_ghostUpdateMotion(sprite* this) {
	static uint8_t num = 0;
	//this->motion = rand() % 4;
	this->motion = DOWN;
}
void GameEngine_updatePositionCommon(sprite* this) {
	this->erase_x = this->x;
	this->erase_y = this->y;
	if (board[this->y + directions[this->motion].vert][this->x + directions[this->motion].hor] != WALL){
		this->x += directions[this->motion].hor;
		this->y += directions[this->motion].vert;
		this->need_redraw = TRUE;
	} else {
		this->need_redraw = FALSE;
	}
}
void GameEngine_pacmanUpdatePosition(sprite* this) {
	GameEngine_updatePositionCommon(this);
	// TODO - check collision, kill pacman, update score, update board
	switch (board[this->y][this->x]){
		case EMPTY : {
			
		} break;
		case DOT : {
			
		} break;
		case BIGDOT : {
			
		} break;
	}
}
void GameEngine_ghostUpdatePosition(sprite* this) {
	GameEngine_updatePositionCommon(this);
	// TODO - check collision, kill pacman
}

