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

    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    
    //int k = 0;
    //uint16_t adc;
    //int threshold = 250;

    uint8_t counter;

    while(1) {
        //adc = getADCValue(k);
        counter=0;
        while(counter<=255) {
            _delay_us(100000);
            setPWM(counter);
            counter++;
            uart_puts("\n\r increment: ");
            uart_puti(counter);
        }
        counter=255;
        while(counter>=0) {
            _delay_us(100000);
            setPWM(counter);
            counter--;
            uart_puts("\n\r decrement: ");
            uart_puti(counter);
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
