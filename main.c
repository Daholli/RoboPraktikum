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
    
    


    enum enumbig {ROT, GELB, GRUEN} autos;
    enum enumsmall {ROT, GRUEN} fuss;

    enumbig auto_ampel(enumbig zustand) {
        switch(zustand) {
            case ROT:
                break;
            case GELB:
                break;
            case GRUEN:
                break;
        }
    }

    void changePhases() {

    }

    void keyPress() {
        const uint32_t start = getMsTimer();
        if (getMstTimer() > start + 1000) {
            changePhases();
        }
    }
       
    while (1) {
        uint8_t sw= !( PINC & ( 1 << 3 ) );
        if(nextEvent < getMsTimer()) {
            if (sw_alt < sw) {
                PORTC ^= (1<<5);
            }
            sw_alt=sw;
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
