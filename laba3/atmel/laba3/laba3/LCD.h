#ifndef LCD_H_
#define LCD_H_

typedef enum _LCDRegister
{
	
	LCD_IR=0,
	LCD_DR
	}_LCDRegister;
	
void LCDWriteByte(_LCDRegister reg, uint8_t data);

void LCDWriteStr(char *data);
void LCDWriteInt(int data);
void LCDWriteFloat(float data);
void LCDWriteBinary(int data);
void LCDWriteTwoComplement(int data);

#endif /* LCD_H_ */