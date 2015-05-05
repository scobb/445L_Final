#ifndef _SCORE_ENGINE_H_
#define _SCORE_ENGINE_H_

#include "stdint.h"
#include "TopLevelState.h"
extern TopLevelState TopScores;
extern uint8_t initial;
extern char current_initial[];
void ScoreEngine_init(void);
void ScoreEngine_update(uint8_t object);
void ScoreEngine_startPressed(void);
void ScoreEngine_displayScores(void);
void ScoreEngine_displayFinalScore(void);
uint32_t ScoreEngine_getScore(void);
#endif
