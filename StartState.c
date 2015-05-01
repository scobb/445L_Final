#include "StartState.h"
#include "stdint.h"
#define TOP 1
#define BOTTOM 0
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif
uint8_t cursorLocation = TOP;
uint8_t needRedraw = FALSE;
TopLevelState Start =  {
	&StartState_updateState,
	&StartState_upPressed,
	&StartState_downPressed,
	0,
	0,
	&StartState_startPressed,
	&StartState_playSound,
	&StartState_drawInitial,
};
void StartState_playSound(void) {
	// TODO - a song?
}
void StartState_startPressed(void){
	// TODO - check the location of the cursor, forward to appropriate new state
}
void StartState_upPressed(void){
	cursorLocation ^= 1;
	needRedraw = TRUE;
}
void StartState_downPressed(void){
	cursorLocation ^= 1;
	needRedraw = TRUE;
}
void StartState_drawInitial(void){
	// TODO - draw title screen
	// TODO - draw cursor
}
void StartState_updateState(void){
	if (needRedraw){
		// TODO - erase opposite cursor position, draw current cursor position
	}
}