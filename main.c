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
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    unsigned char c;
    int Stringlength = 4;
    char String[Stringlength+2];
    while(1) {
        int i;
        
        String[Stringlength] = "\n"
        String[Stringlength+1] = "\r"
        
        for(i=0; i < sizeof(String)-2; i++){
            c = uart_getc();
            String[i] = c;
        }
        uart_puts(String);
        
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
