#include "DAC.h"

void DAC_Init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;          // Enable clock DAC0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	DAC0->C0 |= (DAC_C0_DACEN_MASK | DAC_C0_DACTRGSEL_MASK);
}

uint8_t DAC_Load_Trig(uint16_t load)
{
	uint8_t load_temp;
	if(load>0xFFF)	return (1);		// Check input data range
	load_temp=load&0x0FF;
	DAC0->DAT->DATL = load_temp;	// Load Least Significant Byte to DAC
	load_temp=(load>>8);
	DAC0->DAT->DATH = load_temp;	// Load Most Significant Byte to DAC
	DAC0->C0 |= DAC_C0_DACSWTRG_MASK;		// Trigger DAC
	return (0);
}
