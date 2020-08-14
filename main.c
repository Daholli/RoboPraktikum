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

uint16_t start;
uint16_t end;
    
typedef enum {UINT8, UINT16, UINT32, FLOAT} Datatype;

void measureTime(Datatype type) {
    switch(type) {
        case UINT8:
            volatile uint8_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < 1000; i++) {
                z = ( z + 1 ) * 2;
            }
            end = getMsTimer();
            uart_puts("\n\r uint8 runtime: ");
            uart_puti((end-start));
            return;
        case UINT16:
            volatile uint16_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < 1000; i++) {
                z = ( z + 1 ) * 2;
            }
            end = getMsTimer();
            uart_puts("\n\r uint8 runtime: ");
            uart_puti((end-start));
            return;
        case UINT32:
            volatile uint32_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < 1000; i++) {
                z = ( z + 1 ) * 2;
            }
            end = getMsTimer();
            uart_puts("\n\r uint8 runtime: ");
            uart_puti((end-start));
            return;
        case FLOAT:
            volatile float z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < 1000; i++) {
                z = ( z + 1 ) * 2;
            }
            end = getMsTimer();
            uart_puts("\n\r uint8 runtime: ");
            uart_puti((end-start));
            return; 
        }

    }
}

    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    
    //int k = 0;

    measureTime(UINT8);
    measureTime(UINT16);
    measureTime(UINT32);
    measureTime(FLOAT);
    

    while(1) {
        

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
