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

uint16_t loopstart;
uint16_t loopend;
    
typedef enum {UINT8, UINT16, UINT32, FLOAT} Datatype;

void measureTime(Datatype type, int iterations) {
    uart_puts("\n\rIterations: ");
    uart_puti(iterations);
    int j;
    loopstart = getMsTimer();
    for(j=0; j < iterations; j++){
        asm volatile ("nop");
    }
    loopend = getMsTimer();
    uint16_t looplength = loopend - loopstart;
    switch(type) {
        case UINT8: {
            volatile uint8_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < iterations; i++) {
                z = ( z + 1 ) * 2;
                asm volatile ("nop");
            }
            end = getMsTimer()-looplength;
            uart_puts("\n\r uint8 runtime: ");
            uart_puti((end-start));
            return;
        }
        case UINT16: { 
            volatile uint16_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < iterations; i++) {
                z = ( z + 1 ) * 2;
                asm volatile ("nop");
            }
            end = getMsTimer()-looplength;
            uart_puts("\n\r uint16 runtime: ");
            uart_puti((end-start));
            return;
        }
        case UINT32: {
            volatile uint32_t z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < iterations; i++) {
                z = ( z + 1 ) * 2;
                asm volatile ("nop");
            }
            end = getMsTimer()-looplength;
            uart_puts("\n\r uint32 runtime: ");
            uart_puti((end-start));
            return;
        }
        case FLOAT: {
            volatile float z=0;
            start = getMsTimer();
            int i;
            for(i=0; i < iterations; i++) {
                z = ( z + 1 ) * 2;
                asm volatile ("nop");
            }
            end = getMsTimer()-looplength;
            uart_puts("\n\r float runtime: ");
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

    measureTime(UINT8, 10000);
    measureTime(UINT16, 10000);
    measureTime(UINT32, 10000);
    measureTime(FLOAT, 10000);
    

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
