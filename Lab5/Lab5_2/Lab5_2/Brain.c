/*
 * Brain.c
 *
 * Created: 2020-03-16 17:26:43
 *  Author: elijam-8
 */ 

#include "Brain.h"
#include "LCD.h"
#include "TinyTimber.h"
#include "avr/io.h"
#define MAX 6
/*
void onBridge(Brain *brain, int d){
	brain->queueBridge--;
}

void decode(Brain *brain, int inp){
	if((inp&1)==1){
		brain->queueNorth++;
	}
	else if(((inp<<1)&1)==1){
		brain->queueNorth--;
		brain->queueBridge++;
		AFTER(SEC(5),brain,onBridge,0);
	}
	else if((((inp<<2)&1)==1)){
		brain->queueSouth++;
	}
	else if((((inp<<3)&1)==1)){
		brain->queueSouth++;
		brain->queueBridge++;
		AFTER(SEC(5),brain,onBridge,0);
	}
	ASYNC(brain,updateLCD,0);
	ASYNC(brain,logicOfLight,inp);
}

void logicOfLight(Brain *brain, int i){
	if(brain->queueNorth==0&&brain->queueSouth==0){//No cars on bridge
		 ASYNC(brain, output, 0xA); //Send red lights to North/South
	}
	else if(brain->queueNorth>0&&brain->queueSouth==0){//Cars on North but not South
		ASYNC(brain, output, 0x9); //Send green to North and red to South
	}
	else if(brain->queueNorth==0&&brain->queueSouth>0){//Cars not on North but on Cars South
		ASYNC(brain, output, 0x6);//Send red to North and green to South
	}
	AFTER(SEC(1),brain,logicOfLight,0);
}

void output(Brain *b, int o){
	UDR0 = o;
}*/

void onBridge(Brain *brain, int d){
	brain->queueBridge--;
	ASYNC(brain,updateLCD,0);
}

void decode(Brain *brain, int inp){
	ASYNC(brain,updateLCD,0);
	if((inp&1)==1){
		brain->queueNorth++;
	}
	else if(((inp>>1)&1)==1){
		brain->queueNorth--;
		brain->queueBridge++;
		brain->queuePassed++;
		ASYNC(brain,updateLCD,0);
		AFTER(SEC(5),brain,onBridge,0);
	}
	else if((((inp>>2)&1)==1)){
		brain->queueSouth++;
	}
	else if(((inp>>3)&1)==1){
		brain->queueSouth--;
		brain->queueBridge++;
		brain->queuePassed++;
		ASYNC(brain,updateLCD,0);
		AFTER(SEC(5),brain,onBridge,0);
	}
	if(brain->queuePassed>=MAX && (brain->queueNorth!=0&&brain->queueSouth!=0)){
		ASYNC(brain, output, 0xA);
		brain->queuePassed=0;
		if(brain->queueOther == 1){
			brain->queueOther = 0;
			AFTER(SEC(5),brain,output,0x9);
		}
		else if(brain->queueOther == 0){
			brain->queueOther = 1;
			AFTER(SEC(5),brain,output,0x6);
		}	
	}
	//ASYNC(brain,updateLCD,0);
	ASYNC(brain,logicOfLight,0);
}

void logicOfLight(Brain* brain, int i){
	if(brain->queueNorth==0&&brain->queueSouth==0){//No cars on bridge
		brain->queueOther = NULL;
		ASYNC(brain, output, 0xA); //Send red lights to North/South
	}
	else if(brain->queueNorth>0&&brain->queueSouth==0){//Cars on North but not South
		brain->queueOther = 0;
		ASYNC(brain, output, 0x9); //Send green to North and red to South
	}
	else if(brain->queueNorth==0&&brain->queueSouth>0){//Cars not on North but on Cars South
		brain->queueOther = 1;
		ASYNC(brain, output, 0x6);//Send red to North and green to South
	}
}

void output(Brain *b, int o){
	ASYNC(b,updateLCD,0);
	UDR0 = o;
}