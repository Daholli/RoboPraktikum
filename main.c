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


volatile uint16_t target1 = (SERVO_MIN+SERVO_MAX)/2;
volatile uint16_t target2 = (SERVO_MIN+SERVO_MAX)/2;


volatile uint16_t counter1 = (SERVO_MIN+SERVO_MAX)/2; // 0° Kleiner Arm
volatile uint16_t counter2 = (SERVO_MIN+SERVO_MAX)/2; // 0° Großer Arm

ISR(TIMER1_OVF_vect) {
    	if(counter1 < target1) {
        	counter1++;
    	} else if (counter1 > target1) {
		counter1--;
	}

    	if(counter2 < target2) {
        	counter2++;
	} else if (counter2 > target2) {
		counter2--;
	}
}

void setServoAngle(uint8_t nr, uint16_t alpha) {
	if(nr == 0) {
		target1 = round((alpha + 195)*5);
	} else if (nr == 1) {
		target2 = round((alpha + 110)*10);
	} else { 
		return;
	}
}

float phir[2];
void gotoXY(uint8_t x, uint8_t y) {
	phir[0] = atan2(x,y);
	phir[1] = sqrt((x*x) + (y*y));
	gotoRadial(phir[0], phir[1]);
}

uint16_t theta[2];
uint16_t* thetas(float phi, float r, uint16_t l1, uint16_t l2) {
	uint16_t alpha = acos(((r*r) + (l1*l1)  - (l2*l2))/(2*r*l1));
	uint16_t beta = acos(((l1*l1)+(l2*l2) - (r*r))/(2*l1*l2));
	theta[0] = round(phi - alpha);
	theta[1] = round(180 + beta);
	return theta;
}

void gotoRadial(float phi, float r) {	
	uint16_t* angles = thetas(phi, r, L1, L2);
	setServoAngle(0,angles[0]);
	setServoAngle(1,angles[1]);
}

void resetServo() {
	target1 = (SERVO_MIN+SERVO_MAX)/2; 
	target2 = (SERVO_MIN+SERVO_MAX)/2; 
}


int main(void) {
	// Initialisierung ausfuehren

	init();

	setBit(TIMSK1, TOIE1);

 	setBit(DDRC, 0);
 	setBit(DDRC, 5);


	gotoXY(3,4);
	
   	while(1) {
		uart_puts("\n\r Increments: ");
		uart_puti(counter1);
		uart_puts(" ");
		uart_puti(counter2);
		uart_puts(" Targets: ");
		uart_puti(target1);
		uart_puts(" ");
		uart_puti(target2);	
		
		if (counter1 != target1) {
			OCR1A = counter1;
		}
		
		if (counter2 != target2) {
			OCR1B = counter2;
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
