#include "PauseState.h"
#include "GameEngine.h"
#include "ActiveState.h"
#include "WavReader.h"
TopLevelState Paused = {
	0,
	0,
	0,
	0,
	0,
	&PauseState_startPressed,
	&PauseState_playSound,
	&PauseState_drawInitial,
};


void PauseState_startPressed(){
	// TODO - might need to have critical here
	ActiveState_set(&InGame);
	
	// TODO - might want to set a semaphore here that draws initial inside main loop?
	ActiveState_get()->draw_initial();
}

void PauseState_playSound(){
	if (song_playing) music_stop();
}

void PauseState_drawInitial(){
	// TODO - Write PAUSED across center of screen, blank screen
}
