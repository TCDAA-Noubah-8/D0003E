/*
 * Input.c
 *
 * Created: 2020-03-16 17:51:39
 *  Author: elijam-8
 */ 

#include "Input.h"
#include "LCD.h"

void recieiveMessage(Input* inp){
	int d = UDR0;
	ASYNC(inp->brain, decode, d);
}

