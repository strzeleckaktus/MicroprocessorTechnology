#include "ADC.h"

uint8_t ADC_Init(void)
{
	uint16_t kalib_temp;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;          // Enable clock for ADC0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;					// Enable clock for portu B module
	PORTB->PCR[2] &= ~(1<<2);										// Select PTB2 as ADC output channel 4
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_4) | ADC_CFG1_ADLSMP_MASK;	// // Conversion clock BUS/2=10.49MHz, clock ADCK = 2.62MHz (2621440Hz), long conversion time
	ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;										// Enable fast clock support mode 
	ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);		// Enable averaging 32 samples
	ADC0->SC3 |= ADC_SC3_CAL_MASK;											// Start calibration
	while(ADC0->SC3 & ADC_SC3_CAL_MASK);								// Wait during calibration
	
	if(ADC0->SC3 & ADC_SC3_CALF_MASK)
{
	  ADC0->SC3 |= ADC_SC3_CALF_MASK;
	  return(1);																				// error, return
	}
	
	kalib_temp = 0x00;
	kalib_temp += ADC0->CLP0;
	kalib_temp += ADC0->CLP1;
	kalib_temp += ADC0->CLP2;
	kalib_temp += ADC0->CLP3;
	kalib_temp += ADC0->CLP4;
	kalib_temp += ADC0->CLPS;
	kalib_temp += ADC0->CLPD;
	kalib_temp /= 2;
	kalib_temp |= 0x8000;                       // Set Most Significant Bit (MSB)
	ADC0->PG = ADC_PG_PG(kalib_temp);           // Save to plus-side gain calibration register
	//ADC0->OFS = 0;													  // Zero offset calibration
	ADC0->SC1[0] = ADC_SC1_ADCH(31);						// Disable ADC0
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;					// Fast conversion mode
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_1) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(MODE_12);	// Clock ADCK = 10.49MHz, resolution = 12 bit, long sampling time
	ADC0->SC3 = 0;															
	ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);		// Averaging 32 samples
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;											// Continous mode 
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
	return(0);																	// Success, return
}

