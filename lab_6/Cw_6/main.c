/*---------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 5 - Ćwiczenie 6: Sterowanie jasnością diody LED za pomocą przetwornika C/A
					autor: Mariusz Sokołowski
					wersja: 07.11.2020r.
-----------------------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "DAC.h"
#include "tsi.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float adc_volt_coeff = ((float)(((float)2.91) / 4095) );			// Result correction factor in relation to the reference voltage of the converter
uint8_t wynik_ok=0;
uint16_t temp, slider;
uint16_t dac;
float	wynik;

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];	// Reading data and clearing the COCO flag
	if(!wynik_ok)				// Check that the result is consumed by the master loop
	{
		wynik = temp;			// Send new data to the main loop
		wynik_ok=1;
	}
}
int main (void)
{
	uint8_t	kal_error;
	slider=50;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	TSI_Init();					// Init Touch Slider
	LCD1602_Init();		 	// Init LCD display
	LCD1602_Backlight(TRUE);
	sprintf(display,"Slider=%02d",slider);
	LCD1602_SetCursor(0,0);
	LCD1602_Print(display);
	LCD1602_SetCursor(0,1);
	LCD1602_Print("Uk=");
	
	kal_error=ADC_Init();		// Initialization and calibration of ADC
	if(kal_error)
	{
		while(1);							// Calibration error
	}
	DAC_Init();		// DAC initialiaztion
	dac=43*slider;	// Extension of the touch field range to 12 bits
	DAC_Load_Trig(dac);	// Triggering the C / A converter with the initial value
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(4);		// Initial trigger for ADC0 on channel 4 and interrupt enabled
	
	while(1)
	{
		if(wynik_ok)
		{
			slider=TSI_ReadSlider();
			if(slider!=0)
			{
				sprintf(display,"%02d",slider);
				LCD1602_SetCursor(7,0);
				LCD1602_Print(display);
				slider-=2;		// Correction of the range of variability
				dac=43*slider;	// Extension of the touch field range to 12 bits
				DAC_Load_Trig(dac);		// Setting a new value of the DAC
			}
			wynik = wynik*adc_volt_coeff;		// Adjusting the raw result to the voltage range
			sprintf(display,"%.3fV",wynik);
			LCD1602_SetCursor(3,1);
			LCD1602_Print(display);
			wynik_ok=0;
		}
	}
}
