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

    uint32_t start;

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

    enumsmall fuss_ampel(enumsmall zustand) {
        switch(zustand) {
            case ROT:
                break;
            case GRUEN:
                break;
        }
    }

    enum phases {0, 1, 2, 3} Phasen;

    bool lock;

    void changePhases(phases Phasen) {
        while(getMsTimer() > start+10000) {
            switch(Phasen) {
                case 0: 
                    // Autos werden Gelb Fußgänger bleiben Rot
                    if (getMsTimer() > start+ 1000) {


                        changePhases(1);
                        return;
                    }
                case 1: 
                    // Autos werden Rot Fußgänger werden Grün
                    if (getMsTimer() > start+ 2000) {
    
    
                        changePhases(2);
                        return;
                    }
                case 2:
                    // Autos werden Rot-Gelb Fußgänger werden Rot
                    if (getMsTimer() > start+ 7000) {


                        changePhases(3);
                        return;
                    }
                case 3:
                    // Autos werden Grün Fußgänger bleiben Rot
                    if (getMsTimer() > start+ 8000) {


                        lock=false;
                        return;
                    }
            }
        }
    }


    void keyPress() {
        start = getMsTimer();
        if(!lock) {
            changePhases(0);
            lock = true;
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
