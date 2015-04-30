#ifndef _STOP_STATE_H_
#define _STOP_STATE_H_
#include "TopLevelState.h"
extern TopLevelState Stop;
void StopState_startPressed(void);
void StopState_upPressed(void);
void StopState_downPressed(void);
void StopState_updateState(void);
void StopState_playSound(void);

#endif
