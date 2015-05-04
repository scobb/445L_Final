#include <stdint.h>
#include <stdlib.h>
#include "inc//tm4c123gh6pm.h"
#include "GraphicsEngine.h"
#include "ScoreEngine.h"
#include "ST7735.h"
#include "stdio.h"
#include "ff.h"
#include "diskio.h"
#include "WavReader.h"
#include "GraphicsEngine.h"

#define FILETESTSIZE 10000
#define SCORE_SIZE 5
#define FALSE 0
#define TRUE 1

uint32_t score;
uint8_t found_score = FALSE;
UINT reads, writes;
TopLevelState TopScores =  {
	0,
	0,
	0,
	0,
	0,
	&ScoreEngine_startPressed,
	0,
	&ScoreEngine_displayScores,
};

void ScoreEngine_init(){
	score = 0;
}

uint32_t ScoreEngine_getScore(void){
	return score;
}

void ScoreEngine_startPressed(){
	
}

void ScoreEngine_update(uint8_t object){
	switch (object) {
		case GHOST:
			score += 100;
			break;
		case DOT:
			score += 25;
			break;
		case BIGDOT:
			score += 50;
			break;
		case FRUIT:
			score += 200;
			break;
	}
}

void ScoreEngine_displayFinalScore(){
	//The big thing we want to do is move our printf to the middle of the display
	ST7735_FillScreen(0);
	ST7735_SetCursor(3, 6);
	printf("Game Over");
	ST7735_SetCursor(3, 7);
	printf("Final Score: %d", score);
}

void ScoreEngine_displayScores(){
	//f_open and f_read/f_write
	unsigned char current_score[5];
	uint8_t index = 0;
	
	ST7735_FillScreen(0);
	Fresult = f_open(&Handle, "scores.txt", FA_READ);
	Fresult = f_read(&Handle, buffer, 512, &reads);
	if (Fresult == FR_OK){
		int i = 0;
		while (buffer[i] != 0){
			char c = buffer[i];
			if (c != ','){
				//digit, so let's add it to a string
				current_score[index] = c;
				index++;
			}
			else {
				if (index < 5){
					//score is lower than 5 digits, so let's fill the back end of this array with 0's
					for (index; index <= 5; index++){
						current_score[index] = 0;
					}
				}
				printf("%s\n", current_score);
				index = 0;
			}
			i++;
		}
	}
	else {
		
	}
}
