/*
 * LCD.c
 *
 * Created: 3/2/2020 9:16:10 PM
 *  Author: noor_
 */ 

#include "LCD.h"
#include "PulseGenerator.h"
#include "TinyTimber.h"
#include <avr/io.h>
#include <ctype.h>

void writeChar(char ch, int pos){
	char mask;
	uint16_t allSCC[10] = {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};
	uint16_t number;
	char nibble = 0x0000;
	char out1 = 0x00, out2= 0x00, out3= 0x00, out4 = 0x00;
	uint16_t charr = 0x0000;
	if ((pos > 5)|(pos < 0)){
		return;
	}
	if(pos%2==0){
		mask = 0xF0;
	}
	else{
		mask = 0x0F;
	}
	
	if(isdigit(ch)){
		number = ch-'0'; //omvandlar typ
	}
	/*else if((ch<0)|(ch>9)){
		return;
	}*/
	else{
		number = ch;
	}
	int mod = pos%2;
	if(mod==1){
		pos-=1; //fixar med index
	}
	
	
	charr = allSCC[number];
	for(int i = 0; i < 4 ; i ++){
		nibble = charr & 0x000f;
		charr = charr >> 4;
		if(i==0){
			out1=nibble;
		}
		if(i==1){
			out2=nibble;
		}
		if(i==2){
			out3=nibble;
		}
		if(i==3){
			out4=nibble;
		}
	}
	
	if(pos==0){
		LCDDR0 = (out1<<(mod*4)) + (LCDDR0&mask);
		LCDDR5 = (out2<<(mod*4))+(LCDDR5&mask);
		LCDDR10 = (out3<<(mod*4))+(LCDDR10&mask);
		LCDDR15 = (out4<<(mod*4))+(LCDDR15&mask);

	}
	if(pos==2){
		LCDDR1 = (out1<<(mod*4))+ (LCDDR1&mask);
		LCDDR6 = (out2<<(mod*4))+ (LCDDR6&mask);
		LCDDR11 = (out3<<(mod*4))+ (LCDDR11&mask);
		LCDDR16 = (out4<<(mod*4))+ (LCDDR16&mask);
	}
	if(pos==4){
		LCDDR2 = (out1<<(mod*4))+ (LCDDR2&mask);
		LCDDR7 = (out2<<(mod*4))+ (LCDDR7&mask);
		LCDDR12 = (out3<<(mod*4))+ (LCDDR12&mask);
		LCDDR17 = (out4<<(mod*4))+ (LCDDR17&mask);
	}
}


void printAt(long num, int pos) {
	//lock(&currMut);
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
	//unlock(&currMut);
	
}

void init(void) {
	//Clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt sources
	EIMSK = 0xC0;
	EIFR = 0xC0;
	PCMSK1 |= 0xD0;
	PCMSK0 |= 0x0C;
	
	PORTB |= 0xD0;
	PORTE |= 0x0C;
	
	// LCD
	LCDCRA = 0xC0;			// LCD ENABLE and LOW POWER WAVEFORM
	LCDCRB = 0xB7;			// AsyncClock, 1/3 Bias, 1/4 Duty, 25 Segments
	LCDFRR = 0x07;			// LCD Clock Divide 32 Hz
	LCDCCR = 0x0F;			// 3.35 Volt
	
	LCDDR13 = 0x1;			// init left pulse
}

void updateLCD(LCD *self, int arg) {
	printAt(self->g1->frequency, 0);
	printAt(self->g2->frequency, 4);
}

void change(LCD *self, int pulse) {
	if (pulse == 4) {
		LCDDR13 = 0x1;
		LCDDR18 = 0x0;
		self->currentPulse = self->g1;
		} else if (pulse == 6) {
		LCDDR13 = 0x0;
		LCDDR18 = 0x1;
		self->currentPulse = self->g2;
	}
}