#include "ActiveState.h"

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
TopLevelState* activeState;

TopLevelState* ActiveState_get(){
		return activeState;
}
void ActiveState_set(TopLevelState* newState){
	long sr = StartCritical();
	activeState = newState;
	activeState->draw_initial();
	EndCritical(sr);
}