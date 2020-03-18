/*
 * Lab4.c
 *
 * Created: 3/1/2020 10:09:31 PM
 * Author : noor_
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PulseGenerator.h"
#include "GUI.h"
#include "TinyTimber.h"
#include "LCD.h"
#include "Port.h"

PORT p = initPORT(1);
PulseGenerator g1 = initPulseGenerator(4, 0, 0, 0, &p);
PulseGenerator g2 = initPulseGenerator(6, 0, 0, 0, &p);
LCD lcd = initLCD(&g1, &g2, &g1);
AsyncHandler ah = initAsyncHandler(&lcd);
GUI gui = initGUI(&lcd, &ah);


int main(void) {
	init();
	
	INSTALL(&gui, changePortE, IRQ_PCINT0); // höger & vänster
	INSTALL(&gui, changePortB, IRQ_PCINT1);	// upp & ner & ner tryckt 
	
	return TINYTIMBER(&lcd, updateLCD, 0); // Om något blir fel så return -1
}