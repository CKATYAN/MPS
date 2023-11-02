/*
 * laba4.c
 *
 * Created: 01-Nov-23 4:26:00 PM
 * Author : new
 */

#include "FreeRTOS.h"
#include "task.h"
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


void vClockHandler (void *pvParameters);

int main(void)
{
	xTaskCreate(vClockHandler, (char*) "Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
    /* Replace with your application code */
    while (1)
    {
    }
}

void vApplicationIdleHook() {} // ???????? ??? idle ??????

void vClockHandler(void *pvParameters) 
{ 
    initTimerCounter(); 

    LCDInit(); 
    nullifyTimeArray(); 
    nullifyDataArray(); 
    while (1) 
    { 
        computePosition(); 
        displayTime(positionOnDisplay); 
        displayData(positionOnDisplay); 
        vTaskDelay(100); 
    } 
}

void initTimerCounter() // ????????????? ??????
{
	TCNT1 = 0;
	TCCR1B |= (1<<CS10) | (1<<CS11) | (0<<CS12);
	TIMSK |= (1 << OCIE1B);
	OCR1BH = 0b00111101;
	OCR1BL = 0b00001001;

	//fast mode: 1 second = 1 hour
	//OCR1BH = 0b00000001;
	//OCR1BL = 0b00000100;
	sei();
}


ISR(TIMER1_COMPB_vect) { // ?????????? ??????-????????
	computeTimeArray();
	TCNT1 = 0;
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