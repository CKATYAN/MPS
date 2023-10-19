#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "LCD.h"

#define LCD_PORT PORTA
#define LCD_RS (1<<4)
#define LCD_E (1<<5)
#define LCD_MASK 0xC0

void LCDInit()
{
	DDRA|=~LCD_MASK;
	LCDWriteByte(LCD_IR, 0x33);
	_delay_ms(100);
	LCDWriteByte(LCD_IR, 0x32);
	_delay_ms(100);
	LCDWriteByte(LCD_IR, 0x28);
	_delay_ms(100);
	LCDWriteByte(LCD_IR, 0x01);
	_delay_ms(100);
	LCDWriteByte(LCD_IR, 0x0c);
	_delay_ms(100);
}

void LCDWriteByte(_LCDRegister reg, uint8_t data)
{
	LCD_PORT&=LCD_MASK;
	
	if(reg)LCD_PORT|=LCD_RS;
	
	LCD_PORT|=data>>4;
	LCD_PORT|=LCD_E;
	_delay_us(40);
	LCD_PORT&=~LCD_E;
	_delay_us(40);
	
	LCD_PORT&=0xf0;
	
	LCD_PORT|=data&0x0f;
	LCD_PORT|=LCD_E;
	_delay_us(40);
	LCD_PORT&=~LCD_E;
	_delay_us(40);
}

void LCDKursorPosihin(uint8_t row, uint8_t col)
{
	if(!row) {
		LCDWriteByte(LCD_IR, (1<<7)|col);
	}
	else{
		LCDWriteByte(LCD_IR, (3<<6)|col);
	}
}

void LCDWriteStr(char *data)
{
	for (int i = 0; i < strlen(data); i++)
		LCDWriteByte(LCD_DR, data[i]);
}

void LCDWriteFloat(float adc_read)
{
	char str[100];

	char *tmpSign = (adc_read < 0) ? "-" : "";
	float tmpVal = (adc_read < 0) ? -adc_read : adc_read;

	int tmpInt1 = tmpVal;                  // Get the integer (678).
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int tmpInt2 = trunc(tmpFrac * 1000);  // Turn into integer (123).

// Print as parts, note that you need 0-padding for fractional bit.

	sprintf (str, "%s%d.%d\n", tmpSign, tmpInt1, tmpInt2);
	for (int i = 0; i < strlen(str); i++)
	{
		LCDWriteByte(LCD_DR, str[i]);
	}
	
}

void LCDWriteInt(int data)
{
	char str[10];
	sprintf (str, "%d", data);
	for (int i = 0; i < strlen(str); i++)
	{
		LCDWriteByte(LCD_DR, str[i]);
	}
}