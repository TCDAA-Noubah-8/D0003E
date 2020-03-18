/*
 * Port.c
 *
 * Created: 3/2/2020 9:43:58 PM
 *  Author: noor_
 */ 


#include "PulseGenerator.h"
#include "TinyTimber.h"
#include "Port.h"
#include <avr/io.h>


void porting(PORT *self, int portBit) { //race condition
	if (portBit == 4) { // Pulse generator 1
		PORTE ^= 0x10; 
		} else if (portBit == 6) { // Pulse generator 2 
		PORTE ^= 0x40; 
	}
}

