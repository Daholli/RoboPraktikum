/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "servo.h"
#include "stdlib.h"


void init();


#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
	setPWM(r);
	OCR1A(g*4);
	ORC1B(b*4);
}	
    
int main(void) {
	// Initialisierung ausfuehren

	init();

	setbit(DDRB, 1);
	setbit(DDRB, 2);

	TCCR1A = (1 << WGM11) | (1 << COM1A0) | (1 << COM1A1);
	TCCR1B = (1 << CS01);

	
    	uint8_t counter=0;
	uint8_t upperlimit=255;

   	while(1) {
		counter++;
		_delay_us(1000);
		if(counter < 75) {
			setRGB(255,0,0);
		} else if(counter < 150) {
			setRGB(0,255,0);
		} else if(counter < 225) {
			setRGB(0,0,255);
		} else if(counter < 250) {
			setRGB(255,255,255);
		} else {
			counter = 0;
		}
    	}
}


//INIT
void init() {
	uartInit();		// serielle Ausgabe an PC
	ADCInit(0);		// Analoge Werte einlesen
	PWMInit();		// Pulsweite auf D6 ausgeben 
	timerInit();		// "Systemzeit" initialisieren
	// servoInit(); // Servoansteuerung initialisieren
}
