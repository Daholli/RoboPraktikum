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

void timer0_init() {
    TCCR0 |= (1 << CS00);
    TCNT0 = 0;
}

    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    //char buffer[255];
    //
    DDRB |= (1 << 1);
    
    uint8_t timer;

    int k = 0;
    uint16_t adc;
    int threshold = 250;

    while(1) {
        adc = getADCValue(k);
        if(TCNT0 >= adc) {
            PORTB ^= (1 << 1);
            TCNT0 = 0; 
        }
        uart_puts("\n\r");
        uart_puti(adc);
        //uart_puti(adc);         
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
