/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 5 - Ćwiczenie 1: wyzwalanie programowe przetwornika A/C - tryb wyzwalania automatycznego
					autor: Mariusz Sokołowski
					wersja: 07.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float volt_coeff = ((float)(((float)2.91) / 4095) );			// transform coefficient referencyjnego przetwornika
uint8_t wynik_ok=0;
uint16_t temp;
float	wynik;

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];	// Read data, clear COCO flag
	if(!wynik_ok)				// Check if result processed in main()
	{
		wynik = temp;			// Send new value
		wynik_ok=1;
	}
	NVIC_EnableIRQ(ADC0_IRQn);
}
int main (void)
{
	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 // LCD
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");				// Control prompt
	
	kal_error=ADC_Init();				// A/C init and calibration
	if(kal_error)
	{	
		while(1);						// Calib. error
	}
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12);		// Trigger ADC. Enable IRQ
	
	while(1)
	{
		if(wynik_ok)
		{
			wynik = wynik*volt_coeff;		// Convert result
			sprintf(display,"U=%.4fV",wynik);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			wynik_ok=0;
		}
	}
}
