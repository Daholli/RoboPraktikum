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


#define L1 12
#define L2 10


volatile uint16_t target1;
volatile uint16_t target2;


volatile uint16_t counter1 = (SERVO_MIN+SERVO_MAX)/2; // 0° Kleiner Arm
volatile uint16_t counter2 = (SERVO_MIN+SERVO_MAX)/2; // 0° Großer Arm

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
		target1 = round((alpha + 195)/0.2);
		OCR1A = counter1;
	} else if (nr == 1) {
		target2 = round((alpha + 110)/0.1);
		OCR1B = counter2;
	} else { 
		return;
	}
}

float* xytoRadial(uint8_t x, uint8_t y) {
	float phir = [2];
	phir[0] = atan2(x,y);
	phir[1] = sqrt((x*x) + (y*y));
	return phir;
}

uint16_t* thetas(float phi, float r, uint16_t l1, uint16_t l2) {
	uint16_t thetas = [2];
	uint16_t alpha = acos(((r*r) + (l1*l1)  - (l2*l2))/(2*r*l1));
	uint16_t beta = acos(((l1*l1)+(l2*l2) - (r*r))/(2*l1*l2));
	thetas[0] = round(phi - alpha);
	thetas[1] = round(180 - beta);
	return thetas;
}

void gotoRadial(float r, float phi) {	
	angles = thetas(phi, r, L1, L2);
	

}

void resetServo {
	target1 = (SERVO_MIN+SERVO_MAX)/2; 
	target2 = (SERVO_MIN+SERVO_MAX)/2; 

}


int main(void) {
	// Initialisierung ausfuehren

	init();

	setBit(TIMSK1, TOIE1);

 	setBit(DDRC, 0);
 	setBit(DDRC, 5);

	
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
