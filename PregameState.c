#include "PregameState.h"

TopLevelState Pregame =  {
	&PregameState_updateState,
	0,
	0,
	0,
	0,
	0,
	0,
	&PregameState_drawInitial,
};
void PregameState_updateState(void){
	// TODO - count number of updates before starting
}

void PregameState_drawInitial(void) {
	// TODO - initial board draw with READY in center
}
