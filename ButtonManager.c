#include "ButtonManager.h"
#include "inc/tm4c123gh6pm.h"
#include "SysTick.h"
#include "ActiveState.h"
#include <stdint.h>

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
#define NUM_ON_D 1
#define NUM_ON_E 4
#define PE0       ((volatile uint32_t *)0x40024004)
#define PE1       ((volatile uint32_t *)0x40024008)
#define PE2       ((volatile uint32_t *)0x40024010)
#define PE3       ((volatile uint32_t *)0x40024020)
#define PD3       ((volatile uint32_t *)0x40007020)
#define TRUE 1
#define FALSE 0
typedef struct {
  volatile uint32_t* readValue;
	uint8_t isLow;
	void(*handler)(void);
} buttonStatus;
void PortD_Init(void);
void PortE_Init(void);
void ButtonManager_Init(void){
	PortD_Init();
	PortE_Init();
}
void PortD_Init() {
	// for button on PD3
  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) activate clock for Port D
	while (!(SYSCTL_RCGCGPIO_R & 0x00000008)){}
  GPIO_PORTD_AFSEL_R &= ~0x08;        // 6) disable alt funct on PD3
  GPIO_PORTD_DIR_R &= ~0x08;          // 5) PD3 are in
  GPIO_PORTD_AMSEL_R &= ~0x08;        // 3) disable analog on PD
  GPIO_PORTD_PUR_R |= 0x08;          // enable pull-up on PD
  GPIO_PORTD_DEN_R |= 0x08;          // 7) enable digital I/O on PD
  GPIO_PORTD_PCTL_R &= ~0x0000F000; // configure PD3 as GPIO
	
  GPIO_PORTD_IS_R &= ~0x08;     // (d) PD3 is edge-sensitive
  GPIO_PORTD_IBE_R &= ~0x08;    //     PD3 is not both edges
  GPIO_PORTD_IEV_R &= ~0x08;    //     PD3 falling edge event
  GPIO_PORTD_DR2R_R |= 0x08;        // 2mA output on outputs
  GPIO_PORTD_ICR_R = 0x08;      // (e) clear flag4
  GPIO_PORTD_IM_R |= 0x08;      // (f) arm interrupt on PD3 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0xA0000000; // (g) priority 6
  NVIC_EN0_R |= NVIC_EN0_INT3;      // (h) enable interrupt 3 in NVIC
}

void PortE_Init() {
	// for button on PE0-3
  SYSCTL_RCGCGPIO_R |= 0x00000010;  // 1) activate clock for Port E
	while (!(SYSCTL_RCGCGPIO_R & 0x00000010)){} 
  GPIO_PORTE_AFSEL_R &= ~0x0F;        // 6) disable alt funct on PE3-0
  GPIO_PORTE_DIR_R &= ~0x0F;          // 5) PE3-0 are in
  GPIO_PORTE_AMSEL_R &= ~0x0F;        // 3) disable analog on PE
  GPIO_PORTE_PUR_R |= 0x0F;          // enable pull-up on PE
  GPIO_PORTE_DEN_R |= 0x0F;          // 7) enable digital I/O on PE
  GPIO_PORTE_PCTL_R &= ~0x0000FFFF; // configure PE3-0 as GPIO
	
  GPIO_PORTE_IS_R &= ~0x0F;     // (d) PE3-0 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x0F;    //     PE3-0, 1 is not both edges
  GPIO_PORTE_IEV_R &= ~0x0F;    //     PE3-0, 1 falling edge event
  GPIO_PORTE_DR2R_R |= 0x0F;        // 2mA output on outputs
  GPIO_PORTE_ICR_R = 0x0F;      // (e) clear flag4
  GPIO_PORTE_IM_R |= 0x0F;      // (f) arm interrupt on PE3-0 *** No IME bit as mentioned in Book ***
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x000000A0; // (g) priority 6
  NVIC_EN0_R |= NVIC_EN0_INT4;      // (h) enable interrupt 4 in NVIC
	
}
void CheckDebounce(buttonStatus* buttons, uint8_t numPorts){
	// private function to allow us to debounce all buttons
	uint8_t i;
	SysTick_Wait10ms(1);
	for (i=0; i < numPorts; ++i){
		// if a button was low before and is still low, call its handler
		if (buttons[i].isLow && *(buttons[i].readValue) == 0){
			buttons[i].handler();
		}
	}
}
void GPIOPortE_Handler(void){
	// local variables
	uint8_t i;
	uint8_t needCheck = FALSE;
	buttonStatus ports[NUM_ON_E] = {
		{PE0, FALSE ,&ActiveState_upPressed},
		{PE1, FALSE, &ActiveState_downPressed},
		{PE2, FALSE, &ActiveState_leftPressed},
		{PE3, FALSE, &ActiveState_rightPressed}
	};
	
	// process the interrupt
	GPIO_PORTE_ICR_R |= 0x0F;      // acknowledge flag PE3-0
	for (i=0; i < NUM_ON_E; i++){
		if (*(ports[i].readValue) == 0){
			ports[i].isLow = TRUE;
			//ports[i].handler();
			needCheck = TRUE;
		}
	}
	if (needCheck)
		CheckDebounce(&ports[0], NUM_ON_E);
}
void GPIOPortD_Handler(void){
	// local variables
	uint8_t i;
	uint8_t needCheck = FALSE;
	buttonStatus ports[NUM_ON_D] = {
		{PD3, FALSE ,&ActiveState_startPressed},
	};
	
	// process the interrupt
	GPIO_PORTD_ICR_R |= 0x08;      // acknowledge flag PD3
	
	for (i=0; i < NUM_ON_D; i++){
		if (*(ports[i].readValue) == 0){
			ports[i].isLow = TRUE;
			needCheck = TRUE;
		}
	}
	if (needCheck)
		CheckDebounce(&ports[0], NUM_ON_D);
}

