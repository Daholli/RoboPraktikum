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

volatile float mod = 1;


volatile uint16_t target1 = (SERVO_MIN + SERVO_MAX)/2;
volatile uint16_t target2 = (SERVO_MIN + SERVO_MAX)/2;

volatile uint16_t counter1 = (SERVO_MIN + SERVO_MAX)/2; // Großer Arm
volatile uint16_t counter2 = (SERVO_MIN + SERVO_MAX)/2; // kleiner Arm

ISR(TIMER1_OVF_vect) {
    	if(abs(counter1 - target1) > 10) {
		if (counter1 < target1) {
        		counter1 += 3;
    		} else if (counter1 > target1) {
			counter1 -= 3;
		}
	} else {
		 if (counter1 < target1) {
        		counter1 += 1;
    		} else if (counter1 > target1) {
			counter1 -= 1;
		}
	}
	if(abs(counter2 - target2) > 10) {
		if (counter2 < target2) {
        		counter2 += round(3 * mod);
    		} else if (counter2 > target2) {
			counter2 -= round(3 * mod);
		}
	} else {
		 if (counter2 < target2) {
        		counter2 += 1;
    		} else if (counter2 > target2) {
			counter2 -= 1;
		}
	}
}
// init array for the picture

volatile float* xPic;
volatile float* yPic;

volatile uint16_t iterx = 0;
volatile uint16_t itery = 0;

void addtoarray(char id, float coord) {
	if(id == 'x') {
		xPic[iterx] = coord;
		iterx++;
	} else if (id == 'y') {
		yPic[itery] = coord;
		itery++;
	}
}

volatile uint16_t arraysize;

volatile int bint=0;
volatile char buffer[100];

ISR(USART_RX_vect) {
	buffer[bint] = UDR0;
	if(buffer[bint] == '\r') {
		buffer[bint] = '\0';
		if(buffer[0] == 'x') {
			memmove(buffer, buffer+1, strlen(buffer));
			addtoarray('x' , atof(buffer));
		} else if (buffer[0] == 'y') {
			memmove(buffer, buffer+1, strlen(buffer));
			addtoarray('y', atof(buffer));
		} else if (buffer[0] == 's') {
			memmove(buffer, buffer+1, strlen(buffer));
			arraysize = atoi(buffer);
		}
		bint = 0;
	} else {
		bint++;
	}
}

void setServoAngle(uint8_t nr, uint16_t alpha) {
	if(nr == 0) {
		target1 = (5*alpha) + 1000;
	} else if (nr == 1) {
		target2 = (10*alpha) + 1000;
	} else { 
		return;
	}
}

float phir[2];
void gotoXY(float x, float y) {
	phir[0] = atan2(y,x)*180/M_PI;
	phir[1] = sqrtf(x*x+y*y);
	gotoRadial(phir[0], phir[1]);
}

uint16_t theta[2];
uint16_t* thetas(float phi, float r, uint16_t l1, uint16_t l2) {
	uint16_t alpha = acos(((r*r) + (l1*l1)  - (l2*l2))/(2*r*l1))*180/M_PI;
	uint16_t beta = acos(((l1*l1)+ (l2*l2) - (r*r))/(2*l1*l2))*180/M_PI;
	/*uart_puts("\n\r a, b, phi: ");
	uart_puti(alpha);
	uart_puts(" ");
	uart_puti(beta);
	uart_puts(" ");
	uart_puti(round(phi));*/
	theta[0] = round(phi + alpha);
	theta[1] = round(180 - beta);
	return theta;
}

void gotoRadial(float phi, float r) {	
	uint16_t* angles = thetas(phi, r, L1, L2);
	/*uart_puts("\n\r Angles: ");
	uart_puti(angles[0]);
	uart_puts(" ");
	uart_puti(angles[1]);*/
	setServoAngle(0,angles[0]);
	setServoAngle(1,angles[1]);
	scale();
}

void scale() {
	float modifier1 = abs(counter1 - target1);
	float modifier2 = abs(counter2 - target2);

	mod = modifier2/modifier1;
	if(mod > 5){
		mod = 5;
	}
			
}

int main(void) {
	// Initialisierung ausfuehren

	init();

	setBit(TIMSK1, TOIE1);

	setBit(UCSR0B, RXCIE0);
	setBit(UCSR0B, RXC0);
    setBit(UCSR0B, TXC0);

	uint8_t lock =0;
	
	uint16_t currentstep = 0;

   	while(0) {
				
		if(1) {
			// Let the servo draw slowly
			if (counter1 != target1) {
				OCR1A = counter1;
			}
			
			if (counter2 != target2) {
				OCR1B = counter2;
			}				
			
			// determine if it arrived
			if (counter1 == target1 && counter2 == target2 && lock) {
				lock = 0;
			} else {
				if(!lock && currentstep < arraysize){
					gotoXY(xPic[currentstep], yPic[currentstep]);
					lock = 1;
					_delay_ms(10);
					currentstep++;
				}
			}
		}	
	}
}


//INIT
void init() {
	uartInit();		// serielle Ausgabe an PC
	//ADCInit(0);		// Analoge Werte einlesen
	//PWMInit();		// Pulsweite auf D6 ausgeben 
	timerInit();		// "Systemzeit" initialisieren
	servoInit(); 		// Servoansteuerung initialisieren
}
