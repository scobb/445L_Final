#ifndef __TOP_LEVEL_STATE_H__
#define __TOP_LEVEL_STATE_H__
typedef struct {
	void (*update_state)(void);
	void (*up_pressed)(void);
	void (*down_pressed)(void);
	void (*left_pressed)(void);
	void (*right_pressed)(void);
	void (*start_pressed)(void);
	void (*play_sound)(void);
	void (*draw_initial)(void);
	
} TopLevelState;

#endif
