#include "GameEngine.h"
#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"
#include "Heartbeat.h"

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

// public functions
void GameEngine_Init(){
	//TODO: initialize the game engine
	GraphicsEngine_drawInitBoard();
	ActiveState_set(&InGame);
}

void GameEngine_updateState(){
	// TODO - iterate over sprite array, update matrix, check collisions, update score
	// TODO - redraw sprites that move.
	if (Heartbeat_count == 0) rg.x++;
	drawSprite(&rg);
}
void GameEngine_upPressed(){
	//Check to make sure that this move is valid
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
