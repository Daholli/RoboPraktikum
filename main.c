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
    
uint32_t start;
int lock = 0;
    
typedef enum {ROT, ROTGELB, GELB, GRUEN} Autos;
typedef enum {ROTf, GRUENf} Fuss;

void auto_ampel(Autos zustand) {
    switch(zustand) {
        case ROT:
            PORTC |=  (1<<5);
            PORTC &= ~(1<<4);
            PORTC &= ~(1<<3);
            break;
        case ROTGELB:
            PORTC |=  (1<<5);
            PORTC |=  (1<<4);
            PORTC &= ~(1<<3);
            break;
        case GELB:
            PORTC &= ~(1<<5);
            PORTC |=  (1<<4);
            PORTC &= ~(1<<3);
            break;
        case GRUEN:
            PORTC &= ~(1<<5);
            PORTC &= ~(1<<4);
            PORTC |=  (1<<3);
            break;
    }
}

void fuss_ampel(Fuss zustand) {
    switch(zustand) {
        case ROTf:
            PORTC |=  (1<<1);
            PORTC &= ~(1<<0);
            break;
        case GRUENf:
            PORTC &= ~(1<<1);
            PORTC |=  (1<<0);
            break;
    }
}
    
void changePhases(int phases) {
    while(getMsTimer() < start+10000) {
        switch(phases) {
            case 0: 
            // Autos werden Gelb Fußgänger bleiben Rot
                if (getMsTimer() > start+ 1000) {
                    auto_ampel(GELB);
                    fuss_ampel(ROTf);

                    changePhases(1);
                    return;
                }
            case 1: 
                // Autos werden Rot Fußgänger werden Grün
                if (getMsTimer() > start+ 2000) {
                    auto_ampel(ROT);
                    fuss_ampel(GRUENf);
                    changePhases(2);
                    return;
                }
            case 2:
                // Autos werden Rot-Gelb Fußgänger werden Rot
                if (getMsTimer() > start+ 7000) {
                    auto_ampel(ROTGELB);
                    fuss_ampel(ROTf);

                    changePhases(3);
                    return;
                }
            case 3:
                // Autos werden Grün Fußgänger bleiben Rot
                if (getMsTimer() > start+ 8000) {
                    auto_ampel(GRUEN);
                    fuss_ampel(ROTf);

                    lock=0;
                    return;
            case 4:
                    // Default 
                    auto_ampel(GRUEN);
                    fuss_ampel(ROTf);
                    return;
                }
        }
    }
}

void keyPress() {
    start = getMsTimer();
    if(!lock) {
        changePhases(0);
        lock = 1;
    }        
}


int main(void) {
	// Initialisierung ausfuehren

	init();

    DDRC |= (1 << 5); // AutoRot
    DDRC |= (1 << 4); // AutoGelb
    DDRC |= (1 << 3); // AutoGrün

    DDRC |= (1 << 1); // FußRot
    DDRC |= (1 << 0); // FußGrün

    changePhases(4);
       
    uint8_t sw_alt= 0;
    while (1) {
        uint8_t sw= !( PINB & ( 1 << 5 ) );
        if (sw_alt > sw) {
            keyPress();
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
