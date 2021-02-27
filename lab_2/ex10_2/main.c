#include "MKL05Z4.h" /*Device header*/
#define RED_LED_POS 8
#define GREEN_LED_POS 9
#define BLUE_LED_POS 10
#define BUTTON_1_POS 1
#define BUTTON_2_POS 2
#define BUTTON_3_POS 3
#define ALL_BTN_MASK ((1 << BUTTON_1_POS)|(1 << BUTTON_2_POS)|(1 <<BUTTON_3_POS))
void delay_ms( int n) {
for( int i = 0 ; i < 3500*n; i++){}
}
int main()
{
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable clock for GPIO B */
	for(int i = 1; i <=3;i++){
		PORTB->PCR[i] |= PORT_PCR_MUX(1); /* Set Pin 1 MUX as GPIO */
		PORTB->PCR[i] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
		PORTB->PCR[i+7] |= PORT_PCR_MUX(1); /* Set Pin 8 MUX as GPIO  */
	}
PTB->PDDR |= (0b111<<RED_LED_POS); /* Set RED LED pin as output */
PTB->PSOR |= (0b111<<RED_LED_POS); /* Turn off RED LED */
while(1){
	
for (;;) {
                if ((PTB->PDIR & ALL_BTN_MASK) != ALL_BTN_MASK) {
                        PTB->PTOR = ((PTB->PDIR & ALL_BTN_MASK) ^ ALL_BTN_MASK) <<(RED_LED_POS - BUTTON_1_POS);
                   while ((PTB->PDIR & ALL_BTN_MASK) != ALL_BTN_MASK);
                }
        }
}
}