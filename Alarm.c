// Alarm.c
// Vincent Steil & William Kelsey
// Runs on LM3S1968 or LM3S8962
// February 6, 2013
// The fsm behind an alarm. One state and alarm per weekday.

#include "Alarm.h"

#define Monday    &Weekday[0]
#define Tuesday   &Weekday[1]
#define Wednesday &Weekday[2]
#define Thursday  &Weekday[3]
#define Friday    &Weekday[4]
#define Saturday  &Weekday[5]
#define Sunday    &Weekday[6]


struct State {
  struct State *Next;
  unsigned char Current;
  struct State *Previous;
  unsigned int hour;
  unsigned int minute;
  unsigned char enabled;
};
typedef struct State Day;
Day *day;

Day Weekday[7]={
  {Sunday, 0, Tuesday, 0, 0, 0},
  {Monday, 1, Wednesday, 0, 0, 0},
  {Tuesday, 2, Thursday, 0, 0, 0},
  {Wednesday, 3, Friday, 0, 0, 0},
  {Thursday, 4, Saturday, 0, 0, 0},
  {Friday, 5, Sunday, 0, 0, 0},
  {Saturday, 6, Monday, 0, 0, 0}
};

void Alarm_Init(void){
  day = Monday;
}

void Alarm_NextDay(void){
  day = (*day).Next;
}

void Alarm_PreviousDay(void){
  day = (*day).Previous;
}

unsigned char Alarm_CurrentDay(void){
  return (*day).Current;
}

void Alarm_IncrementHour(void){
  (*day).hour++;
  (*day).hour %= 24;
}

void Alarm_DecrementHour(void){
  if ((*day).hour == 0){
    (*day).hour = 23;
  }
  else {
    (*day).hour--;
  }
}

void Alarm_IncrementMinute(void){
  (*day).minute++;
  (*day).minute %= 60;
}

void Alarm_DecrementMinute(void){
  if ((*day).minute == 0){
    (*day).minute = 59;
  }
  else {
    (*day).minute--;
  }
}

void Alarm_ToggleEnable(void){
  (*day).enabled ^= 0x01;             // toggle the enable flag
}

unsigned char Alarm_ReturnStatus(void){
  return (*day).enabled;              // returns status (enabled/disabled of the current weekday
}

int Alarm_ReturnHour(void){
  return (*day).hour;
}

int Alarm_ReturnMinute(void){
  return (*day).minute;
}


