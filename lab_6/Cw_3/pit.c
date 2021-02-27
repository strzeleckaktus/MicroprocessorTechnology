#include "MKL05Z4.h"
#include	"pit.h"


void PIT_Init(void)
{
	uint32_t tsv;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			// Enable clock for PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Enable PIT
	tsv=BUS_CLOCK;												// Set interrupt period = 1s
	//tsv=BUS_CLOCK/10;										// Set interrupt period = 100ms
	//tsv=BUS_CLOCK/100;									// Set interrupt period = 10ms
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);		// Init TSV value
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Enable interrupt and timer
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);	
}
