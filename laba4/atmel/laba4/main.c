/*
 * laba4.c
 *
 * Created: 01-Nov-23 4:26:00 PM
 * Author : new
 */

#include "FreeRTOS.h"
#include "task.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "LCD.h"
#include "TIME.h"
#include "DATA.h"

#define LEFT 0b01111111
#define RIGHT 0b10111111
#define CHANGING 0b11011111
#define CHANGING_UP 0b11111101
#define CHANGING_DOWN 0b11111110

int positionOnDisplay = 0;
int shiftCursorOnDisplay;
bool isFirstLine;
bool isGoingLeft = true;
bool stop_flag = false;
bool isMovingToTheRIght;
int flagMoovingSpeed = 0;
int counter = 0;
int tick = 0;

void vDisplayClockTask (void *pvParameters);
void vButtonHandlerTask (void *pvParameters);

int main(void)
{
	xTaskCreate(vDisplayClockTask, (char*) "DisplayClockTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(vButtonHandlerTask, (char*) "ButtonHandlerTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
    /* Replace with your application code */
    while (1)
    {
    }
}

void vApplicationIdleHook() {} // empty func to rid of idle-thread

void initStopButton()
{
	DDRD = 0;
	MCUCR = (1<<ISC01) | (1<<ISC00);
	GICR |= (1<<INT0);
	GIFR = (1<<INTF0);
}

void vButtonHandlerTask(void *pvParameters)
{
	initStopButton();
	while(1)
	{
		vTaskDelay(1);
	}
}

void init_timer(){
	TCNT1 = 0;
	TCCR1B |= (1<<CS10) | (1<<CS11) | (0<<CS12);
	TIMSK |= (1 << OCIE1B);
	OCR1BH = 0b00111101;
	OCR1BL = 0b00001001;
	sei();
}

ISR(TIMER1_COMPB_vect) {
	computeTimeArray();
	TCNT1 = 0;
}

void vDisplayClockTask(void *pvParameters)
{	
    LCDInit(); 
    nullifyTimeArray(); 
	nullifyDataArray();
	init_timer();
    while (1) 
    {
        displayData(positionOnDisplay);
        displayTime(positionOnDisplay);
        if (!stop_flag)
        {
	        computePosition();
        }
        else
        {
	        if (PIND == CHANGING_UP)
	        {
		        cli();
		        GICR &= (0<<INT0);
		        changingMode();
	        }
        }
    }
	vTaskDelete(NULL);
}

void togglePosition()
{
	if (!isFirstLine && shiftCursorOnDisplay > 2 && shiftCursorOnDisplay < 6)
	{
		toggleMonth();
		return;
	}
	LCDKursorPosihin(!isFirstLine, shiftCursorOnDisplay + positionOnDisplay);
	LCDWriteByte(LCD_DR, ' ');
	_delay_ms(150);
	if (isFirstLine)
	displayTime(positionOnDisplay);
	else
	displayData(positionOnDisplay);
	_delay_ms(150);
}

void toggleMonth()
{
	LCDKursorPosihin(1, positionOnDisplay+3);
	for (int i = 0; i < 3; i++)
	LCDWriteByte(LCD_DR, ' ');
	_delay_ms(200);
	displayData(positionOnDisplay);
	_delay_ms(200);
}

void changingMode()
{
	isFirstLine = true;
	shiftCursorOnDisplay = 3;
	isMovingToTheRIght = true;
	while(1)
	{
		togglePosition();
		switch(PIND)
		{
			case CHANGING:
			sei();
			initStopButton();
			return 0;
			break;
			case LEFT:
			isMovingToTheRIght = false;
			shiftCursorOnDisplay--;
			checkCursorPosition();
			_delay_ms(300);
			break;
			case RIGHT:
			isMovingToTheRIght = true;
			shiftCursorOnDisplay++;
			checkCursorPosition();
			_delay_ms(300);
			break;
			case CHANGING_UP:
			if (isFirstLine)
			changePositionTime(shiftCursorOnDisplay - 3, 1);
			else
			changePositionData(shiftCursorOnDisplay, 1);
			_delay_ms(300);
			break;
			case CHANGING_DOWN:
			if (isFirstLine)
			changePositionTime(shiftCursorOnDisplay - 3, -1);
			else
			changePositionData(shiftCursorOnDisplay, -1);
			_delay_ms(300);
			break;
			default:break;
		}
		
		
	}
}

void checkCursorPosition()
{
	if (isFirstLine)
	{
		switch(shiftCursorOnDisplay-3)
		{
			case -1:
			changeLine();
			break;
			case 2:
			shiftFromColon();
			break;
			case 5:
			shiftFromColon();
			break;
			case 8:
			changeLine();
			break;
			default:
			return;
		}
	}
	else
	switch(shiftCursorOnDisplay)
	{
		case -1:
		changeLine();
		break;
		case 2:
		shiftFromColon();
		break;
		case 4:
		shiftFromMonth();
		break;
		case 5:
		shiftFromMonth();
		break;
		case 6:
		shiftFromColon();
		break;
		case 11:
		changeLine();
		break;
		default:
		return;
	}
}

void shiftFromMonth()
{
	if (isMovingToTheRIght)
	shiftCursorOnDisplay += 3;
	else
	shiftCursorOnDisplay -= 3;
}

void shiftFromColon()
{
	if (isMovingToTheRIght)
	shiftCursorOnDisplay++;
	else
	shiftCursorOnDisplay--;
}

void changeLine()
{
	isFirstLine = !isFirstLine;
	if (isFirstLine)
	{
		if (isMovingToTheRIght)
		{
			shiftCursorOnDisplay = 3;
			return;
		}
		shiftCursorOnDisplay = 10;
	}
	else
	{
		if (isMovingToTheRIght)
		{
			shiftCursorOnDisplay = 0;
			return;
		}
		shiftCursorOnDisplay = 10;
	}
}

void computePosition()
{
	switch(PIND)
	{
		case CHANGING:
		flagMoovingSpeed = 0;
		break;
		case RIGHT:
		flagMoovingSpeed = 1;
		break;
		case LEFT:
		flagMoovingSpeed = 2;
		break;
	}
	
	switch(flagMoovingSpeed)
	{
		case 0:
		 vTaskDelay(150);
		break;
		case 1:
		 vTaskDelay(500);
		break;
		case 2:
		 vTaskDelay(1000);
		break;
	}
	if (positionOnDisplay == 5 || positionOnDisplay == 0)
	isGoingLeft = !isGoingLeft;

	isGoingLeft ? positionOnDisplay-- : positionOnDisplay++;
}

ISR(INT0_vect)
{
	stop_flag = !stop_flag;
	_delay_ms(60);
}