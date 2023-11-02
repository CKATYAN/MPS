#ifndef LCD_H_
#define LCD_H_

typedef enum _LCDRegister
{
	
	LCD_IR=0,
	LCD_DR
	}_LCDRegister;
	
void LCDInit();
void LCDWriteByte(_LCDRegister reg, uint8_t data);

void LCDWriteStr(char *data);
void LCDWriteInt(int data);
void LCDKursorPosihin(uint8_t row, uint8_t col);

#endif /* LCD_H_ */