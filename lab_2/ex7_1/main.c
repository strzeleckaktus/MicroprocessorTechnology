/*-----------------------------------------------------------------------------
* Name: main.c
* Purpose: RGB LED experiments for FRDM-KL05Z board
* Author: Student
*----------------------------------------------------------------------------*/
#include "MKL05Z4.h" /*Device header*/
#define RED_LED_POS 8
#define GREEN_LED_POS 9
#define BLUE_LED_POS 10
void delays_ms(int n);

void delay_ms(int n) {
volatile int i;
volatile int j;
	for( i = 0 ; i < n; i++)
	for(j = 0; j < 3500; j++) {}
}
int main()
{
SIM->SCGC5 |=SIM_SCGC5_PORTB_MASK; /* Enable clock for GPIO B */
PORTB->PCR[8] |= PORT_PCR_MUX(1); /* MUX config. Set Pin 8 of PORT B as GPIO */
PORTB->PCR[9] |= PORT_PCR_MUX(1); /* MUX config. Set Pin 9 of PORT B as GPIO */
PORTB->PCR[10] |= PORT_PCR_MUX(1); /* MUX config. Set Pin 10 of PORT B as GPIO */
PTB->PDDR |=(1<<RED_LED_POS); /* Set pin 8 of GPIO B as output */
PTB->PDDR |=(1<<GREEN_LED_POS); /* Set pin 9 of GPIO B as output */
PTB->PDDR |=(1<<BLUE_LED_POS); /* Set pin 10 of GPIO B as output */
while(1){
delay_ms(500);
PTB->PDOR&=~(1<<BLUE_LED_POS); /* Turn on BLUE LED */
delay_ms(500); /* Wait */
PTB->PDOR|=(1<<BLUE_LED_POS); /* Turn off BLUE LED */
delay_ms(500); /* Wait */
PTB->PDOR&=~(1<<BLUE_LED_POS); /* Turn on BLUE LED */
PTB->PDOR&=~(1<<GREEN_LED_POS); /* Turn on GREEN LED */
delay_ms(1000); /* Wait */
PTB->PDOR|=(1<<BLUE_LED_POS); /* Turn off BLUE LED */
PTB->PDOR|=(1<<GREEN_LED_POS); /* Turn off GREEN LED */
delay_ms(500);
PTB->PDOR&=~(1<<RED_LED_POS); /* Turn on RED LED */
delay_ms(500);	
PTB->PDOR|=(1<<RED_LED_POS);	// Turn off RED LED
delay_ms(500);
PTB->PDOR&=~(1<<RED_LED_POS); /* Turn on RED LED */
PTB->PDOR&=~(1<<GREEN_LED_POS); /* Turn on GREEN LED */
delay_ms(1000); /* Wait */
PTB->PDOR|=(1<<RED_LED_POS); /* Turn off RED LED */
PTB->PDOR|=(1<<GREEN_LED_POS); /* Turn off GREEN LED */
delay_ms(500); /* Wait */
PTB->PDOR&=~(1<<GREEN_LED_POS); /* Turn on GREEN LED */
delay_ms(500); /* Wait */
PTB->PDOR|=(1<<GREEN_LED_POS); /* Turn off GREEN LED */
delay_ms(500); /* Wait */
PTB->PDOR&=~(1<<BLUE_LED_POS); /* Turn on BLUE LED */
PTB->PDOR&=~(1<<RED_LED_POS); /* Turn on RED LED */
delay_ms(1000); /* Wait */
PTB->PDOR|=(1<<BLUE_LED_POS); /* Turn off BLUE LED */
PTB->PDOR|=(1<<RED_LED_POS); /* Turn off RED LED */
delay_ms(2000); /* Wait */
}
}
