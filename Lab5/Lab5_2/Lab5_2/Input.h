/*
 * Input.h
 *
 * Created: 2020-03-16 17:51:49
 *  Author: elijam-8
 */ 
#ifndef INPUT_H_
#define INPUT_H_

#include "Brain.h"
#include "TinyTimber.h"
#include <avr/io.h>

typedef struct{
	Object super;
	Brain *brain;
}Input;

void recieiveMessage(Input* inp);

#define initInput(cont) {initObject(), cont};

#endif