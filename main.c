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

volatile uint8_t temperatur;

#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))


void setDisplay(uint8_t num) {
	switch(num){
		case 0:
			clearBit(PORTD,5);
			setBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			clearBit(PORTC,3);
			clearBit(PORTC,4);
			clearBit(PORTC,5);
			break;
		case 1:	
			clearBit(PORTD,5);
			setBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			setBit(PORTD,4);

			setBit(PORTC,3);
			setBit(PORTC,4);
			setBit(PORTC,5);
			break;
		case 2:
			clearBit(PORTD,5);
			clearBit(PORTD,6);
			setBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			clearBit(PORTC,3);
			clearBit(PORTC,4);
			setBit(PORTC,5);
			break;
		case 3:
			clearBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			setBit(PORTC,3);
			clearBit(PORTC,4);
			setBit(PORTC,5);
			break;
		case 4:
			clearBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			setBit(PORTD,4);

			setBit(PORTC,3);
			setBit(PORTC,4);
			clearBit(PORTC,5);
			break;
		case 5:
			setBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			setBit(PORTC,3);
			clearBit(PORTC,4);
			clearBit(PORTC,5);
			break;
		case 6:
			setBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			clearBit(PORTC,3);
			clearBit(PORTC,4);
			clearBit(PORTC,5);
			break;
		case 7:
			clearBit(PORTD,5);
			setBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			setBit(PORTD,4);

			setBit(PORTC,3);
			clearBit(PORTC,4);
			setBit(PORTC,5);
			break;
		case 8:
			clearBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			clearBit(PORTC,3);
			clearBit(PORTC,4);
			clearBit(PORTC,5);
			break;
		case 9:
			clearBit(PORTD,5);
			clearBit(PORTD,6);
			clearBit(PORTD,7);
			setBit(PORTD,3);
			clearBit(PORTD,4);

			setBit(PORTC,3);
			clearBit(PORTC,4);
			clearBit(PORTC,5);
			break;

		default:
			setBit(PORTD,5);
			setBit(PORTD,6);
			setBit(PORTD,7);
			setBit(PORTD,3);
			setBit(PORTD,4);

			setBit(PORTC,3);
			setBit(PORTC,4);
			setBit(PORTC,5);
			break;
	}
}

void firstNumber(uint8_t num) {
	clearBit(PORTB, 2);
	setBit(PORTB, 1);
	setDisplay(num);
}
void secondNumber(uint8_t num) {
	clearBit(PORTB, 1);
	setBit(PORTB, 2);
	setDisplay(num);
	clearBit(PORTD, 3);
}

void numberlogic() {
	if(floor(temperatur/10) == 0) {
		secondNumber(temperatur);
	} else {
		firstNumber(floor(temperatur/10));
		secondNumber(temperatur-(floor(temperatur/10)*10));
	}


}

#define SIZE_OF_BUFFER 10


int main(void) {
	// Initialisierung ausfuehren

	init();
	uint16_t adc;	


	setBit(DDRD,5);
	setBit(DDRD,6);
	setBit(DDRD,7);

	setBit(DDRD,3);
	setBit(DDRD,4);

	setBit(DDRC,3);
	setBit(DDRC,4);
	setBit(DDRC,5);
	
	setBit(DDRB, 1);
	setBit(DDRB, 2);


	uint16_t circularBuffer[10];
	uint8_t bufferLength =0;

	uint8_t writeIndex = 0;
	
	uint32_t sum = 0;
	
   	while(1) {
		adc = getADCValue(0);

		uint16_t tmp = circularBuffer[writeIndex];
		circularBuffer[writeIndex] = adc;

		if(bufferLength < SIZE_OF_BUFFER) {
			bufferLength++;
			sum += circularBuffer[writeIndex];
		} else {
			sum -= tmp;
			sum += circularBuffer[writeIndex];
		}
		
		writeIndex++;	
		if(writeIndex == SIZE_OF_BUFFER) {
			writeIndex =0;
		}

		temperatur = round((sum/bufferLength)*1.075*0.1);
		uart_puts("\n\r");	
		uart_puti(temperatur);
    	}
}


//INIT
void init() {
	uartInit();		// serielle Ausgabe an PC
	ADCInit(0);		// Analoge Werte einlesen
	//PWMInit();		// Pulsweite auf D6 ausgeben 
	timerInit();		// "Systemzeit" initialisieren
	// servoInit(); // Servoansteuerung initialisieren
}
