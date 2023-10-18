#include "TCN75.h"

#define  TCN75_ADDRES_PREFIX 0x90

static uint8_t TWIReadWriteByte(_Sequence sequence, uint8_t data);

uint16_t TCN75ReadTemp(uint8_t adres)
{
	adres = TCN75_ADDRES_PREFIX|(adres<<1);
	TWIReadWriteByte(TWI_Sequence_Start,adres);
	TWIReadWriteByte(TWI_Sequence_Body_NAK,0);
	TWIReadWriteByte(TWI_Sequence_Start,(adres|1));
	int temp = (int)TWIReadWriteByte(TWI_Sequence_Body_ACK,0);
	temp = (temp << 8)|TWIReadWriteByte(TWI_Sequence_Body_NAK,0);
	TWIReadWriteByte(TWI_Sequence_Stop,0);

	return temp;
}

uint8_t TWIReadWriteByte(_Sequence sequence, uint8_t data)
{
	switch(sequence)
	{
		case TWI_Sequence_Start:
			TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
			while (!(TWCR &(1<<TWINT)));
			TWDR=data;
			TWCR=(1<<TWEN)|(1<<TWINT);
			while(!(TWCR&(1<<TWINT)));
		break;
		
				case TWI_Sequence_Body_ACK:
					TWDR=data;
					TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);
					while (!(TWCR &(1<<TWINT)));
				break;
				
				case TWI_Sequence_Stop:
					TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
				break;
	}
	
	return TWDR;
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
