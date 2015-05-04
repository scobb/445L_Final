#ifndef _START_STATE_H_
#define _START_STATE_H_
#include "TopLevelState.h"

#define TOP 11
#define BOTTOM 12
extern TopLevelState Start;
void StartState_startPressed(void);
void StartState_upPressed(void);
void StartState_downPressed(void);
void StartState_drawInitial(void);
void StartState_updateState(void);
void StartState_playSound(void);
#endif
