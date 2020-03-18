/*
 * IncFile1.h
 *
 * Created: 3/2/2020 9:42:17 PM
 *  Author: noor_
 */ 


#ifndef GUI_H_
#define GUI_H_

#include "TinyTimber.h"
#include "PulseGenerator.h"
#include "LCD.h"
#include "AsyncHandler.h"

typedef struct {
	Object super;
	LCD *lcd;
	AsyncHandler *ah;
} GUI;

void changePortB(GUI *self, int arg);
void changePortE(GUI *self, int arg);

#define initGUI(lcd, ah) {initObject(), lcd, ah}

#endif /* GUI_H_ */