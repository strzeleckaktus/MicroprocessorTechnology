#include "ADC.h"

uint8_t ADC_Init(void)
{
	uint16_t kalib_temp;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;          // Enable clock for ADC0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;	        // Enable clock for port A
	PORTA->PCR[0] &= ~(PORT_PCR_MUX(7));		// PTA0 - analog input, channel 12

	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_4) | ADC_CFG1_ADLSMP_MASK;	// ADCK = 2.62MHz (2621440Hz)
	ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;		// High freq. support
	ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);		// Averaging 32
	//ADC0->SC2 = ADC_SC2_REFSEL(REFSEL_EXT);		// External reference voltage
	
	ADC0->SC3 |= ADC_SC3_CAL_MASK;				// Calibration
	while(ADC0->SC3 & ADC_SC3_CAL_MASK);		// Wait
	
	if(ADC0->SC3 & ADC_SC3_CALF_MASK)
{
	  ADC0->SC3 |= ADC_SC3_CALF_MASK;
	  return(1);								// Error
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
	kalib_temp |= 0x8000;                       // Set MSB
	ADC0->PG = ADC_PG_PG(kalib_temp);           // Save plus-side gain calibration register"
	//ADC0->OFS = 0;							// Zero offset calibration
	ADC0->SC1[0] = ADC_SC1_ADCH(31);
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_1) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(MODE_12);	// ADCK =10.49MHz (10485760Hz), 12 bit, long sampling time
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;							// Continous processing
	//ADC0->SC2 |= ADC_SC2_ADTRG_MASK;						// Hardware triggering
	//SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK | SIM_SOPT7_ADC0TRGSEL(4);		// PIT0 triggering
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
	
	return(0);
}

