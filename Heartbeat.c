#include "Heartbeat.h"
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#define LED (*((volatile uint32_t *)0x40004004)) //Uses PD0. Looks like mainly for colors (1=red)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void TIMER1_Init(void);
void Heartbeat_blink(){
	LED ^= 1;
}

void Heartbeat_Init(){
	// Interfaces with a 1.5 kohm resistor
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000001;  // 1) activate clock for Port D
	while (!(SYSCTL_RCGCGPIO_R & 0x00000001)){}
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTA_LOCK_R = GPIO_LOCK_KEY;   // 2) unlock GPIO Port D
  GPIO_PORTA_CR_R |= 0x01;           // allow changes to PD0
	GPIO_PORTA_DIR_R |= 0x01;        // make PD0 output
  GPIO_PORTA_AFSEL_R &= ~0x01;     // disable alt funct on PD0
  GPIO_PORTA_DEN_R |= 0x01;        // enable digital I/O on PD0
                                   // configure PD0 as GPIO
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFFF0)+0x00000000;
  GPIO_PORTA_AMSEL_R &= ~0x01;
	GPIO_PORTA_PUR_R |= 0x01;
	
	LED = 0x01;
	TIMER1_Init();
	
}
void TIMER1_Init(void){volatile unsigned short delay;
	long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x02;      // 0) activate TIMER1
	while (!(SYSCTL_RCGCTIMER_R & 0x02)) {}
	delay = SYSCTL_RCGCTIMER_R;
  TIMER1_CTL_R &= ~0x00000001;     // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;       // 2) configure for 32-bit timer mode
  TIMER1_TAMR_R = 0x00000002;      // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = 40000000;				// .5 s
	TIMER1_TAPR_R = 0;               // 5) 12.5ns TIMER1A
  TIMER1_ICR_R = 0x00000001;       // 6) clear TIMER1A timeout flag
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF1FFF)|0x00006000; // 8) priority 3
  NVIC_EN0_R = NVIC_EN0_INT21;     // 9) enable interrupt 19 in NVIC

  //TIMER1_ICR_R = TIMER_ICR_TATOCINT;	// 6) clear TIMER1A timeout flag
  TIMER1_IMR_R |= 0x00000001;      		// 7) arm timeout interrupt
  TIMER1_CTL_R |= 0x00000001;     		// 8) enable TIMER1A
  EndCritical(sr);
}
// Executed every 500 ms
void Timer1A_Handler(void){ 
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;       // acknowledge TIMER1A timeout
  Heartbeat_blink();
}
