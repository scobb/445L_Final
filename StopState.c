#include "StopState.h"
#include "ST7735.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "ScoreEngine.h"
#include "ActiveState.h"
#include "WavReader.h"
#include "stdio.h"

#define DEATH_CYCLES 7
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
const unsigned short pacman_death_1[] = {
 0x0000, 0x0020, 0x03CF, 0x028A, 0x0000, 0x0041, 0x07BE, 0x07FF, 0x02EB, 0x0000, 0x038E, 0x079E, 0x0638, 0x0000, 0x0000, 0x0041,
 0x07BE, 0x07FF, 0x02EB, 0x0000, 0x0000, 0x0020, 0x03CF, 0x028A, 0x0000,

};
const unsigned short pacman_death_2[] = {
 0x0000, 0x0104, 0x0249, 0x0000, 0x0000, 0x00E3, 0x07FF, 0x0451, 0x0000, 0x0000, 0x03CF, 0x07BE, 0x0410, 0x0000, 0x0000, 0x00E3,
 0x07FF, 0x0451, 0x0000, 0x0000, 0x0000, 0x0104, 0x0249, 0x0000, 0x0000,

};

const unsigned short pacman_death_3[] = {
 0x0000, 0x03CF, 0x0000, 0x0000, 0x0000, 0x01C7, 0x07FF, 0x03AE, 0x0000, 0x0000, 0x038E, 0x07BE, 0x0555, 0x0000, 0x0000, 0x0000,
 0x07FF, 0x0124, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

const unsigned short pacman_death_4[] = {
 0x0000, 0x00C3, 0x0000, 0x0000, 0x0000, 0x01E7, 0x07FF, 0x0061, 0x0000, 0x0000, 0x038E, 0x07FF, 0x0430, 0x0000, 0x0000, 0x0000,
 0x0514, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

const unsigned short pacman_death_5[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0269, 0x0269, 0x0000, 0x0000, 0x0000, 0x03AE, 0x079E, 0x02AA, 0x0000, 0x0000, 0x0041,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

const unsigned short pacman_death_6[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01C7, 0x0514, 0x0451, 0x0020, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

const unsigned short pacman_death_7[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5ACB, 0x632C, 0x5ACB, 0x0000, 0x0000, 0x632C, 0x0000, 0x632C, 0x0000, 0x0000,
 0x5ACB, 0x632C, 0x5ACB, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

uint8_t die_play = FALSE;
uint8_t die_done = FALSE;


const unsigned short* bmps[DEATH_CYCLES] = {pacman_death_1, pacman_death_2, pacman_death_3, pacman_death_4,
	pacman_death_5, pacman_death_6, pacman_death_7};
uint8_t death_step = 0;
TopLevelState Stop =  {
	&StopState_updateState,
	&StopState_upPressed,
	&StopState_downPressed,
	&StopState_leftPressed,
	&StopState_rightPressed,
	&StopState_startPressed,
	&StopState_playSound,
	0,
};

uint8_t see_scores = FALSE;

void StopState_startPressed(void){
	if (death_step >= DEATH_CYCLES){
		if ( see_scores && die_done ) {
			if (num_lives == 0){
				// display top scores
				die_play = FALSE;
				die_done = FALSE;
				death_step = 0;
				see_scores = FALSE;
				ActiveState_set(&TopScores);
			}
		}
	}
}

void StopState_upPressed(void){
	if (!see_scores && num_lives > 0) return;//we only need this when scores are shown and we lost/won
	ST7735_SetCursor(3+(2*initial), 10);
	printf(" ");
	ST7735_SetCursor(3+(2*initial), 10);
	if (current_initial[initial] == 'a') current_initial[initial] = 'z';
	else current_initial[initial] = current_initial[initial] - 1;
	printf("%c", current_initial[initial]);
}
	
void StopState_downPressed(void){
	if (!see_scores && num_lives > 0) return;
	ST7735_SetCursor(3+(2*initial), 10);
	printf(" ");
	ST7735_SetCursor(3+(2*initial), 10);
	if (current_initial[initial] == 'z') current_initial[initial] = 'a';
	else current_initial[initial] = current_initial[initial] + 1;
	printf("%c", current_initial[initial]);
}
	
void StopState_leftPressed(void){
	if (!see_scores && num_lives > 0) return;
	ST7735_SetCursor(3+(2*initial), 11);
	printf(" ");
	if (initial == 0) initial = 2;
	else initial = initial - 1;
	ST7735_SetCursor(3+(2*initial), 11);
	printf("^");
}
	
void StopState_rightPressed(void){
	if (!see_scores && num_lives > 0) return;
	ST7735_SetCursor(3+(2*initial), 11);
	printf(" ");
	if (initial == 2) initial = 0;
	else initial = initial + 1;
	ST7735_SetCursor(3+(2*initial), 11);
	printf("^");
}

void StopState_updateState(void){
	ST7735_SetCursor(0, 0);
	p.in_motion = FALSE;
	p.motion = RIGHT;
	if (death_step < DEATH_CYCLES){
		p.bmp[RIGHT] = bmps[death_step++];
		drawSprite(&p);
	} else {
		if (!see_scores && die_done){
			if (num_lives > 0) {
				--num_lives;
				GameEngine_reset();
				die_done = FALSE;
				die_play = FALSE;
				death_step = 0;
				ActiveState_set(&InGame);
			} else {
				ScoreEngine_displayFinalScore();
				see_scores = TRUE;
			}
		}
	}
}
void StopState_playSound(void){
	// pacman death sound
	if (!die_play){
		if (song_playing) music_stop();
		looped = FALSE;
		die_play = TRUE;
		music_play("die.wav");
	}
	if(needMore){
		load_more();
	}
}
