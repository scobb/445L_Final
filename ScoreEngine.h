enum Objects {
	ghost = 0x00,
	smallDot = 0x01,
	bigDot = 0x02,
	fruit = 0x03,
};

void ScoreEngine_init(void);
void ScoreEngine_update(uint8_t object);
void ScoreEngine_displayScores(void);
void ScoreEngine_displayFinalScore(void);
