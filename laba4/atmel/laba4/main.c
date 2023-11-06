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
int counter = 0;
int tick = 0;





void init_timer(){
	    // Установите режим таймера и предделитель
    TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);
    
    // Установите начальное значение счетчика
    TCNT0 = 0;
    
    // Включите прерывание по переполнению таймера
    TIMSK |= (1 << TOIE0);
    
    // Разрешите глобальные прерывания
    sei();
}

ISR(TIMER0_OVF_vect) {
    // Ваш код обработки прерывания
    //computeTimeArray();
    // Очистите флаг прерывания
    TIFR |= (1 << TOV0);
}










void vDisplayClockTask (void *pvParameters);

int main(void)
{
	xTaskCreate(vDisplayClockTask, (char*) "DisplayClockTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
    /* Replace with your application code */
    while (1)
    {
    }
}

void vApplicationIdleHook() {} // empty func to rid of idle-thread

void vDisplayClockTask(void *pvParameters)
{	
    LCDInit(); 
    nullifyTimeArray(); 
	nullifyDataArray();

    while (1) 
    {
		
		init_timer();
		//vTaskStartScheduler();
		
		
		
		
        computePosition(); 
        displayTime(positionOnDisplay); 
        displayData(positionOnDisplay);
		counter = xTaskGetTickCount();
		tick = counter % 1000;
		//if(tick >= 0 && tick <= 45)
		//{
			//computeTimeArray();
			//// 1 - why this does not work (too fast?)
			//// 2 - why pdMS_TO_TICK give me invalid value
		//}
		//
		
	
		
    }
	vTaskDelete(NULL);
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