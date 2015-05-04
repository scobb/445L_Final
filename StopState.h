#ifndef _STOP_STATE_H_
#define _STOP_STATE_H_
#include "TopLevelState.h"
#include "stdint.h"
extern TopLevelState Stop;
extern uint8_t see_scores;
void StopState_startPressed(void);
void StopState_upPressed(void);
void StopState_downPressed(void);
void StopState_updateState(void);
void StopState_playSound(void);

#endif
