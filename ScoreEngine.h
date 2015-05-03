#ifndef _SCORE_ENGINE_H_
#define _SCORE_ENGINE_H_

#include "stdint.h"
void ScoreEngine_init(void);
void ScoreEngine_update(uint8_t object);
void ScoreEngine_displayScores(void);
void ScoreEngine_displayFinalScore(void);
uint32_t ScoreEngine_getScore(void);
#endif
