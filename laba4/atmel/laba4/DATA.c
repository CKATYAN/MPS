/*
 * display.c
 *
 * Created: 12.10.2023 13:25:29
 *  Author: acer
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"

int displaySecondLine[16], data[11], month = 0;

const char* months[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

const int* days[] = {
	"31",
	"28",
	"31",
	"30",
	"31",
	"30",
	"31",
	"31",
	"30",
	"31",
	"30",
	"31",
};

void nullifyDataArray()
{
	data[0] = '0';
	data[1] = '1';
	data[2] = ' ';
	data[3] = 'J';
	data[4] = 'a';
	data[5] = 'n';
	data[6] = ' ';
	data[7] = '2';
	data[8] = '0';
	data[9] = '2';
	data[10] = '4';
}

void setMonthDataArray()
{
	const char* monthStr = months[month];
	for (int i = 0; i < 3; i++)
	{
		data[i + 3] = monthStr[i];
	}
}

void displayData(int positionOnDisplay)
{
	clearDisplaySecondLine();
	dataArrayToDisplaySecondLine(positionOnDisplay);
	showDisplaySecondLine();
}

void clearDisplaySecondLine()
{
	for (int i = 0; i < 16; i++)
	{
		displaySecondLine[i] = ' ';
	}
}

void dataArrayToDisplaySecondLine(int positionOnDisplay)
{
	for(int i = 0; i<11; i++)
	{
		displaySecondLine[positionOnDisplay + i] = data[i];
	}
}

void showDisplaySecondLine()
{
	LCDKursorPosihin(1, 0);
	for (int i = 0; i < 16; i++)
	{
		LCDWriteByte(LCD_DR, displaySecondLine[i]);
	}
}

int getYearFromData()
{
	char str[5] = "";
	for (int i = 0; i < 4; i++)
		str[i] = data[7 + i];
	str[4] = '\0';
	
	char* endPtr;
	int year = strtol(str, &endPtr, 10);
	if (*endPtr != '\0') return 0;
	
	return year;
}

bool isLeapYear()
{
	int year = getYearFromData();
	if (year % 4 != 0) return false;
	
	return (year % 100 == 0 && year % 400 != 0) ? false : true;
}

const char* findDayMonth()
{
	return (month == 1 && isLeapYear()) ? "29" : days[month];
}

void computeDay()
{
	const char* day;
	day = findDayMonth();
	data[1]++;
	if((data[1] > day[1]) && data[0] == day[0])
	{
		data[0] = '0';
		data[1] = '1';
		month++;
		if (month == 12)
		{
			month = 0;
			int year = getYearFromData();
			char str[5];
			year++;
			sprintf(str, "%d", year);
			for (int i = 0; i < 4; i++)
			data[i+7] = str[i];
			if (year > 9999)
			nullifyDataArray();
		}
		setMonthDataArray();
	}
	else if (data[1] > '9')
	{
		data[0]++;
		data[1] = '0';
		
	}
}

void changePositionData(int position, int changing)
{
	const char* day;
	switch(position)
	{
		case 0:
			day = findDayMonth();
			data[position] += changing;
			if (data[position] > day[0] || data[1] > day[1] && data[position] >= day[0])
			{
				data[position] = '0';
				return;
			}
			if (data[position] < '0')
			{
				if (data[1] <= day[1])
					data[position] = day[0];
				else
					data[position] = day[0] - 1;
			}
			break;
		case 1:
			day = findDayMonth(); 
			data[position] += changing;
			if (data[position] > '9' || data[0] == day[0] && data[position] > day[1])
			{	
				if (data[0] != '0')
					data[position] = '0';
				else 
					data[position] = '1';
				return;
			}
			if (data[position] < '0' && data[0] != 0)
			{
				data[position] = day[1];
				return;
			}
			if (data[position] < '1')
			{
				if (data[0] < day[0])
					data[position] = '9';
				else
					data[position] = day[1];
				return;
			}
			break;
		case 3: 
			changeMonth(day, changing);
			break;
		case 5:
			changeMonth(day, changing); 
			break;
		case 7:
			changeYear(position, changing, day);
			break;
		case 8:
			changeYear(position, changing, day);
			break;
		case 9:
			changeYear(position, changing, day);
			break;
		case 10:
			changeYear(position, changing, day);
			break;
	}
}

void changeYear(int position, int changing, const char* day)
{
	data[position] += changing;
	if (data[position] > '9')
	{
		data[position] = '0';
		return;
	}
	if (data[position] < '0')
		data[position] = '9';
		day = findDayMonth();
			if (data[0] > day[0] || data[0] == day[0] && data[1] > day[1])
			{
				data[0] = day[0];
				data[1] = day[1];
			}
}

void changeMonth(const char* day, int changing)
{
	month += changing;
	if (month > 11)
		month = 0;
	if (month < 0)
		month = 11;
	day = findDayMonth();
	if (data[0] > day[0] || data[0] == day[0] && data[1] > day[1])
	{
		data[0] = day[0];
		data[1] = day[1];
	}
	setMonthDataArray();
}