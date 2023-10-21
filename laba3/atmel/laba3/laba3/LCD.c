#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>

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

void LCDWriteFloat(float data)
{
	char str[100];

	char *sign = (data < 0) ? "-" : "";
	data = fabs(data);
	
	int integerPart = (int)data;
	float floatPart = data - integerPart;
	int hiddenFloat = trunc(floatPart * 100);

	sprintf (str, "%s%d.%02d", sign, integerPart, hiddenFloat);
	for (int i = 0; i < strlen(str); i++)
	{
		LCDWriteByte(LCD_DR, str[i]);
	}
}

void LCDWriteInt(int data)
{
	char str[10];
	sprintf(str, "%d", data);
	for (int i = 0; i < strlen(str); i++)
	{
		LCDWriteByte(LCD_DR, str[i]);
	}
}

void LCDWriteBinary(int data)
{
	for (int i = 0; i < 12; i++)
	{
		char bit = (data & 0x8000) ? '1' : '0';
		data <<= 1;

		LCDWriteByte(LCD_DR, bit);
		if (i == 7) 
		{
			LCDWriteByte(LCD_DR, '.');
		}
	}
}

void LCDWriteTwoComplement(int data)
{
	data >>= 4;
	int isNegative = data & 0x800;
	
	if (isNegative)
	{
		data--;
		data = (~data) & 0xFFF;
	}

	float floatPart = 0.0;
	for (int i = -4; i < 0; i++)
	{
		floatPart += (data & 1) * pow(2, i);
		data >>= 1;
	}
	int integerPart = data & 0xFF;

	float value = integerPart + floatPart;
	if (isNegative) 
	{
		value = -value;
	}

	LCDWriteFloat(value);
}