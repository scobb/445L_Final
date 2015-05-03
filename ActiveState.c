#include "ActiveState.h"
#include "stdint.h"
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
uint8_t needInitialDraw = FALSE;
TopLevelState* activeState;
/*
	void (*update_state)(void);
	void (*up_pressed)(void);
	void (*down_pressed)(void);
	void (*left_pressed)(void);
	void (*right_pressed)(void);
	void (*start_pressed)(void);
	void (*play_sound)(void);
	void (*draw_initial)(void);
*/
void ActiveState_updateState(){
	if (activeState && activeState->update_state){
		activeState->update_state();
	}
}
void ActiveState_upPressed(){
	if (activeState && activeState->up_pressed){
		activeState->up_pressed();
	}
}
void ActiveState_downPressed(){
	if (activeState && activeState->down_pressed){
		activeState->down_pressed();
	}
}
void ActiveState_leftPressed(){
	if (activeState && activeState->left_pressed){
		activeState->left_pressed();
	}
}
void ActiveState_rightPressed(){
	if (activeState && activeState->right_pressed){
		activeState->right_pressed();
	}
}
void ActiveState_startPressed(){
	if (activeState->start_pressed){
		activeState->start_pressed();
	}
}
void ActiveState_playSound(){
	if (activeState && activeState->play_sound){
		activeState->play_sound();
	}
}
void ActiveState_drawInitial(){
	if (activeState && activeState->draw_initial){
		activeState->draw_initial();
	}
}
TopLevelState* ActiveState_get(){
		return activeState;
}
void ActiveState_set(TopLevelState* newState){
	long sr = StartCritical();
	activeState = newState;
	if (!needInitialDraw){
		needInitialDraw = TRUE;
	}
	EndCritical(sr);
}
