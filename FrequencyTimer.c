// Timer0A.c
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1, Program 9.8

  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Volume 2, Program 7.5, example 7.6

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
#include <stdint.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "FrequencyTimer.h"
#include "MAX5353.h"
#define PF4       (*((volatile uint32_t *)0x40025040))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function
uint8_t myNum;
uint8_t wavePos;
uint16_t waveForm[32] = {
  2048,2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
  3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
  156,39,0,39,156,345,600,910,1264,1648};

// ***************** FrequencyTimer_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in 12.5ns units
// Outputs: none
void FrequencyTimer_Init(){
	long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;      // 0) activate timer0
	while (!(SYSCTL_RCGCTIMER_R & 0x01)) {}
  TIMER0_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;       // 2) configure for 32-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // 3) configure for periodic mode, default down-count settings
  TIMER0_TAPR_R = 0;               // 5) 12.5ns timer0A
  TIMER0_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x60000000; // 8) priority 3
  NVIC_EN0_R = NVIC_EN0_INT19;     // 9) enable interrupt 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
  EndCritical(sr);
}

void FrequencyTimer_arm(uint32_t periodCycles){
	long sr = StartCritical();
  TIMER0_CTL_R &= ~0x00000001;     		// 1) disable timer0A during setup
	if (periodCycles != 0){
		TIMER0_TAILR_R = periodCycles-1;    // 4) reload value
	}
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;	// 6) clear timer0A timeout flag
  TIMER0_IMR_R |= 0x00000001;      		// 7) arm timeout interrupt
  TIMER0_CTL_R |= 0x00000001;     		// 8) enable timer0A
	EndCritical(sr);
}

void FrequencyTimer_disarm() {
	long sr = StartCritical();
  TIMER0_CTL_R &= ~0x00000001;     	// 1) disable timer0A during setup
  TIMER0_IMR_R &= ~0x00000001;      // 7) disarm timeout interrupt
  TIMER0_CTL_R |= 0x00000001;     	// 8) enable timer0A
	EndCritical(sr);
}

	/*
	inputs: notes - list of note objects. first note must 
	have highest frequency.
	instruments - instrument objects. must match number of note entries.
	num - size of each array above.
	outputs: magnitude of combined wave.
	*/
/*
uint16_t FrequencyTimer_combine(Note* notes, Instrument* instruments, uint8_t num){
	static uint32_t total_steps = 0;
	uint8_t i;
	uint32_t total_mag;
	uint8_t* steps = (uint8_t*) malloc(sizeof(uint8_t) * num);
	// fastest frequency will increment every time.
	steps[0] = total_steps % NUM_STEPS;
	total_mag = (instruments[0].waveForm[steps[0]] * notes[0].dynamicPercent) / 100;
	for (i = 1; i < num; ++i){
		steps[i] = (total_steps * notes[0].periodCycles / notes[i].periodCycles) % NUM_STEPS;
		total_mag += (instruments[i].waveForm[steps[i]] * notes[i].dynamicPercent) / 100;
	}
 
	
	free(steps);
	return total_mag / num;
}
	
void FrequencyTimer_setNotes(Note* notes){
	myNote = notes;
}
void FrequencyTimer_setInstruments(Instrument* instruments){
	myInstrument = instruments;
}
void FrequencyTimer_setNum(uint8_t num){
	myNum = num;
}

void FrequencyTimer_getNotes(Note** notes){
	*notes = myNote;
}

void FrequencyTimer_getInstruments(Instrument** instruments){
	*instruments = myInstrument;
}

void FrequencyTimer_getPosition(uint8_t* position){
	*position = wavePos;
}
*/
void Timer0A_Handler(void){
	// two(notes[0].periodCycles, notes[1].periodCycles);
	static int8_t ind = 0;
	int8_t size = 32;
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
	ind = (ind + 1) % size;
	wavePos = ind;
	DAC_Out(waveForm[ind]);
}