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

uint32_t score;

void ScoreEngine_init(){
	score = 0;
}

uint32_t ScoreEngine_getScore(void){
	return score;
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
	ST7735_SetCursor(80, 59);
	printf("Game Over\n");
	//Note: this part should be on the next line, just trying to center it
	ST7735_SetCursor(85, 56);
	printf("Final Score: %d", score);
}

void ScoreEngine_displayScores(){
	UINT successfulreads, successfulwrites;
  char c;
	char* currentScore = calloc(SCORE_SIZE*(sizeof c)+1, (sizeof c));
	char** scores;//TODO: Initialize this to some size
	uint8_t counter, totalCounter;
  //int16_t x, y; int i; uint32_t n;
	int i;
  c = 0;
  //x = 0;
  //y = 10;
	counter = 0;
	totalCounter = 0;
	printf("High Scores\n");
	
	Fresult = f_open(&Handle, "scores.txt", FA_READ);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened scores.txt", ST7735_Color565(0, 0, 255));
    for(i=0; i<FILETESTSIZE; i++){
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
      if((successfulreads == 1) && (Fresult == FR_OK)){
        //ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
				if (c == ','){
					(*currentScore) = 0;
					currentScore -= counter * (sizeof counter) - sizeof counter;//Should get us to the original pointer
					*scores = currentScore;
					counter = 0;
					totalCounter++;
				}
				(*currentScore) = c;
				counter++;
				currentScore += sizeof counter;
				/*
        x = x + 6;
        if(x > 122){
          x = 0;                          // start over on the next line
          y = y + 10;
        }
        if(y > 150){
          y = 10;                         // the screen is full
        }
				*/
      } else{
        ST7735_DrawString(0, 0, "f_read error", ST7735_Color565(0, 0, 255));
        while(1){};
      }

    }
  } else{
    ST7735_DrawString(0, 0, "Error scores.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(20, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
    while(1){};
  }
  //ST7735_DrawString(0, 0, "file test passed    ", ST7735_Color565(255, 255, 255));
  Fresult = f_close(&Handle);
	
	for (i = 0; i < totalCounter; i++){
		//In theory, will print out the scores
		long current = strtol(*scores, NULL, 10);
		if ((uint32_t) current < score){
			printf("%d\n", score);
		}
		printf("%s\n", (*scores));
		scores += sizeof (*scores);
	}
	
	//Now we need to write back
	Fresult = f_open(&Handle2, "scores.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult){
    ST7735_DrawString(0, 0, "score file error", ST7735_Color565(0, 0, 255));
		printf("\n%d\n", Fresult);
    while(1){};
  } else{
    for(i=0; i<(totalCounter+1); i++){
			char* currScore = (*scores);
			long current = strtol(currScore, NULL, 10);
			scores += sizeof (*currScore);
			if ((uint32_t) current < score){
				//We need to write our score in here next, before we do this other score
				char* ourScore = calloc(SCORE_SIZE*(sizeof c)+1, (sizeof c));
				sprintf(ourScore, "%d", score);
				c = (*currScore);
				while (c){
					Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
					if((successfulwrites != 1) || (Fresult != FR_OK)){
						ST7735_DrawString(0, 0, "f_write error", ST7735_Color565(0, 0, 255));
						while(1){};
					}
					currScore += sizeof c;
					c = (*currScore);
				}
				//Now that c is 0, we have finished this string, so we need to add a comma
				c = ',';
				Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
				if((successfulwrites != 1) || (Fresult != FR_OK)){
					ST7735_DrawString(0, 0, "f_write error", ST7735_Color565(0, 0, 255));
					while(1){};
				}
			}
			
			c = (*currScore);
			while (c){
				Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
				if((successfulwrites != 1) || (Fresult != FR_OK)){
					ST7735_DrawString(0, 0, "f_write error", ST7735_Color565(0, 0, 255));
					while(1){};
				}
				currScore += sizeof c;
				c = (*currScore);
			}
			//Now that c is 0, we have finished this string, so we need to add a comma
			c = ',';
			Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
			if((successfulwrites != 1) || (Fresult != FR_OK)){
				ST7735_DrawString(0, 0, "f_write error", ST7735_Color565(0, 0, 255));
				while(1){};
			}
    }
    Fresult = f_close(&Handle2);
    if(Fresult){
      ST7735_DrawString(0, 0, "file2 f_close error", ST7735_Color565(0, 0, 255));
      while(1){};
    }
  }
	
	//If we decide to allow another game to be played, we will need to reset the score back to 0
	score = 0;
}
