/*
 * AsyncHandler.c
 *
 * Created: 3/2/2020 9:27:48 PM
 *  Author: noor_
 */ 

#include "AsyncHandler.h"
#include "TinyTimber.h"
#include "PulseGenerator.h"
#include "LCD.h"
#include <avr/io.h>

void hold(AsyncHandler *self, int arg) {
	if (self->held == 0) {
		self->held = 1;
		AFTER(MSEC(500), self, hold, 0);
		} else if (((PINB >> 6) & 1) == 0) {
		SYNC(self->lcd->currentPulse, increasePulse, 0);
		SYNC(self->lcd, updateLCD, 0);
		AFTER(MSEC(200), self, hold, 0);
		} else if (((PINB >> 7) & 1) == 0) {
		SYNC(self->lcd->currentPulse, decreasePulse, 0);
		SYNC(self->lcd, updateLCD, 0);
		AFTER(MSEC(200), self, hold, 0);
		} else {
		self->held = 0;
	}
}