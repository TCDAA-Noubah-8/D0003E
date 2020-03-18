/*
 * AsyncHandler.h
 *
 * Created: 3/2/2020 9:27:10 PM
 *  Author: noor_
 */ 


#ifndef ASYNCHANDLER_H_
#define ASYNCHANDLER_H_

#include "PulseGenerator.h"
#include "LCD.h"

typedef struct {
	Object super;
	LCD *lcd;
	int held;
} AsyncHandler;

void hold(AsyncHandler *self, int arg);

#define initAsyncHandler(lcd) {initObject(), lcd}

#endif /* ASYNCHANDLER_H_ */