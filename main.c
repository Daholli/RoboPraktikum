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


volatile uint16_t target=1900;
volatile uint16_t increment = 1050;

ISR(TIMER1_OVF_vect) {
    if(increment<target) {
        increment++;
        setBit(PORTC,0);
    } else {
        clearBit(PORTC, 0);
    }
    
}
    
int main(void) {
	// Initialisierung ausfuehren
    init();

    setBit(TIMSK1, TOIE1);
    setBit(DDRB, 1);
    setBit(DDRC, 0);
    setBit(DDRC, 5);
    
    //uint8_t k;
    //uint16_t adc;

    while(1) {
        _delay_us(5000);
        setServo(0, increment);
        uart_puts("\n\r");
        uart_puti(increment);
        if(increment==target) {
            setBit(PORTC, 5);
        } else {
            clearBit(PORTC, 5);
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
