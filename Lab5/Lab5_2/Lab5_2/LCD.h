/*
 * LCD.h
 *
 * Created: 2020-03-16 17:02:15
 *  Author: elijam-8
 */ 
#ifndef LCD_H_
#define LCD_H_

#include "TinyTimber.h"
#include "Brain.h"
#include <avr/io.h>

/*typedef struct{
	Object super;
	
}LCD;*/

#define initLCD() {initObject()}*/

void writeChar(char ch, int pos);
void printAt(long num, int pos);
void init(void);
void updateLCD(Brain *b, int d);

#endif