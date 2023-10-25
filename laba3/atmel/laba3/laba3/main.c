/*
 * GccApplication2.c
 *
 * Created: 18.10.2023 16:52:41
 * Author : student
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "LCD.h"
#include "TCN75.h"
#include "stdbool.h"
#define TCN75ADDRES 0x4
#define MAX_T 0b00000010
#define MIN_T 0b00000001

bool stop_flag = false;

ISR(INT0_vect){ // прерывание кнопки
	stop_flag = true;
	_delay_ms(60);
}

int main(void)
{
    LCDInit();
	DDRB = 0xff;
	PORTB=0;
	
	DDRD = 0; 
	MCUCR = (1<<ISC01) | (0<<ISC00);
	GICR |= (1<<INT0);
	GIFR = (1<<INTF0);
	sei();
	
    while (1) 
    {
		LCDKursorPosihin(0,0);
		LCDWriteStr("Temp = ");
		float TCN75_temp = LCDWriteTwoComplement((int)(TCN75ReadTemp(TCN75ADDRES, MAX_T, MIN_T)));
		LCDWriteStr(";   ");
		LCDKursorPosihin(1,0);
		LCDWriteStr("MAX=");
		LCDWriteInt(MAX_T);
		LCDWriteStr("  ");
		LCDWriteStr("MIN=");
		LCDWriteInt(MIN_T);
		
		if (stop_flag) PORTB = 0xFF;
		else PORTB = 0x00;
		
		_delay_ms(300);
		
    }
}

