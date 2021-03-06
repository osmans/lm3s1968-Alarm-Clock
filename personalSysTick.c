/*This is a SysTick file written by team
William Kelsey, wdk279
Vincent Steil, vjs432


WHAT IS THE CORE CLOCK SPEED??

*/
#include "personalSysTick.h"



#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = period -1;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;		  // enable SysTick with core clock
}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
void SysTick_Wait(unsigned long delay){
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}
// Time delay using busy wait.
// This assumes 50 MHz system clock.
void SysTick_10ms_wait(unsigned long delay){
  unsigned long i;
	SysTick_Init(500000);
  for(i=0; i<delay; i++){
    SysTick_Wait(500000);  // wait 10ms (assumes 50 MHz clock)
  }
}


/*Converts number of seconds into mins, hours and days*/
/*number of seconds is a counter incremented every */
/*
void timeKeeper(int NumberOfSeconds) {
	minutes = NumberOfSeconds % 60;
	hours = NumberOfSeconds/(60*60);
	days = NumberOfSeconds/(60*60*24);
	
	//Worth resetting the counter once a week. int is 32 bits so can store 1 week of seconds
	if((days == 7) && (minutes == 0) && (seconds ==0)) {
		NumberOfSeconds =0;
	}
}
*/
