#ifndef _WAV_READER_H_
#define _WAV_READER_H_
#include "ff.h"
#include "stdint.h"
extern FIL Handle;
extern FIL Handle2;
extern FRESULT MountFresult;
extern FRESULT Fresult;
extern uint8_t looped;
extern uint8_t song_playing;
extern uint8_t needMore;
extern unsigned char buffer[512];

void music_init(void);

void music_play(const char* fileName);

void music_stop(void);

void load_more(void);
#endif
