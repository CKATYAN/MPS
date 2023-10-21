#include "TCN75.h"
#include <stdint.h>

#define TCN75_ADDRES_PREFIX 0x90

static uint8_t TWIReadWriteByte(_Sequence sequence, uint8_t data);

void TCN75Config(uint16_t addres)
{	
	TWIReadWriteByte(TWI_Sequence_Start, addres);
	TWIReadWriteByte(TWI_Sequence_Body_ACK,0b00000001);
	TWIReadWriteByte(TWI_Sequence_Body_NAK,0b01000000);
	TWIReadWriteByte(TWI_Sequence_Stop,0);
}

uint16_t TCN75ReadTemp(uint8_t addres)
{	
	addres=TCN75_ADDRES_PREFIX|(addres<<1);
	TCN75Config(addres);
	
	TWIReadWriteByte(TWI_Sequence_Start,addres);
	TWIReadWriteByte(TWI_Sequence_Body_NAK,0);
	TWIReadWriteByte(TWI_Sequence_Start,(addres|1));
	int temp=(int)TWIReadWriteByte(TWI_Sequence_Body_ACK,0);
	temp=(temp<<8)|TWIReadWriteByte(TWI_Sequence_Body_NAK,0);
	TWIReadWriteByte(TWI_Sequence_Stop,0);
	
	return temp;
}

uint8_t TWIReadWriteByte(_Sequence sequence, uint8_t data)
{
	switch(sequence)
	{
		case TWI_Sequence_Start:
		TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
		while(!(TWCR &(1<<TWINT)));
		TWDR = data;
		TWCR =(1<<TWEN)|(1<<TWINT);
		while(!(TWCR&(1<<TWINT)));
		break;
		
		case TWI_Sequence_Body_ACK:
		TWDR = data;
		TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);
		while(!(TWCR&(1<<TWINT)));
		break;
		
		case TWI_Sequence_Body_NAK:
		TWDR= data;
		TWCR=(1<<TWEN)|(1<<TWINT);
		while(!(TWCR&(1<<TWINT)));
		break;
		
		case TWI_Sequence_Stop:
		TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
		break;
	}
	return TWDR;
}
