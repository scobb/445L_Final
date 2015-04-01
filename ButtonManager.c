#include "ButtonManager.h"
#include "inc/tm4c123gh6pm.h"
#include "SysTick.h"
#include <stdint.h>

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
#define NUM_ON_D 1
#define NUM_ON_E 4
#define PD0       (*((volatile uint32_t *)0x40007004))
#define PD1       (*((volatile uint32_t *)0x40007008))
#define PD2       (*((volatile uint32_t *)0x40007010))
#define PD3       (*((volatile uint32_t *)0x40007020))
#define TRUE 1
#define FALSE 0
typedef struct {
  uint32_t readValue;
	uint8_t isLow;
	void(*handler)(void);
} buttonStatus;

void ButtonManager_Init(){
	volatile uint32_t delay;
	// for button on PD3
  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) activate clock for Port D
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTD_AFSEL_R &= ~0x08;        // 6) disable alt funct on PD1-0
  GPIO_PORTD_DIR_R &= ~0x08;          // 5) PD1-0 are in
  GPIO_PORTD_AMSEL_R &= ~0x08;        // 3) disable analog on PD
  GPIO_PORTD_PUR_R |= 0x08;          // enable pull-up on PD
  GPIO_PORTD_DEN_R |= 0x08;          // 7) enable digital I/O on PD
  GPIO_PORTD_PCTL_R &= ~0x0000F000; // configure PD1-0 as GPIO
	
  GPIO_PORTD_IS_R &= ~0x08;     // (d) PD0, 1 is edge-sensitive
  GPIO_PORTD_IBE_R &= ~0x08;    //     PD0, 1 is not both edges
  GPIO_PORTD_IEV_R &= ~0x08;    //     PD0, 1 falling edge event
  GPIO_PORTD_ICR_R = 0x08;      // (e) clear flag4
  GPIO_PORTD_IM_R |= 0x08;      // (f) arm interrupt on PD1-0 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0xA0000000; // (g) priority 6
  NVIC_EN0_R |= NVIC_EN0_INT3;      // (h) enable interrupt 19 in NVIC
	
	// for button on PE0-3
  SYSCTL_RCGCGPIO_R |= 0x00000010;  // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTE_AFSEL_R &= ~0x0F;        // 6) disable alt funct on PD1-0
  GPIO_PORTE_DIR_R &= ~0x0F;          // 5) PD1-0 are in
  GPIO_PORTE_AMSEL_R &= ~0x0F;        // 3) disable analog on PD
  GPIO_PORTE_PUR_R |= 0x0F;          // enable pull-up on PD
  GPIO_PORTE_DEN_R |= 0x0F;          // 7) enable digital I/O on PD
  GPIO_PORTE_PCTL_R &= ~0x0000FFFF; // configure PD1-0 as GPIO
	
  GPIO_PORTE_IS_R &= ~0x0F;     // (d) PD0, 1 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x0F;    //     PD0, 1 is not both edges
  GPIO_PORTE_IEV_R &= ~0x0F;    //     PD0, 1 falling edge event
  GPIO_PORTE_ICR_R = 0x0F;      // (e) clear flag4
  GPIO_PORTE_IM_R |= 0x0F;      // (f) arm interrupt on PD1-0 *** No IME bit as mentioned in Book ***
	// TODO - check these
  NVIC_PRI0_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x000000A0; // (g) priority 6
  NVIC_EN0_R |= NVIC_EN0_INT4;      // (h) enable interrupt 19 in NVIC
	
	
}
void upPressed(){
}
void downPressed(){
}
void leftPressed(){
}
void rightPressed(){
}
void CheckDebounce(buttonStatus* buttons, uint8_t numPorts){
	// private function to allow us to debounce all buttons
	uint8_t i;
	SysTick_Wait10ms(40);
	for (i=0; i < numPorts; ++i){
		// if a button was low before and is still low, call its handler
		if (buttons[i].isLow && buttons[i].readValue == 0){
			buttons[i].handler();
		}
	}
}
void GPIOPortE_Handler(void){
	// TODO 
}
void GPIOPortD_Handler(void){
	// handler for port D -- all 5 buttons
	uint8_t i;
	uint8_t needCheck = FALSE;
	
	/*// TODO - processing here
	buttonStatus ports[NUM_ON_D] = {
		{PD3, FALSE ,&upPressed},
	};
	GPIO_PORTD_ICR_R = 0x08;      // acknowledge flag 0-1
	
	// check all ports to see if any is low
	for (i=0; i < NUM_ON_D; i++){
		if (ports[i].readValue == 0){
			ports[i].isLow = TRUE;
			needCheck = TRUE;
		}
	}
	if (needCheck)
		CheckDebounce(&ports[0], 2);*/
}

