#include "tinythreads.h"
#include <stdbool.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/portpins.h>

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
mutex currMut = MUTEX_INIT;
void printAt(long num, int pos) {
	lock(&currMut);
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
	unlock(&currMut);
	
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
	TCCR1B = (1<<CS12); //256 prescaler
	uint16_t boundary = 8000000/256; //8MHz/prescaler (borde vara ungefär 1 sec) (31250 cycler av 0xFFFF)
	uint16_t wrapstatus = 0; //Off default

	while(1){
		if(TCNT1 >= boundary &&wrapstatus==0 ){
			wrapstatus = 1;
			if(boundary + 8000000/256 > 0xffff){
				boundary= boundary +(8000000/256) -0xffff;//om totalTimer gått över toppen så gör vi wraparound
			}
			else{
				boundary = boundary+8000000/256; //annars sätter på value
			}
			//tända segment
			if(LCDDR13 != 0){ // om segment är på så stänger vi av denna
				LCDDR13 = 0;
				} else{
				LCDDR13 = 0x01; //sätter på den om den var släckt
			}
		}
		if(TCNT1<boundary){
			wrapstatus=0;
		}
	}
}
void blink2(void) {//1sec
	while(1){
		if(getBlinkVar()>=10){
			resetBlinkVar();			
			//tända segment
			if(LCDDR13 != 0){ // om segment är på så stänger vi av denna
				LCDDR13 = 0;
				} else{
				LCDDR13 = 0x01; //sätter på den om den var släckt
			}
		}
	}
}

void CounterBtn(int pos){
	btnCount++;
	printAt(btnCount,pos);
}

void button(int pos) {
	PORTB = (1<<PORTB7); // set bit 7 to 1
	LCDDR3 = 0x1; //start segementen
	int previousButton = 0; //används för att kolla vilken segment som skall lysa
	int hold = 0; //använder denna för att kolla om man håller in joysticken

	while (1) {
		if (hold == 0 && PINB >> 7 == 0 && previousButton == 0) {
			CounterBtn(pos);
			LCDDR18 = 0x1;
			LCDDR3 = 0x0;
			previousButton = 1;
			hold = 1;
			} else if (hold == 0 && PINB >> 7 == 0 && previousButton == 1) {
			CounterBtn(pos);
			LCDDR3 = 0x1;
			LCDDR18 = 0x0;
			previousButton = 0;
			hold = 1;
			} else if (PINB >> 7 == 1) {//inte trycker
			hold = 0;
		}
	}
}

int main() {
    spawn(computePrimes, 0);
    //computePrimes(3);
	spawn(button, 4);
	//spawn(blink,0);
	blink2();
	//printAt(2,2);
}