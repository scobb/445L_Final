#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

void GraphicsEngine_drawInitial(TopLevelState* state){
	state->draw_initial();
}

void GraphicsEngine_redraw(TopLevelState* state){
}