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
    //char buffer[255];
    DDRB |= (1 << 1);
    
    DDRD &= ~(1<<1);
    
    int k = 0;
    uint16_t adc;
    //int threshold = 250;
    uint8_t timer =0;

    while(1) {
        _delay_us(100000);
        adc = getADCValue(k);
        while(timer < 255) {
            if(timer >= adc) {
                clearBit(PORTB, 1);
            } else {
                setBit(PORTB, 1);
            }
            timer++;
            uart_puts("\n\r Timer: ");
            uart_puti(timer);
        }
        timer =0;
        uart_puts("\n\r ADC: ");
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
