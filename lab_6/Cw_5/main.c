/*-----------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 5 - Ćwiczenie 5: współpraca przetwornika A/C z prztwornikiem C/A
					autor: Mariusz Sokołowski
					wersja: 07.11.2020r.
------------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "DAC.h"
#include "pit.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float adc_volt_coeff = ((float)(((float)2.91) / 4095) );			// Result correction factor in relation to the reference voltage of the converter
uint8_t wynik_ok=0;
uint16_t temp, dac_old;
uint16_t dac=0;
float	wynik, dac_nap;

void PIT_IRQHandler()
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		// Clear the interrupt request flag
}

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		// Reading the data and clearing the COCO flag
	if(!wynik_ok)					// Check if the result consumed by the main loop
	{
		wynik = temp;				// Send new data to the main loop
		dac_old=dac;
		if(dac>4095)
			dac=0;
		else
			dac+=1;
		DAC_Load_Trig(dac);		// Setting a new value of the DAC
		wynik_ok=1;
	}	
}
int main (void)
{
	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 // IInitialization of LCD
	LCD1602_Backlight(TRUE);
	LCD1602_Print("---");		// The control screen - will not disappear if the rest of the program is not working
	
	kal_error=ADC_Init();		// nitialization and calibration of ADC
	if(kal_error)
	{
		while(1);							// Calibration error
	}
	DAC_Init();						// DAC initialization
	DAC_Load_Trig(dac);		// Triggering the C / A converter with the initial value
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(4);		// Enable interrupt and select channel 4
	PIT_Init();			// PIT0 init
	
	while(1)				// Calibration error
	{
		if(wynik_ok)
		{
			wynik = wynik*adc_volt_coeff;				// Adjusting the result to the voltage range
			sprintf(display,"ADC=%.4fV",wynik);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			dac_nap = dac_old*adc_volt_coeff;		// Adjusting the value to the voltage range
			sprintf(display,"DAC=%.4fV",dac_nap);
			LCD1602_SetCursor(0,1);
			LCD1602_Print(display);
			wynik_ok=0;
		}
	}
}
