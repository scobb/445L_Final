#include "ActiveState.h"

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
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
	if (activeState->update_state){
		activeState->update_state();
	}
}
void ActiveState_upPressed(){
	if (activeState->up_pressed){
		activeState->up_pressed();
	}
}
void ActiveState_downPressed(){
	if (activeState->left_pressed){
		activeState->left_pressed();
	}
}
void ActiveState_leftPressed(){
	if (activeState->update_state){
		activeState->update_state();
	}
}
void ActiveState_rightPressed(){
	if (activeState->right_pressed){
		activeState->right_pressed();
	}
}
void ActiveState_startPressed(){
	if (activeState->start_pressed){
		activeState->start_pressed();
	}
}
void ActiveState_playSound(){
	if (activeState->play_sound){
		activeState->play_sound();
	}
}
void ActiveState_drawInitial(){
	if (activeState->draw_initial){
		activeState->draw_initial();
	}
}
TopLevelState* ActiveState_get(){
		return activeState;
}
void ActiveState_set(TopLevelState* newState){
	long sr = StartCritical();
	activeState = newState;
	activeState->draw_initial();
	EndCritical(sr);
}
