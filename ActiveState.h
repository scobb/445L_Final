#ifndef _ACTIVE_STATE_H_
#define _ACTIVE_STATE_H_
#include "TopLevelState.h"
#include "stdint.h"
extern uint8_t needInitialDraw;
TopLevelState* ActiveState_get(void);
void ActiveState_set(TopLevelState*);
void ActiveState_updateState(void);
void ActiveState_upPressed(void);
void ActiveState_downPressed(void);
void ActiveState_leftPressed(void);
void ActiveState_rightPressed(void);
void ActiveState_startPressed(void);
void ActiveState_playSound(void);
void ActiveState_drawInitial(void);
#endif
