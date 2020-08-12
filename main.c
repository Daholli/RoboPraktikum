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

 
    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    //char buffer[255];
    //
    DDRB |= (1 << 1);

    int k = 0;
    uint16_t adc;
    int threshold = 250;

    while(1) {
        uart_puts("\n\r");
        adc = getADCValue(k);
        if(adc < threshold) {
            PORTB |= (1<<1);
        } else {
            PORTB &= ~(1<<1);
        }
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
