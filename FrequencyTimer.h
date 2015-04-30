// Timer0A.h
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
  Program 7.5, example 7.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

 #ifndef _FREQUENCY_TIMER_H_
 #define _FREQUENCY_TIMER_H_
 #define A4 5673
 #include <stdint.h>
 //#include "Note.h"
 //#include "Instrument.h"

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in 12.5ns units
// Outputs: none
void FrequencyTimer_Init(void);	
/*
void FrequencyTimer_setNotes(Note* notes);
void FrequencyTimer_setInstruments(Instrument* instruments);
void FrequencyTimer_setNum(uint8_t num);
void FrequencyTimer_getNotes(Note** notes);
void FrequencyTimer_getInstruments(Instrument** instruments);
void FrequencyTimer_getPosition(uint8_t* position);*/
void FrequencyTimer_arm(uint32_t periodCycles);
void FrequencyTimer_disarm(void);
#endif // __TIMER0AINTS_H__
