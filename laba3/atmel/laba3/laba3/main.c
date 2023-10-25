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
#define TCN75ADDRES 0x4
#define MAX_T 0b00000010
#define MIN_T 0b00000001




int main(void)
{
    LCDInit();
	DDRB=0xff;
	PORTB=0;
    while (1) 
    {
		LCDKursorPosihin(0,0);
		LCDWriteStr("Temp = ");
		float TCN75_temp = LCDWriteTwoComplement((int)(TCN75ReadTemp(TCN75ADDRES, MAX_T, MIN_T)));
		LCDWriteStr(";   ");
		LCDKursorPosihin(1,0);
		LCDWriteStr("MAX=");
		LCDWriteInt(MAX_T);
		LCDWriteStr("   ");
		LCDWriteStr("MIN=");
		LCDWriteInt(MIN_T);
		if (TCN75_temp > MAX_T)
			PORTB = 0xFF;
		else
			PORTB = 0;
		_delay_ms(300);
    }
}

