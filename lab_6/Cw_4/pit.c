#include "MKL05Z4.h"
#include	"pit.h"

void PIT_Init(void)
{
	uint32_t tsv;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			// Enable clock for PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Enable PIT
	tsv=BUS_CLOCK;												// Interrupt period 1s
	//tsv=BUS_CLOCK/2;										// Interrupt period 0.5s
	//tsv=BUS_CLOCK/10;										// Interrupt period 100ms
	//tsv=BUS_CLOCK/100;									// Interrupt period co 10ms
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);		// Intial value load
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Enable interrunts and start counter
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);	
}