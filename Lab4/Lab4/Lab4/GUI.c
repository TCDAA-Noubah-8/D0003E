/*
 * GUI.c
 *
 * Created: 3/2/2020 9:40:08 PM
 *  Author: noor_
 */ 

#include "PulseGenerator.h"
#include "GUI.h"
#include "LCD.h"
#include "TinyTimber.h"
#include "AsyncHandler.h"
#include <avr/io.h>
#include <avr/iom169p.h>
#include <avr/interrupt.h>
#include <avr/portpins.h>

void changePortB(GUI *self, int arg) { // Upp & ner & otryckt 
	if (((PINB >> 7) & 1) == 0 && !self->ah->held) {	
		ASYNC(self->lcd->currentPulse, decreasePulse, 0);
		ASYNC(self->lcd, updateLCD, 0);
		ASYNC(self->ah, hold, 0);
		} else if (((PINB >> 6) & 1) == 0 && !self->ah->held) {
		ASYNC(self->lcd->currentPulse, increasePulse, 0);
		ASYNC(self->lcd, updateLCD, 0);
		ASYNC(self->ah, hold, 0);
		}  else if (((PINB >> 4) & 1) == 0) {
		ASYNC(self->lcd->currentPulse, saveState, 0);
		ASYNC(self->lcd, updateLCD, 0);
	}
}

void changePortE(GUI *self, int arg) { // PortE höger & Vänster
	if (((PINE >> 3) & 1) == 0) {									
		ASYNC(self->lcd, change, 6);
		ASYNC(self->lcd, updateLCD, 0);
		} else if (((PINE >> 2) & 1) == 0) {							
		ASYNC(self->lcd, change, 4);
		ASYNC(self->lcd, updateLCD, 0);
	}
}
