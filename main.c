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

uint8_t timer =0;

ISR(timer) {

}
    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    //char buffer[255];
    DDRB |= (1 << 1);
    
    DDRD &= ~(1<<1);
    
    int k = 0;
    uint16_t adc;
    //int threshold = 250;
    uint8_t timer =0;

    while(1) {
        adc = getADCValue(k);
        int i;
        for(i=0; i < 255; i++) {
            if(timer >= adc) {
                clearBit(PORTB, 1)
            } else {
            setBit(PORTB, 1);
            }
        }
        timer =0;
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
