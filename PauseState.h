#ifndef _PAUSE_STATE_H_
#define _PAUSE_STATE_H_
#include "TopLevelState.h"

extern TopLevelState Paused;
void PauseState_startPressed(void);
void PauseState_playSound(void);
void PauseState_drawInitial(void);
#endif
