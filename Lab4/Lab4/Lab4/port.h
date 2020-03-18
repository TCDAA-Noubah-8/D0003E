/*
 * port.h
 *
 * Created: 3/2/2020 9:44:50 PM
 *  Author: noor_
 */ 


#ifndef PORT_H_
#define PORT_H_

#include "PulseGenerator.h"
#include "TinyTimber.h"

typedef struct {
	Object super;
	int currentPulse;
} PORT;

void porting(PORT *self, int portBit);

#define initPORT(pulse) {initObject(), pulse}

#endif /* PORT_H_ */