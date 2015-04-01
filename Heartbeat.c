#include "Heartbeat.h"
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#define LED (*((volatile uint32_t *)0x40007004)) //Uses PD0. Looks like mainly for colors (1=red)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void Timer0_Init(void);
void Heartbeat_blink(){
	LED ^= 1;
}

void Heartbeat_Init(){
	// assumes PORTD already initialized
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;   // 2) unlock GPIO Port F
  GPIO_PORTD_CR_R |= 0x01;           // allow changes to PF4-0
	GPIO_PORTD_DIR_R |= 0x01;        // make PF1 output (PF1 built-in LEDs)
  GPIO_PORTD_AFSEL_R &= ~0x01;     // disable alt funct on PF1
  GPIO_PORTD_DEN_R |= 0x01;        // enable digital I/O on PF1
                                   // configure PF1 as GPIO
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFFFFF0)+0x00000000;
  GPIO_PORTD_AMSEL_R &= ~0x01;
	
	LED = 0x00;
	Timer0_Init();
	
}
void Timer0_Init(void){volatile unsigned short delay;
	long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;      // 0) activate timer0
  TIMER0_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;       // 2) configure for 32-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // 3) configure for periodic mode, default down-count settings
  TIMER0_TAPR_R = 0;               // 5) 12.5ns timer0A
  TIMER0_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x60000000; // 8) priority 3
  NVIC_EN0_R = NVIC_EN0_INT19;     // 9) enable interrupt 19 in NVIC
	TIMER0_TAILR_R = 40000000;				// .5 s
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;	// 6) clear timer0A timeout flag
  TIMER0_IMR_R |= 0x00000001;      		// 7) arm timeout interrupt
  TIMER0_CTL_R |= 0x00000001;     		// 8) enable timer0A
  EndCritical(sr);
}
// Executed every 10 ms
void Timer0A_Handler(void){ 
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;       // acknowledge timer0A timeout
  Heartbeat_blink();
}
