/*
 * GccApplication2.c
 *
 * Created: 18.10.2023 16:52:41
 * Author : student
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#define TCN75ADDRES 0x7

int main(void)
{
    LCDInit();
	
    while (1) 
    {
		LCDKursorPosihin(0,0);
//		LDCWriteStr("Temp = ");
		
//		LCDWriteInt((int)(TCN75ReadTemp(TCN75ADDRES)>>8));
		LCDWriteStr(";   ");
		LCDWriteFloat(15.75);
		LCDWriteInt(1150);
		_delay_ms(500);
    }
}

