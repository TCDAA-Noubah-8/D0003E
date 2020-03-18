#include "tinythreads.h"
#include <stdbool.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/portpins.h>

mutex blinkM = MUTEX_INIT;
mutex buttonM = MUTEX_INIT;
long btnCount = 0;

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
		number = ch-'0';
	}
	else if((ch<0)|(ch>9)){
		return;
	}
	else{
		number = ch;
	}
	int mod = pos%2;
	if(mod==1){
		pos-=1;
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

bool is_prime(long i){
	if(i>0){
		for(int j = 2;j<i;j++){
			if((i%j)==0){
				return false; //inte prime
			}
		}
	}
	return true;
}
int pp;

void printAt(long num, int pos) {
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
	
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
			//yield();
        }
    }
}

void blink(void) {

	while(1){
		lock(&blinkM);
			//tända segment
		if(LCDDR13 != 0) { // om segment är på så stänger vi av denna
			LCDDR13 = 0;
		} else {
			LCDDR13 = 0x01; //sätter på den om den var släckt
		}
	}
}

void button(int pos) {
	while(1){
		lock(&buttonM);
		printAt(btnCount, pos);
	}
}

void init(void) {
	TCCR1A = (1 << COM1A1) | (1 << COM1A0); 
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); 
	OCR1A = 8000000/2048;
	TCNT1 = 0x0;
	TIMSK1 = (1 << OCIE1A); 
}

int main() {
	init();
	
    spawn(blink, 0);
    spawn(button, 4);
	computePrimes(0);
}




