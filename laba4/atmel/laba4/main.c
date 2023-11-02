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

#include "LCD.h"
#include "TIME.h"
#include "DATA.h"

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

	int positionOnDisplay = 0;
	while (1)
	{
		displayData(positionOnDisplay);
		displayTime(positionOnDisplay);
	}
	vTaskDelete(NULL);
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