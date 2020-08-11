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
    const uint16_t delay = 50;
    uint32_t nextEvent = getMsTimer()+delay;

    uint8_t sw_alt = 0;
    DDRC |= (1<<5);
       
    while (1) {
        uint8_t sw= !( PINC & ( 1 << 3 ) );
        if (sw_alt < sw) {
            if(nextEvent < getMsTimer()) {
                PORTC ^= (1<<5);
            }
        } else if (sw_alt > sw) {

        } else {

        }
        sw_alt=sw;
         
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
