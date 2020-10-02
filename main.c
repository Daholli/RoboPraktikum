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
#include <math.h>


void init();


#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))

void HSVtoRGB(uint16_t H, double S, double V){
	uint16_t hi = floor(H/60);
	double f = (H/60) - hi;
	double p = V*(1-S);
	double q = V*(1-(S*f));
	double t = V*(1-(S*(1-f)));

	switch(hi) {
		case 0 | 6:
			setRGB(V*255, t*255, p*255);
			break;
		case 1:
			setRGB(q*255, V*255, p*255);
			break;
		case 2:
			setRGB(p*255, V*255, t*255);
			break;
		case 3:
			setRGB(p*255, q*255, V*255);
			break;
		case 4:
			setRGB(t*255, p*255, V*255);
			break;
		case 5: 
			setRGB(V*255, p*255, q*255);
			break;
	}

}


void setRGB(double r, double g, double b) {
	setPWM(r);
	OCR1A = ((g*4*68)/150);
	OCR1B = ((b*4*68)/150);
}	
    
int main(void) {
	// Initialisierung ausfuehren

	init();
	
	DDRB |= (1<<1) | (1<<2);
	 
	TCCR1A = (1 << WGM10) | (1 << WGM11) | (1 << COM1A0) | (1 << COM1A1) | (1 << COM1B0) | (1 << COM1B1);
	TCCR1B = (1 << CS11);

	
    	uint16_t h=0;

   	while(1) {
		_delay_us(5000);
		uart_puti(h);
		if(h<360) {
			HSVtoRGB(h,1,1);
			h++;
		} else {
			h=0;
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
