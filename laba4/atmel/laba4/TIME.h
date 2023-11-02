/*
 * TIME.h
 *
 * Created: 12.10.2023 14:02:10
 *  Author: acer
 */ 


#ifndef TIME_H_
#define TIME_H_

void displayTime(int positionOnDisplay);
void clearDisplayFirstLine(); 
void timeArrayToDisplayFirstLine(int positionOnDisplay);
void showDisplayFirstLine();
void computeTimeArray();
void nullifyTimeArray();
void changePositionTime(int position, int changing);

#endif /* TIME_H_ */