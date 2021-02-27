/*-------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 6 - Ćwiczenie 2: testowanie odbiornika z przerwaniami
					autor: Mariusz Sokołowski
					wersja: 16.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char rx_buf[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
char temp;
uint8_t rx_FULL=0;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Read data and clear RDRF flag
		if(!rx_FULL)
		{
			rx_buf[0] = temp;	// Send data to main	
			rx_FULL=1;
		}
	}
	NVIC_EnableIRQ(UART0_IRQn);
}
int main (void)
{	
	UART0_Init();		// Init UART0
	while(1)
	{
		if(rx_FULL)		// Is data ready?
		{
			while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
			UART0->D = rx_buf[0];
			rx_FULL=0;	// Data taken
		}
	}
}
