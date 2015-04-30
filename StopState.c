#include "StopState.h"
TopLevelState Stop =  {
	&StopState_updateState,
	0,
	0,
	0,
	0,
	&StopState_startPressed,
	&StopState_playSound,
	0,
};
void StopState_startPressed(void){
	// TODO - are we done with death animation?
	// TODO - if so, redirect appropriately
}
void StopState_updateState(void){
	// TODO - draw pacman unfold
}
void StopState_playSound(void){
	// TODO - pacman death sound
}
