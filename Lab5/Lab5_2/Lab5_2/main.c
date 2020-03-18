/*
 * Lab5_2.c
 *
 * Created: 2020-03-16 17:00:45
 * Author : elijam-8
 */ 


#include <avr/io.h>
#include "TinyTimber.h"
#include "Brain.h"
#include "Input.h"
#include "LCD.h"

Brain brain = initBrain(0,0,0,0,0,NULL);
Input inp = initInput(&brain);



int main(void)
{
    init();
	
	INSTALL(&inp, recieiveMessage, IRQ_USART0_RX);
	return TINYTIMBER(&brain, updateLCD, 0);
}

