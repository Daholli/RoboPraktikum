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
#include <math.h>


void init();


#define setBit(reg, bit) (reg |= (1 << bit))
#define clearBit(reg, bit) (reg &= ~(1 << bit))
#define toggleBit(reg, bit) (reg ^= (1 << bit))
#define clearFlag(reg, bit) (reg |= (1<<bit))

void reverse(char* str, int len) {
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d) { 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

void ftoa(float n, char* res, int afterpoint) { 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 


void setRGB(uint8_t R, uint8_t G, uint8_t B) {
	switch(R) {
		case 0:
			clearBit(PORTB, 3);
			break;
		case 1:
			setBit(PORTB, 3);
			break;
		default:
			clearBit(PORTB, 3);
			break;

	}
	switch(G) {
		case 0:
			clearBit(PORTB, 4);
			break;
		case 1:
			setBit(PORTB, 4);
			break;
		default:
			clearBit(PORTB, 4);
			break;


	}
	switch(B) {
		case 0:
			clearBit(PORTB, 5);
			break;
		case 1:
			setBit(PORTB, 5);
			break;
		default:
			clearBit(PORTB, 5);
			break;
	}

	return;
}

float maxof(float R, float G, float B) {
	if (R > G && R > B){
		return R;
	} else if (B > R && B > G) {
		return B;
	} else if (G > R && G > B) {
		return G;
	} else {
		return R;
	}
}

float minof(float R, float G, float B) {
	if (R < G && R < B){
		return R;
	} else if (B < R && B < G) {
		return B;
	} else if (G < R && G < B) {
		return G;
	} else {
		return R;
	}
}

float normRGB[3];
float * normalize(uint16_t R, uint16_t G, uint16_t B, uint16_t min, uint16_t max) {
	if(max == 0) {
		normRGB[0] = 0;
		normRGB[1] = 0;
		normRGB[2] = 0;
		return normRGB;
	}	
	normRGB[0] = (R-min)/(max-min);
	normRGB[1] = (G-min)/(max-min);
	normRGB[2] = (B-min)/(max-min);

	return normRGB;
}



float HSV[3];
float * RGBtoHSV(float R, float G, float B) {
	float min = minof(R, G, B);
	float max = maxof(R, G, B);

	if(min == max || (R == G && G == B) ) {
		HSV[0] = 0;
		if(max == 0){
			HSV[1] = 0;
			HSV[2] = max;
		}
	} else if ( max == R ) {
		HSV[0] = 60*(0+((G-B)/(max-min)));
		HSV[1] = (max-min)/max;
		HSV[2] = max;
		return HSV;
	} else if ( max == G ) {
		HSV[0] = 60*(2+((B-R)/(max-min)));
		HSV[1] = (max-min)/max;
		HSV[2] = max;
		return HSV;
	} else if ( max == B ) {
		HSV[0] = 60*(4+((R-G)/(max-min)));
		HSV[1] = (max-min)/max;
		HSV[2] = max;
		return HSV;
	}
	

}
	


int main(void) {
	// Initialisierung ausfuehren

	init();
	DDRB |= (1<<3) | (1 <<4) | (1 << 5);
	//uint16_t adc;	
	uint8_t c = 0;
	uint16_t fremdlicht;
	uint16_t max;
	uint16_t R;
	uint16_t G;
	uint16_t B;
	
	float *RGB;
	float *HSV;
	
   	while(1) {
		if(c == 0) {
			setRGB(0,0,0);
			_delay_ms(500);				
			fremdlicht = getADCValue(0);
			c++;
		} else if(c == 1) {
			setRGB(1,1,1);
			_delay_ms(500);				
			max = getADCValue(0);
			c++;
		} else if(c == 2) {
			setRGB(1,0,0);
			_delay_ms(500);				
			R = getADCValue(0);
			c++;
		} else if(c == 3) {
			setRGB(0,1,0);
			_delay_ms(500);				
			G = getADCValue(0);
			c++;
		} else {
			setRGB(0,0,1);
			_delay_ms(500);				
			B = getADCValue(0);
			c=0;
		}

		uart_puts("\r\n");
		uart_puti(R);
		uart_puts(" ");
		uart_puti(G);
		uart_puts(" ");
		uart_puti(B);
		uart_puts(" ");
		uart_puti(fremdlicht);
		uart_puts(" ");
		uart_puti(max);
		

		RGB = normalize(R,B,G,fremdlicht,max);
		HSV = RGBtoHSV(*(RGB +0), *(RGB +1), *(RGB +2));
		
		char R[10];
		char G[10];
		char B[10];
		ftoa(*(RGB+0), R, 5);
		ftoa(*(RGB+1), G, 5);
		ftoa(*(RGB+2), B, 5);
		uart_puts("\r\n");
		uart_puts("R: ");
		uart_puts(R);
		uart_puts(" G: ");
		uart_puts(G);
		uart_puts(" B: ");
		uart_puts(B);
 

		char H[10];
		char S[10];
		char V[10];
		ftoa(*(HSV+0), H, 2);
		ftoa(*(HSV+1), S, 2);
		ftoa(*(HSV+2), V, 2);
		uart_puts("\r\n");
		uart_puts("H: ");
		uart_puts(H);
		uart_puts(" S: ");
		uart_puts(S);
		uart_puts(" V: ");
		uart_puts(V);
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
