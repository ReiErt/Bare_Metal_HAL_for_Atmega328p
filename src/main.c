/*
 * main.c
 *
 * Created: 09.05.2024 13:16
 * Author : Reilly
 */ 

#define F_CPU 16000000UL

#define EVENT_TIMER1SEC 0x01

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "events.h"

void toggleLED(){
    PORTB ^= (1<<PORTB1);
}

int main(void){
    DDRB |= (1<<PORTB1)|(1<<PORTB0);
    TimerHAL_register_callback(toogleLED);
    TimerHAL_init();
    sei();
    while(1){
        _delay_ms(200);
        PORTB ^= (1<<PORTB0);
    }
}