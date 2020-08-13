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
	DDRD |= (1 << 5);	// Pin 5 an PORTD auf Ausgang stellen

    uint8_t counter=0;
    uint8_t upperlimit = 255;
    uint8_t lowerlimit = 0;

    while(1) {
        //adc = getADCValue(k);
        while(counter<upperlimit) {
            _delay_us(5000);
            setPWM(counter);
            OCROB = counter;
            counter++;
            //uart_puts("\n\r increment: ");
            //uart_puti(counter);
        }
        while(counter>lowerlimit) {
            _delay_us(5000);
            setPWM(counter);
            counter--;
            OCROB = counter;
            //uart_puts("\n\r decrement: ");
            //uart_puti(counter);
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
