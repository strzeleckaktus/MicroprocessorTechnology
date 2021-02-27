/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 5 - Ćwiczenie 7: regulator zmierzchowy
					autor: Mariusz Sokołowski
					wersja: 07.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "DAC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float adc_volt_coeff = ((float)(((float)2.91) / 4095) );			// Correction factor of the result, in the future to the reference voltage of the converter
uint8_t wynik_ok=0;
uint16_t temp;
float wynik;

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		// Read and clear COCO flag
	if(!wynik_ok)					// Check the result consumed by the master loop
	{
		wynik = temp;				// Send the result to the main loop
		wynik_ok=1;
	}
	DAC_Load_Trig(temp);	// Loading new data and trigger ADC 
	
}
int main (void)
{
	uint8_t	kal_error;

	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

	LCD1602_Init();		 // LCD display initialization
	LCD1602_Backlight(TRUE);

	LCD1602_SetCursor(0,0);
	LCD1602_Print("Uk=");
	
	kal_error=ADC_Init();		// ADC initialization
	if(kal_error)
	{
		while(1);							// Calibration error
	}
	DAC_Init();		// DAC initialization
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(4);		// Initial ADC0 trigger on channel 4
	
	while(1)
	{
		if(wynik_ok)
		{
			wynik = wynik*adc_volt_coeff;		// Adjusting the result to the voltage range
			sprintf(display,"%.3fV",wynik);
			LCD1602_SetCursor(3,0);
			LCD1602_Print(display);
			wynik_ok=0;
		}
	}
}
