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
    const uint16_t delay = 2000;
    uint32_t nextEvent = getMsTimer()+delay;
    char s[] = "Hello World!\n\r";
    while(1) {
        if(nextEvent < getMsTimer()) {
            nextEvent += delay;
            uart_puts(s);
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