const unsigned char SmallExplosion[1500] = {		
	8, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 8, 8, 8, 9, 8, 8, 		
	7, 6, 6, 6, 6, 7, 8, 9, 9, 8, 8, 7, 6, 6, 6, 7, 7, 7, 8, 7, 		
	8, 8, 9, 9, 10, 11, 11, 11, 10, 9, 8, 7, 6, 5, 5, 5, 5, 5, 5, 6, 		
	8, 9, 9, 9, 9, 9, 10, 11, 11, 10, 8, 7, 7, 7, 7, 7, 9, 9, 9, 8, 		
	8, 9, 10, 11, 11, 12, 12, 12, 11, 10, 9, 7, 5, 2, 0, 0, 0, 0, 1, 1, 		
	3, 5, 7, 8, 8, 9, 9, 11, 12, 13, 13, 13, 12, 11, 11, 13, 15, 16, 16, 16, 		
	16, 16, 15, 13, 12, 11, 8, 5, 3, 2, 3, 2, 2, 3, 4, 3, 2, 0, 0, 0, 		
	0, 0, 0, 1, 2, 4, 8, 10, 11, 13, 14, 15, 16, 15, 15, 14, 12, 10, 6, 3, 		
	0, 0, 0, 0, 0, 0, 4, 6, 8, 10, 12, 14, 15, 15, 14, 12, 10, 10, 10, 10, 		
	10, 10, 8, 5, 2, 1, 0, 1, 3, 4, 6, 9, 11, 12, 13, 13, 12, 10, 10, 10, 		
	10, 11, 12, 11, 10, 9, 10, 12, 15, 16, 16, 16, 16, 16, 16, 14, 11, 7, 2, 0, 		
	0, 0, 1, 4, 6, 8, 11, 11, 11, 10, 9, 7, 6, 5, 3, 2, 2, 3, 2, 3, 		
	5, 8, 10, 9, 8, 6, 4, 4, 5, 6, 8, 10, 11, 12, 14, 15, 16, 15, 11, 7, 		
	3, 0, 0, 0, 1, 4, 7, 11, 14, 14, 11, 7, 4, 3, 2, 2, 2, 1, 1, 1, 		
	2, 5, 9, 14, 16, 16, 16, 16, 16, 16, 16, 15, 13, 10, 5, 0, 0, 0, 0, 0, 		
	0, 0, 3, 5, 5, 4, 4, 6, 7, 8, 8, 8, 8, 9, 12, 14, 16, 16, 16, 16, 		
	16, 16, 16, 16, 16, 16, 16, 16, 13, 9, 5, 2, 0, 0, 0, 0, 0, 3, 7, 8, 		
	7, 5, 3, 1, 2, 4, 5, 6, 6, 6, 7, 7, 8, 10, 13, 16, 16, 16, 16, 16, 		
	16, 14, 10, 7, 5, 6, 8, 10, 12, 13, 12, 9, 5, 0, 0, 0, 0, 0, 0, 0, 		
	0, 3, 8, 13, 16, 16, 16, 16, 16, 14, 11, 9, 7, 4, 0, 0, 0, 0, 0, 0, 		
	0, 2, 8, 12, 13, 13, 14, 15, 16, 16, 16, 16, 13, 13, 14, 16, 16, 16, 16, 16, 		
	13, 9, 7, 7, 8, 10, 12, 14, 14, 12, 10, 8, 7, 7, 6, 6, 6, 6, 7, 7, 		
	7, 5, 3, 1, 1, 3, 5, 7, 8, 9, 10, 12, 15, 16, 16, 16, 16, 16, 16, 16, 		
	16, 16, 11, 7, 3, 1, 2, 4, 5, 8, 11, 14, 15, 13, 10, 6, 2, 0, 0, 0, 		
	0, 1, 2, 5, 8, 9, 10, 11, 11, 12, 11, 7, 4, 2, 2, 2, 2, 2, 3, 4, 		
	4, 4, 5, 6, 5, 5, 5, 4, 3, 3, 4, 5, 6, 9, 12, 14, 15, 14, 13, 10, 		
	9, 9, 9, 9, 7, 6, 5, 5, 7, 10, 14, 16, 16, 15, 15, 14, 13, 11, 9, 8, 		
	7, 6, 6, 6, 7, 8, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 12, 		
	12, 12, 12, 11, 10, 10, 9, 9, 7, 7, 6, 6, 6, 6, 6, 7, 6, 6, 5, 4, 		
	4, 4, 5, 5, 5, 5, 6, 8, 9, 9, 10, 10, 11, 11, 11, 11, 10, 10, 9, 7, 		
	5, 3, 2, 1, 2, 2, 4, 5, 7, 7, 8, 9, 10, 11, 11, 10, 9, 9, 8, 8, 		
	8, 9, 8, 8, 6, 5, 4, 4, 4, 5, 5, 6, 7, 8, 9, 9, 9, 8, 9, 10, 		
	11, 11, 11, 10, 9, 8, 7, 7, 8, 11, 13, 15, 16, 16, 16, 16, 16, 14, 10, 5, 		
	1, 0, 0, 3, 5, 6, 6, 8, 8, 7, 7, 6, 4, 3, 2, 1, 0, 1, 2, 3, 		
	5, 8, 12, 13, 13, 11, 9, 7, 7, 7, 7, 8, 9, 9, 10, 11, 12, 12, 11, 8, 		
	6, 4, 1, 1, 1, 2, 4, 6, 9, 11, 11, 10, 7, 5, 4, 5, 6, 6, 5, 4, 		
	4, 5, 7, 11, 14, 16, 16, 16, 15, 14, 13, 13, 12, 11, 9, 5, 1, 0, 0, 0, 		
	0, 0, 0, 3, 5, 5, 4, 4, 6, 7, 8, 8, 8, 8, 9, 11, 13, 15, 16, 16, 		
	16, 16, 15, 15, 15, 16, 16, 16, 16, 13, 9, 6, 3, 0, 0, 0, 0, 0, 3, 7, 		
	8, 7, 4, 2, 1, 2, 4, 5, 6, 6, 6, 7, 7, 8, 10, 13, 16, 16, 16, 16, 		
	16, 16, 14, 10, 7, 5, 6, 8, 10, 12, 13, 12, 9, 5, 0, 0, 0, 0, 0, 0, 		
	0, 0, 3, 8, 13, 16, 16, 16, 16, 16, 14, 11, 9, 7, 4, 0, 0, 0, 0, 0, 		
	0, 0, 2, 8, 12, 13, 13, 14, 15, 16, 16, 16, 16, 13, 13, 14, 16, 16, 16, 16, 		
	16, 13, 9, 7, 7, 8, 10, 12, 14, 14, 12, 10, 8, 7, 7, 6, 6, 6, 6, 7, 		
	7, 7, 5, 3, 1, 1, 3, 5, 7, 8, 9, 10, 12, 15, 16, 16, 16, 16, 16, 16, 		
	16, 16, 16, 12, 7, 3, 1, 1, 3, 5, 8, 11, 14, 16, 14, 10, 6, 1, 0, 0, 		
	0, 0, 0, 2, 5, 8, 9, 10, 10, 11, 12, 11, 7, 3, 1, 1, 2, 1, 2, 3, 		
	4, 4, 4, 5, 5, 5, 4, 4, 2, 1, 1, 2, 4, 6, 10, 14, 16, 16, 16, 14, 		
	11, 9, 9, 9, 8, 6, 5, 3, 3, 6, 10, 15, 16, 16, 16, 16, 16, 16, 16, 13, 		
	11, 9, 8, 6, 6, 7, 8, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 		
	9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 9, 9, 9, 8, 8, 8, 8, 		
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 7, 7, 8, 7, 7, 7, 		
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 		
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 5, 5, 4, 4, 7, 		
	10, 11, 11, 10, 8, 5, 4, 3, 3, 3, 4, 5, 9, 13, 16, 16, 16, 16, 16, 15, 		
	9, 5, 4, 4, 6, 6, 5, 4, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 		
	4, 10, 16, 16, 16, 16, 16, 15, 15, 14, 13, 11, 8, 6, 4, 3, 3, 3, 2, 0, 		
	0, 2, 5, 7, 7, 6, 5, 4, 3, 3, 4, 5, 6, 8, 9, 11, 13, 16, 16, 16, 		
	16, 16, 16, 16, 16, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0, 4, 6, 7, 6, 6, 		
	6, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 6, 3, 2, 0, 		
	0, 2, 3, 5, 7, 9, 11, 13, 14, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 		
	7, 1, 0, 0, 0, 0, 0, 1, 6, 8, 8, 6, 3, 1, 0, 0, 0, 0, 0, 0, 		
	1, 2, 2, 0, 0, 1, 4, 7, 8, 8, 7, 6, 8, 11, 14, 15, 14, 12, 10, 9, 		
	7, 6, 6, 6, 6, 6, 6, 6, 4, 2, 1, 2, 5, 10, 13, 13, 13, 11, 11, 11, 		
	11, 10, 8, 8, 9, 10, 10, 10, 11, 14, 14, 14, 14, 14, 13, 11, 8, 6, 4, 2, 		
	1, 1, 1, 1, 2, 2, 3, 3, 2, 2, 3, 4, 6, 8, 10, 9, 9, 8, 8, 8, 		
	7, 7, 7, 7, 7, 7, 7, 6, 5, 7, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 		
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 		
	8, 8, 7, 7, 7, 7, 8, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 		
	8, 8, 8, 7, 7, 7, 7, 7, 6, 5, 4, 5, 7, 9, 10, 11, 12, 11, 10, 9, 		
	8, 7, 7, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 		
	7, 7, 8, 7, 7, 7, 7, 7, 7, 7, 5, 3, 2, 2, 4, 6, 8, 8, 9, 9, 		
	8, 8, 9, 11, 13, 15, 16, 16, 16, 16, 15, 13, 11, 8, 5, 4, 4, 3, 3, 3, 		
	5, 7, 8, 8, 9, 9, 8, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 4, 		
  7, 11, 14, 15, 14, 13, 11, 9, 8, 7, 7, 6, 6, 6, 6, 7, 7, 7, 8, 6}; 	

	