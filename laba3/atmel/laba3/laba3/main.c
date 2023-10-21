/*
 * GccApplication2.c
 *
 * Created: 18.10.2023 16:52:41
 * Author : student
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "TCN75.h"
#define TCN75ADDRES 0x4

int main(void)
{
    LCDInit();
	
    while (1) 
    {
		//TCN75Config(TCN75ADDRES);
		LCDKursorPosihin(0,0);
		LCDWriteStr("Temp = ");
		LCDWriteBinary((int)(TCN75ReadTemp(TCN75ADDRES)));
		//LCDWriteBinary(0b0000000011000000);
		LCDWriteStr(";   ");
		//LCDWriteFloat(15.75);
		//LCDWriteInt(1150);
		_delay_ms(300);
    }
}

