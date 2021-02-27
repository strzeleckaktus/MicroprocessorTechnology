/**
* @file main.c
* @author Koryciak
* @date Sep 2020
* @brief File containing the main function.
* @ver 0.1
*/
#include "led.h"
/**
* @brief Simple delay
* @param c - delay control
* @return NULL
*/
void delay(unsigned char d){
for(volatile int i=0; i<(5000*d); i++);
}
/**
* @brief The main loop.
*
* @return NULL
*/
int main (void) {
volatile unsigned char c = 0;
 LED_Init (); /* initialize all LEDs */
 while(1){
c += 51;
LED_Ctrl(LED_RED, LED_ON);
delay(c);
LED_Ctrl(LED_RED, LED_OFF);
delay(c);
}
}
