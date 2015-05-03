// SDCFile.c
// Runs on TM4C123
// This program is a simple demonstration of the SD card,
// file system, and ST7735 LCD.  It will read from a file,
// print some of the contents to the LCD, and write to a
// file.
// Daniel Valvano
// January 13, 2015

/* This example accompanies the book
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Program 4.6, Section 4.3
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Program 2.10, Figure 2.37

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// 1  ground
// 2  Vcc +3.3V
// 3  PA7 TFT reset
// 4  PA6 TFT data/command
// 5  PD7 SDC_CS, active low to enable SDC
// 6  PA3 TFT_CS, active low to enable TFT
// 7  PA5 MOSI SPI data from microcontroller to TFT or SDC
// 8  PA2 Sclk SPI clock from microcontroller to TFT or SDC
// 9  PA4 MOSO SPI data from SDC to microcontroller
// 10 Light, backlight connected to +3.3 V

#include <stdint.h>
#include "diskio.h"
#include "ff.h"
#include "PLL.h"
#include "ST7735.h"
#include "WavReader.h"
#include "inc\tm4c123gh6pm.h"
#include "heartbeat.h"
#include <stdio.h>
#include <string.h>

//be sure to include this and check this flag!!!
extern uint8_t needMore;
void EnableInterrupts(void);

/*Bryce, here is a walk through about how to incorporate the wavfiles.
things that my wavreader uses, Timer2A.c, ff.c,diskio.c,DAC.c, so be sure to add all of these files to the project.
The way to interact with my wavreader is pretty easy, just call music_init() to intitialize everything
then call music_play("name of song file"). If you want the music to stop, call music_stop(); 
There is a boolean flag in the WavReader.c call needMore, include this as I have done and check it constantly as I am doing here
I want the loading from the SD card to be done in the foreground do the music never has to wait for a load to output.
check this flag constantly, and call load_more() when it is true. That's it!
*/
/*
void music_test(void){
	music_init();
	//music_play("8PCMtest.wav");
	//music_play("laugh.wav");
	//music_play("PCMtest.wav");
	music_play("spring.wav");
	while(1){
		if(needMore){
			load_more();
		}
	}
}	

int main(void){
  UINT successfulreads, successfulwrites;
  uint8_t c, x, y;
	uint32_t testInt;
  PLL_Init();    // 80 MHz
  EnableInterrupts();
	//heartbeat_init();
	music_test();
	
  
}
*/
