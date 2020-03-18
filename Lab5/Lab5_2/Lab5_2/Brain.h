/*
 * Brain.h
 *
 * Created: 2020-03-16 17:26:52
 *  Author: elijam-8
 */ 

#ifndef BRAIN_H_
#define BRAIN_H_

#include "TinyTimber.h"
#include "avr/io.h"
//#include "LCD.h"

typedef struct{
	Object super;
	int queueNorth;
	int queueSouth;
	int queuePassed;
	int queueBridge;
	int queueOther;
	//LCD *lcd;
} Brain;

void logicOfLight(Brain *b, int i);
void input(Brain *b, int i);
void output(Brain *b, int o);
void decode(Brain *b, int o);
void onBridge(Brain *b, int d);

#define initBrain(north, south, passed, on, bridge, other) {north, south, passed, on, bridge, other}

#endif