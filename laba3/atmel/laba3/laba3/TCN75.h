/*
 * TCN75.h
 *
 * Created: 18.10.2023 16:55:23
 *  Author: student
 */ 


#ifndef TCN75_H_
#define TCN75_H_

#include <avr/io.h>
#include "LCD.h"

typedef enum _Sequence
{
	TWI_Sequence_Start=0,
	TWI_Sequence_Body_ACK,
	TWI_Sequence_Body_NAK,
	TWI_Sequence_Stop
}_Sequence;

uint16_t TCN75ReadTemp(uint8_t adres);
#endif
