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


void init();

int main(void) {
	// Initialisierung ausfuehren

	init();

    const uint16_t delay=1000;
    uint32_t nextEvent=getMsTimer()+delay;


    DDRC |= (1<<5);

	while (1) {
        if(nextEvent<getMsTimer()) {
            nextEvent += delay;
            if( PINC & (1 << 5)) {
                PORTC &= ~(1<<5);
            } else {
                PORTC |= (1<<5);
            }
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
