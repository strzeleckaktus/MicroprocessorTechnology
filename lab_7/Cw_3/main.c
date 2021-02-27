/*-------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 6 - Ćwiczenie 3: sterowanie urządzeniami zewnętrznymi
					autor: Mariusz Sokołowski
					wersja: 16.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "uart0.h"
#include "led.h"
#include "tpm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LF	0xa		// Enter
char rx_buf[16];
char LED_R_ON[]="LRON";
char LED_R_OFF[]="LROFF";
char LED_R_DIM[]="LRDIM";
char LED_R_BRI[]="LRBRI";
char Error[]="Zla komenda";
char Too_Long[]="Zbyt dlugi ciag";
uint8_t rx_buf_pos=0;
char temp,buf;
uint8_t rx_FULL=0;
uint8_t too_long=0;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Read data s flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=LF)
			{
				if(!too_long)	// Timeout, ignore rest
				{
					rx_buf[rx_buf_pos] = temp;	// Take bytes
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// 
				}
			}
			else
			{
				if(!too_long)	// If too long, abandon
					rx_buf[rx_buf_pos] = 0;
				rx_FULL=1;
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}
int main (void)
{	
	int brightness;
	uint8_t i;
	LED_Init();
	TPM0_Init_PWM ();			// ToDo 3.3: Enable TPM0 initialization
	UART0_Init();		// Init UART0
	//PTB->PCOR = (1<<8);
	PTB->PSOR = (1<<9);
	PTB->PSOR = (1<<10);
	while(1)
	{
		__WFI();
		if(rx_FULL)		// Is data ready?
		{
			if(too_long)
			{
				for(i=0;Too_Long[i]!=0;i++)	// Too long
					{
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
						UART0->D = Too_Long[i];
					}
					while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is RX ready
					UART0->D = 0xa;		// Next line
					too_long=0;
			}
			else
			{
				if(strcmp (rx_buf,LED_R_ON)==0)	// Turn on red LED 
					TPM0_SetVal(100);
				else
					if(strcmp (rx_buf,LED_R_OFF)==0)
						TPM0_SetVal(0);		// Turn off red LED
				else
					if(strcmp(rx_buf,LED_R_BRI)==0){
							brightness += 25;
							if(brightness >= 100){
								brightness = 100;
							}
							TPM0_SetVal(brightness);}
				else
					if(strcmp(rx_buf,LED_R_DIM)==0){
						brightness -= 25;
							if(brightness <= 0){
								brightness = 0;
							}
						TPM0_SetVal(brightness);}
					else
					{
						for(i=0;Error[i]!=0;i++)	// Zła komenda
						{
							while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
							UART0->D = Error[i];
						}
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
						UART0->D = 0xa;		// Next line
					}
				}
			rx_buf_pos=0;
			rx_FULL=0;	// Dana taken
		}
	}
}
