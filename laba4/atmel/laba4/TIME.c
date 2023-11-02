/*
 * TIME.c
 *
 * Created: 12.10.2023 14:02:25
 *  Author: acer
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "LCD.h"

uint8_t displayFirstLine[16], count_timer = 0, time[8];

void displayTime(int positionOnDisplay)
{
	LCDWriteByte(LCD_IR, 0b10000000);
	clearDisplayFirstLine();
	timeArrayToDisplayFirstLine(positionOnDisplay);
	showDisplayFirstLine();
}

void clearDisplayFirstLine() // сброс прерывание
{
	for (int i = 0; i < 16; i++)
	{
		displayFirstLine[i] = ' ';
	}
}

void timeArrayToDisplayFirstLine(int positionOnDisplay)
{
	for(int i = 0; i<8; i++)
	{
		// +3 для отображения времени по середине, относительно даты
		displayFirstLine[positionOnDisplay + i + 3] = time[i];
	}
}

void showDisplayFirstLine()
{
	for(int i = 0; i < 16; i++)
	{
		LCDWriteByte(LCD_DR, displayFirstLine[i]);
	}
}

void computeTimeArray() // прерывание секунду к бегущая
{
	time[7]++;
	if (time[7] <= '9')	return;
	
	time[7] = '0';
	time[6]++;
	if (time[6] < '6') return;

	time[6] = '0';
	time[4]++;
	if (time[4] <= '9')	return;
	
	time[4] = '0';
	time[3]++;
	if (time[3] < '6') return;
	
	time[3] = '0';
	time[1]++;
	if (time[0] == '2' && time[1] == '4')
	{
		nullifyTimeArray();
		computeDay();	
	}
	if (time[1] <= '9')	return;
	
	time[1] = '0';
	time[0]++;
}

void nullifyTimeArray()
{
	time[0] = '0';
	time[1] = '0';
	time[2] = ':';
	time[3] = '0';
	time[4] = '0';
	time[5] = ':';
	time[6] = '0';
	time[7] = '0';
}

void changePositionTime(int position, int changing)
{
	switch(position)
	{
		case 0:
			time[position] += changing;
			if (time[position] > '2' || time[1] > '3' && time[position] > '1')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
			{ 
				if (time[1] < '4')
					time[position] = '2';
				else
					time[position] = '1';
			}
			break;
		case 1:
			time[position] += changing;
			if (time[position] > '9' || time[0] == '2' && time[position] > '3')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
			{
				if (time[0] < '2')
					time[position] = '9';
				else
					time[position] = '3';
			}
			break;
		case 3:
			time[position] += changing;
			if (time[position] > '5')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
				time[position] = '5';
			break;
		case 4:
			time[position] += changing;
			if (time[position] > '9')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
				time[position] = '9';
			break;
		case 6:
			time[position] += changing;
			if (time[position] > '5')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
				time[position] = '5';
			break;
		case 7:
			time[position] += changing;
			if (time[position] > '9')
			{
				time[position] = '0';
				return;
			}
			if (time[position] < '0')
				time[position] = '9';
			break;
	}
}