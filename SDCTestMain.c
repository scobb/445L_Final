// SDCTestMain.c
// Runs on TM4C123/LM4F120
// Test Secure Digital Card read/write interface by writing test
// data, reading them back, and verifying that they match as
// expected.  Running the unformatted file tests will destroy
// any formatting already on the disk.  The formatted file tests
// will not work unless the disk has already been formatted.
// Valvano
// March 17, 2014

/* This example accompanies the books
   Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers, Volume 3,  
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2013

   Volume 3, Program 6.3, section 6.6   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

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

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) connected to PA4 (SSI0Rx)
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss) <- GPIO high to disable TFT
// CARD_CS (pin 5) connected to PD7 GPIO output 
// Data/Command (pin 4) connected to PA6 (GPIO)<- GPIO low not using TFT
// RESET (pin 3) connected to PA7 (GPIO)<- GPIO high to disable TFT
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground


#include "../inc/tm4c123gh6pm.h"
#include "diskio.h"
#include "ff.h"
#include "PLL.h"
#include "ST7735.h"
#include "Heartbeat.h"
#include "ButtonManager.h"
#include "GameEngine.h"
#include "MAX5353.h"
#include "FrequencyTimer.h"
#include "ActiveState.h"
void EnableInterrupts(void);

static FATFS g_sFatFs;
FIL Handle,Handle2;
FRESULT MountFresult;
FRESULT Fresult;
unsigned char buffer[512];
#define MAXBLOCKS 100
// Describe the error with text on the LCD and then stall.  If
// the error was caused by a mistake in configuring SSI0, then
// the LCD will probably not work.
void diskError(char *errtype, int32_t code, int32_t block){
  ST7735_DrawString(0, 0, "Error:", ST7735_Color565(255, 0, 0));
  ST7735_DrawString(7, 0, errtype, ST7735_Color565(255, 0, 0));
  ST7735_DrawString(0, 1, "Code:", ST7735_Color565(255, 0, 0));
  ST7735_SetCursor(6, 1);
  ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
  ST7735_OutUDec(code);
  ST7735_DrawString(0, 2, "Block:", ST7735_Color565(255, 0, 0));
  ST7735_SetCursor(7, 2);
  ST7735_OutUDec(block);
  while(1){};
}

#define FILETESTSIZE 10000
void FileSystemTest(void){
  UINT successfulreads, successfulwrites;
  char c, d;
  int16_t x, y; int i; uint32_t n;
  c = 0;
  x = 0;
  y = 10;
  n = 1;    // seed
  Fresult = f_open(&Handle2, "testFile.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult){
    ST7735_DrawString(0, 0, "testFile error", ST7735_Color565(0, 0, 255));
		printf("\n%d\n", Fresult);
    while(1){};
  } else{
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      c = ((n>>24)%10)+'0'; // random digit 0 to 9
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
  n = 1;  // reseed, start over to get the same sequence
  Fresult = f_open(&Handle, "testFile.txt", FA_READ);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened testFile.txt", ST7735_Color565(0, 0, 255));
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      d = ((n>>24)%10)+'0'; // expected random digit 0 to 9
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
      if((successfulreads == 1) && (Fresult == FR_OK) && (c == d)){
        ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
        x = x + 6;
        if(x > 122){
          x = 0;                          // start over on the next line
          y = y + 10;
        }
        if(y > 150){
          y = 10;                         // the screen is full
        }
      } else{
        ST7735_DrawString(0, 0, "f_read error", ST7735_Color565(0, 0, 255));
        while(1){};
      }

    }
  } else{
    ST7735_DrawString(0, 0, "Error testFile.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(20, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
    while(1){};
  }
  ST7735_DrawString(0, 0, "file test passed    ", ST7735_Color565(255, 255, 255));
  Fresult = f_close(&Handle);
/*  Fresult = f_open(&Handle,"out.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened out.txt", ST7735_Color565(0, 0, 255));
    c = 'A';
    x = 0;
    y = 10;
    Fresult = f_write(&Handle, &c, 1, &successfulreads);
    ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
    while((c <= 'Z') && (Fresult == FR_OK)){
      x = x + 6;
      c = c + 1;
      if(x > 122){
        x = 0;                          // start over on the next line
        y = y + 10;
      }
      if(y > 150){
        break;                          // the screen is full
      }
      Fresult = f_write(&Handle, &c, 1, &successfulreads);
      ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
    }
  } else{
    ST7735_DrawString(0, 0, "Error out.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(15, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
  }*/
}

const char inFilename[] = "test.txt";   // 8 characters or fewer
const char outFilename[] = "out.txt";   // 8 characters or fewer

int main(void){  
	/*DRESULT res;            
  UINT successfulreads, successfulwrites;
  uint8_t c, x, y;
	int i = 0;*/
	PLL_Init();    								// bus clock at 80 MHz
	Heartbeat_Init();							// heartbeat
  ButtonManager_Init();					// button interrupt enable
	Output_Init();								// Display, SSI2 enable
	printf("PACMAN Player 1\n");
	DAC_Init(2048);								// SSI0 enable
	FrequencyTimer_Init();
	FrequencyTimer_arm(A4);
  EnableInterrupts();
	GameEngine_Init();
	while (1) {
	if (updateStateSemaphore) {
			ActiveState_get()->update_state(); 
			updateStateSemaphore = FALSE;
		} 
	}
}

