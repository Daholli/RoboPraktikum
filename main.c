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
#include "avr/interrupt.h"
#include "stdlib.h"


void init();


#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))

uint16_t counter=1000;
uint16_t upperlimit = 1900;
uint16_t lowerlimit = 1100;

    
int main(void) {
	// Initialisierung ausfuehren
    init();

    setBit(DDRB, 1);
    setBit(DDRC, 5);

    uint8_t k;
    uint16_t adc;

    while(1) {
        while(counter<upperlimit) {
            _delay_us(5000);
            setServo(0,counter);
            counter++;
            uart_puts("\n\r increment: ");
            uart_puti(counter);
        }
        while(counter>lowerlimit) {
            _delay_us(5000);
            setServo(0, counter);
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
	servoInit(); // Servoansteuerung initialisieren
}
