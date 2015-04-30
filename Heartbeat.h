#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_
#include "stdint.h"
void Heartbeat_Init();
extern uint8_t updateStateSemaphore;
#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif
#endif