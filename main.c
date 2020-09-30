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
#include <avr/interrupt.h>


void init();


#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))


volatile uint16_t target1;
volatile uint16_t target2;


volatile uint16_t counter1 = 1999;
volatile uint16_t counter2 = 1001;

ISR(TIMER1_OVF_vect) {
    	if(counter1<target1) {
        	counter1++;
    	} else if (counter1 > target1) {
		counter1--;
	}

    	if(counter2<target2) {
        	counter2++;
	} else if (counter2 > target2) {
		counter2--;
	}
}

void setServoAngle(uint8_t nr, uint16_t alpha) {
	if(nr == 0) {
		OCR1A = alpha;
	} else if (nr == 1) {
		OCR1B = alpha;
	} else { 
		return;
	}
}

void calibrate() {

}


int main(void) {
	// Initialisierung ausfuehren

	init();

	setBit(TIMSK1, TOIE1);

 	setBit(DDRC, 0);
 	setBit(DDRC, 5);
	target1 = 1001;
	target2 = 1999;

	
   	while(1) {
		uart_puts("\n\r Increments: ");
		uart_puti(counter1);
		uart_puts(" ");
		uart_puti(counter2);
		uart_puts(" Targets: ");
		uart_puti(target1);
		uart_puts(" ");
		uart_puti(target2);	
		
		if(counter1 == target1) {
			setBit(PORTC, 5);
		} else if (counter1 > target1 || counter1 < target1) {
			setServoAngle(0, counter1);
		} else {
			clearBit(PORTC, 5);
		}
		
		if(counter2==target2) {
			setBit(PORTC, 0);
		} else if (counter2 < target2 || counter2 > target2) {
			setServoAngle(1, counter2);
		} else {
			clearBit(PORTC, 0);
		}	
			
			
	}
}


//INIT
void init() {
	uartInit();		// serielle Ausgabe an PC
	ADCInit(0);		// Analoge Werte einlesen
	PWMInit();		// Pulsweite auf D6 ausgeben 
	timerInit();		// "Systemzeit" initialisieren
	servoInit(); 		// Servoansteuerung initialisieren
}
