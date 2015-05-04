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
#include "ActiveState.h"
#include "StartState.h"

#define FILETESTSIZE 10000
#define SCORE_SIZE 5
#define FALSE 0
#define TRUE 1

uint32_t score;
uint8_t found_score = FALSE;
UINT reads, writes;
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
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
	ActiveState_set(&Start);
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
	//unsigned char current_score[6];
	unsigned char scores[25][6];
	uint8_t index = 0;
	uint32_t total_index = 0;
	int i = 0;
	
	ST7735_FillScreen(0);
	ST7735_SetCursor(0, 0);
	printf("High Scores");
	Fresult = f_open(&Handle, "scores.txt", FA_READ);
	Fresult = f_read(&Handle, buffer, 512, &reads);
	if (Fresult == FR_OK){
		while (buffer[i] != 0){
			char c = buffer[i];
			if (c != ','){
				if (c == '!') break;
				//digit, so let's add it to a string
				scores[total_index][index] = c;
				index++;
			}
			else {
				if (index < 5){
					//score is lower than 5 digits, so let's fill the back end of this array with 0's
					for (; index <= 5; index++){
						scores[total_index][index] = 0;
					}
				}
				scores[total_index][5] = 0;
				//printf("%s\n", current_score);
				total_index++;
				index = 0;
			}
			i++;
		}
	}
	else {
		printf("We're sorry, the scores on the \nSD card were too high to print to \nthis screen.");
	}
	Fresult = f_close(&Handle);
	//Now let's loop through and print to the screen
	if (score == 0){
		//Means we got here from the home screen
		for (i = 0; i < total_index; i++){
			printf("%s\n", scores[i]);
		}
	}
	else {
		//Means we finished a game
		uint8_t found_score = FALSE;
		for (i = 0; i < total_index+1; i++){
			if ((i < total_index && atoi(scores[i]) > score) || found_score){
				//This means we print the score we read in
				printf("%s\n", scores[i]);
			}
			else if (i < total_index && atoi(scores[i]) <= score && !found_score){
				printf("%u\n", score);
				printf("%s\n", scores[i]);
				found_score = TRUE;
			}
			else if (!found_score && i == total_index){
				printf("%u\n", score);
			}
		}
	}
	
	Fresult = f_open(&Handle, "scores.txt", FA_WRITE);
	if (Fresult == FR_OK){
		uint8_t found_score = FALSE;
		for (i = 0; i < total_index; i++){
			char current_score[7];
			int j;
			
			if (atoi(scores[i]) <= score && !found_score){
				//We need to put our score in first
				char our_score[7];
				int j;
				sprintf(our_score, "%u", score);
				while (our_score[j] != 0) j++;
				our_score[j] = ',';
				j++;
				/*
				for (; j < 7; j++){
					our_score[j] = 0;
				}
				*/
				f_write(&Handle, our_score, j, &writes);
				found_score = TRUE;
			}
			
			for (j = 0; j < 7; j++){
				if (scores[i][j] != 0) current_score[j] = scores[i][j];
				else {
					//We need to put a comma here
					current_score[j] = ',';
					break;
				}
			}
			f_write(&Handle, current_score, ++j, &writes);
		}
		
		if (!found_score && score != 0){
			char our_score[7];
			int j;
			sprintf(our_score, "%u", score);
			while (our_score[j] != 0) j++;
			our_score[j] = ',';
			for (; j < 7; j++){
				our_score[j] = 0;
			}
			f_write(&Handle, our_score, 7, &writes);
		}
		
		//Write the sentinel
		f_write(&Handle, "!", 1, &writes);
	}
	Fresult = f_close(&Handle);
}
