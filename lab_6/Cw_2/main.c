/*-------------------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 5 - Ćwiczenie 2: wyzwalanie programowe przetwornika A/C - tryb wyzwalania na żądanie
					autor: Mariusz Sokołowski
					wersja: 07.11.2020r.
---------------------------------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float adc_volt_coeff = ((float)(((float)2.91) / 4095) );			// Result correction factor in relation to the reference voltage of the converter
uint8_t wynik_ok=0;
uint16_t temp;
float	wynik;

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];										// Reading the data and clearing the COCO flag
	if(!wynik_ok)													// Check if the result consumed by the main loop
	{
		wynik = temp;												// Send new data to the main loop
		wynik_ok=1;
	}
		ADC0->SC1[0] |= ADC_SC1_ADCH(4);		// Program trigger of the ADC0 converter on channel 4
}
int main (void)
{
	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 					// LCD initialization
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");				// The control screen - will not disappear if the rest of the program is not working
	
	kal_error=ADC_Init();				// Initialization and calibration of the A / C converter
	if(kal_error)
	{
		while(1);									// Calibration error
	}
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(4);		// Initial trigger ADC0 on channel 4 and enable the interrupt
	
	while(1)
	{
		if(wynik_ok)
		{
			wynik = wynik * adc_volt_coeff;		// Adjusting the result to the voltage range
			sprintf(display,"U=%.4fV",wynik);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			wynik_ok=0;
		}
	}
}
