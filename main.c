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
    
int main(void) {
	// Initialisierung ausfuehren

	init();
    //const uint16_t delay = 1000;
    //uint32_t nextEvent = getMsTimer()+delay;
    unsigned char c;
    int len = 10;
    char first[len];
    char second[len];
    char op;
    char res[10];
    float result;
    while(1) {
        first = "";
        second = "";
        result = 0;
        uart_puts("Enter operator (+ - * /)\n\r");
        op = uart_getc();
        uart_putc(op);
        int i; 
        uart_puts("\n\r Enter the first Number\n\r");
        for(i=0; i < sizeof(first); i++){
            c = uart_getc();
            if(c == '\r') {
                uart_puts(first + '\0');
                break;
            }
            first[i] = c;
        }
        uart_puts("\n\rEnter the second Number\n\r");
        for(i=0; i < sizeof(second); i++){
            c = uart_getc();
            if(c == '\r') {
                uart_puts(second + '\0');
                break;
            }
            second[i] = c;
        }
        switch(op) {
            case '+':
                result = atoi(first) + atoi(second); 
                break;
            case '-':
                result = atoi(first) - atoi(second); 
                break;
            case '*':
                result = atoi(first) * atoi(second); 
                break;
            case '/':
                result = atoi(first) / atoi(second); 
                break;
            default:
                uart_puts("Faulty operator\n\r")
        }
        uart_puts("\n\rYour result is:\n\r");
        ftoa(result, res, 4);
        uart_puts(res);
        uart_puts("\n\r"); 
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
