/*
 * events.h
 *
 * Created: 26.04.2023 18:54
 *  Author: Reilly
 */ 


#ifndef EVENTS_H_
#define EVENTS_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/common.h>

// event functions
void setEvent (uint8_t counter);
uint8_t eventIsSet(uint8_t counter);
void clearEvent(uint8_t counter);

// global variables
volatile uint16_t EventCounter = 0; 
volatile uint8_t timer_triggerd = 0;
volatile uint8_t TimerCounter = 0;			//tells us how many software timers exist
volatile uint8_t StartTimerCounter = 0;		//the timer has a new start time bekommen

// Timer functions
void setTimer(uint8_t timer, uint32_t time);
void startTimer(uint8_t timer);
void cancelTimer(uint8_t timer);
void Timer(uint32_t counter_now);
struct Timer_box{
	uint32_t time;
	uint32_t start_time;
}big_timer_box[5];

// ----------------------------------------------------------------- Events ------------------------------------------------------------------------

void setEvent(uint8_t counter)
{
	unsigned char sreg_old = SREG;
	cli();
	EventCounter = EventCounter | ( 1 << (counter) );// Counter Bit auf 1 setzen
	SREG = sreg_old;
}

void clearEvent(uint8_t counter)
{
	unsigned char sreg_old = SREG;
	cli();
	EventCounter = EventCounter & ( ~ ( 1 << (counter) ) ); // Counter Bit auf Null setzten
	SREG = sreg_old;
}

uint8_t eventIsSet(uint8_t counter)
{
	return( ( EventCounter >> (counter ) & 1 ) == 1 );
}

// ----------------------------------------------------------------- Timer -------------------------------------------------------------------------------------

void Timer(uint32_t counter_now)
{
	uint32_t ntime; // Derzeitige Zeit

	
		unsigned char sreg_old = SREG;
		cli();
	for(int i = 0; i < 5; i++)
	{	
		// checking first four bits from LSB to MSB to see if 
		// TimerCounter is set AND StartTimerCounter is not set
		// if timer exists but has not started, then we note the start time.
		// then we note that that timer has started
				
		//printf("Timercounter %i\n",TimerCounter);
		if( ((TimerCounter >> i) & 1) == 1 && ((StartTimerCounter >> i) & 1) == 0)
		{
			big_timer_box[i].start_time = counter_now;
			//printf("Startzeit wird geholt\n");
			// bestimmtes bit auf 1 setzen um keine neuen Startzeiten zu bekommen
			StartTimerCounter = StartTimerCounter | (1<<i); 
		}
	}
	ntime = counter_now;
	for (int i = 0; i < 5; i++)
	{
		// if a timer has been "set" AND if timer is running AND 
		// if the time the timer has been running for is longer than it should be running, then timer is triggered and starttimercounter is cleared
		if( (big_timer_box[i].time != 0) && ((TimerCounter >> i & 1 ) == 1) &&  ((ntime - big_timer_box[i].start_time) >= big_timer_box[i].time))
		{
			//printf("ntime = %i starttime = %i Zeitabgelaufen nach %i \n",ntime, big_timer_box[0].start_time, (ntime-big_timer_box[0].start_time));
			timer_triggerd |= (1<<i);
			// bestimmtes bit auf 0 setzen um neuen startwert zu holen
			StartTimerCounter = StartTimerCounter & (~(1<<i)); 
		}
	}
	SREG = sreg_old;
}

void setTimer(uint8_t timer, uint32_t time)
{
	unsigned char sreg_old = SREG;
	cli();
	big_timer_box[timer].time = time;
	SREG = sreg_old;
}

void startTimer(uint8_t timer)
{
	unsigned char sreg_old = SREG;
	cli();
	TimerCounter = TimerCounter | (1 << timer); // when starting timer, set bit at TimerCounter
	StartTimerCounter = StartTimerCounter & (~(1<<timer)); // we starting Timer, clear bit at StartTimerCounter
	SREG = sreg_old;
}

void cancelTimer(uint8_t timer)
{
	unsigned char sreg_old = SREG;
	cli();
	TimerCounter = TimerCounter & ( ~ (1 << timer) );
	SREG = sreg_old;
}


#endif /* EVENTS_H_ */