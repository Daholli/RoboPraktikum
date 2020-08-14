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

#define MAXLEN 10





volatile char uart_string[MAXLEN +1] = "";
volatile uint8_t uart_str_len = 0;
volatile uint8_t complete;


void returnString(volatile char* s) {
    /*int i;
    for(i=0; i < sizeof(s); i++){
        while( !(UDRE0 & ( 1 << UDRE0 )) );
        UDR0 = s[i];

    }*/
    char* output = (char*) s;
    uart_puts("\n\r");
    uart_puts(output);
    complete=0;
}


ISR(USART_RX_vect) {
    unsigned char c;
    c = UDR0;
    if(!complete) {
        if(c != '\n' && c != '\r' && uart_str_len < MAXLEN) {
            setBit(PORTC, 5);
            uart_string[uart_str_len] = c;
            uart_str_len++;
        } else {
            clearBit(PORTC, 5);
            uart_string[uart_str_len] = '\0';
            uart_str_len = 0;
            complete =1;
        }
    }

}

    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    
    //int k = 0;
    sei();
    setBit(UCSR0B, RXCIE0);
    setBit(UCSR0B, RXC0);
    setBit(UCSR0B, TXC0);

    setBit(DDRC, 5);
    setBit(DDRC, 0);

    while(1) {
        if(complete){
            setBit(PORTC, 0);
            returnString(uart_string);
        } else {
            clearBit(PORTC, 0);

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
