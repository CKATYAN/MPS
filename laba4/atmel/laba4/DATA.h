/*
 * DATA.h
 *
 * Created: 12.10.2023 13:29:32
 *  Author: acer
 */ 

#include <stdbool.h>
#ifndef DATA_H_
#define DATA_H_

void clearDisplaySecondLine();
void nullifyDataArray();
void setMonthToData();
void dataArrayToDisplaySecondLine(int positionOnDisplay);
void displayData(int positionOnDisplay);
void showDisplaySecondLine();
int getYearFromData();
bool isLeapYear();
const char* findDayMonth();
void computeDay();
void changePositionData(int position, int changing);

#endif /* DATA_H_ */