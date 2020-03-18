/*
 * Lab1_2.c
 *
 * Created: 2020-01-29 14:01:25
 * Author : Elie Jamous
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/portpins.h>


void writeLong(long i);
void LCD_Init(void);
void writeChar(char ch, int pos);
void writeLong(long i);


void LCD_Init(void){
	LCDCRA= (1<<LCDEN) | (1<<LCDAB);
	LCDCRB= (1<<LCDMUX1) | (1<<LCDMUX0) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0) | (1<<LCDCS);
	LCDCCR= (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
	LCDFRR= (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
}

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
	else if((ch<0)|(ch>9)){
		return;
	}
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


void writeLong(long i){
	int pos = 5;
	while (i>0){
		int charr = i%10;
		writeChar(charr,pos);
		pos--;
		i=i/10;
	}
}

int is_prime(long i){
	if(i>0){
		for(int j = 2;j<i;j++){
			if((i%j)==0){
				return 0; //inte prime
			}
		}
	}
	return 1;
}

void writePrime(void){
	long i =0;
	while(1){
		if(is_prime(i)){
			writeLong(i);
		}
		i++;
	}
}

void blink(void) {
	TCCR1B = (1<<CS12); //256 prescaler
	uint16_t boundary = 8000000/256; //8MHz/prescaler (borde vara ungefär 1 sec) (31250 cycler av 0xFFFF)
	uint16_t wrapstatus = 0; //Off default
	
	while(1){
		if(TCNT1 >= boundary &&wrapstatus==0 ){
			wrapstatus = 1; 
			
			//flytta boundary
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
		//kolla om boundary ligger under klockan3
		if(TCNT1<boundary){
			wrapstatus=0;
		}
	}
}


void button(void) {
	/*
	när sjunde bitten i PINB är 0 så betyder det att josticken trycks ner
	när sjunde bitten i PINB är 1 så betyder det att josticken inte trycks ner
	*/
	PORTB = (1<<PORTB7); // set bit 7 to 1
	LCDDR3 = 0x1; //start segementen
	int toggle = 0; //används för att kolla vilken segment som skall lysa
	int down = 0; //använder denna för att kolla om man håller in joysticken
	
	while (1) {
		if (down == 0 && PINB >> 7 == 0 && toggle == 0) {
			LCDDR18 = 0x1;
			LCDDR3 = 0x0;
			toggle = 1;
			down = 1;
			} else if (down == 0 && PINB >> 7 == 0 && toggle == 1) {
			LCDDR3 = 0x1;
			LCDDR18 = 0x0;
			toggle = 0;
			down = 1;
			} else if (PINB >> 7 == 1) {//inte trycker
			down = 0; 
		}
	}
}



void partFyra(long i){
	TCCR1B = (1<<CS12); //256 prescaler
	uint16_t totalTimer = 8000000/256; //8MHz/prescaler (borde vara ungefär 1 sec) (31250 cycler av 0xFFFF)
	uint16_t wrapstatus = 0; //Off default
	
	PORTB = (1<<PORTB7); // set bit 7 to 1
	LCDDR3 = 0x1; //start segementen
	int toggle = 0; //används för att kolla vilken segment som skall lysa
	int down = 0; //använder denna för att kolla om man håller in joysticken
	
	
	
	
	while(1){
			if(is_prime(i)){
				writeLong(i);
			}
			i++;
		
		
			if (down == 0 && PINB >> 7 == 0 && toggle == 0) {
				LCDDR18 = 0x1;
				LCDDR3 = 0x0;
				toggle = 1;
				down = 1;
				} else if (down == 0 && PINB >> 7 == 0 && toggle == 1) {
				LCDDR3 = 0x1;
				LCDDR18 = 0x0;
				toggle = 0;
				down = 1;
				} else if (PINB >> 7 == 1) {
				down = 0;
			}
		
		
			if(TCNT1 >= totalTimer &&wrapstatus==0 ){
				wrapstatus = 1;
				if(totalTimer + 8000000/256 > 0xffff){
					totalTimer= totalTimer +(8000000/256) -0xffff;//om totalTimer gått över toppen så gör vi wraparound
				}
				else{
					totalTimer = totalTimer+8000000/256; //annars sätter på value
				}
				//tända segment
				if(LCDDR13 != 0){ // om segment är på så stänger vi av denna
					LCDDR13 = 0;
					} else{
					LCDDR13 = 0x1; //sätter på den om den var släckt
				}
			}
			if(TCNT1<totalTimer){
				wrapstatus=0;
			}
	}
	
}


int main(void)
{
	CLKPR=0x80;
	CLKPR=0x00;
	LCD_Init();
	//writeChar(5,5);
	//writeChar(4,4);
	partFyra(25000);
	//blink(); // 2
	//button(); // 3
	//writePrime(); //task1
	//writeLong(7654321);
	//writeChar(3,3);
	//writeChar(1,5);
	//writeChar(2,4);
	
}