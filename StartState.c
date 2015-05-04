#include "StartState.h"
#include "stdint.h"
#include "WavReader.h"
#include "GameEngine.h"
#include "ActiveState.h"
#include "GraphicsEngine.h"
#include "ScoreEngine.h"
#include "WavReader.h"
#include "ST7735.h"
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif
uint8_t cursorLocation = TOP;
uint8_t needRedraw = FALSE;
uint8_t playing = FALSE;
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
TopLevelState Start =  {
	&StartState_updateState,
	&StartState_upPressed,
	&StartState_downPressed,
	0,
	0,
	&StartState_startPressed,
	&StartState_playSound,
	&StartState_drawInitial,
};
void StartState_playSound(void) {
	// play dat song
	if (!playing){
		if (song_playing) music_stop();
		looped = FALSE;
		playing = TRUE;
		music_play("opening.wav");
	}
	if(needMore){
		load_more();
	}
}
void StartState_startPressed(void){
	// check the location of the cursor, forward to appropriate new state
	if (song_playing) {
		music_stop();
	}
	if (cursorLocation == TOP){
		GameEngine_Init();
		GameEngine_reset();
		GraphicsEngine_initBoard();
		playing = FALSE;
		ActiveState_set(&InGame);
	} else {
		playing = FALSE;
		ActiveState_set(&TopScores);
	}
}
void swapCursor(void){
	// helper function - swap cursor position
	cursorLocation = cursorLocation == TOP ? BOTTOM : TOP;
}
void StartState_upPressed(void){
	swapCursor();
	needRedraw = TRUE;
}
void StartState_downPressed(void){
	swapCursor();
	needRedraw = TRUE;
}
void StartState_drawInitial(void){
	// draw title screen
	GraphicsEngine_drawTitle();
	
	// draw cursor
	GraphicsEngine_drawCursor(cursorLocation);
}
void StartState_updateState(void){
	if (needRedraw){
		// erase opposite cursor position, draw current cursor position
		GraphicsEngine_drawCursor(cursorLocation);
	}
}
