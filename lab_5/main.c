/**************************************************************
 * This file is a part of the PWM Demo (C).                   *
 **************************************************************/
/**
 * @file main.c
 * @author Koryciak
 * @date Nov 2020 
 * @brief File containing the main function. 
 * @ver 0.1
 */
 
#include "frdm_bsp.h" 
#include "led.h" 
#include "lcd1602.h" 
#include "tsi.h"
#include "pit.h"
#include "tpm.h"
//#include "tpm.h" // ToDo 2.2: Attach TPM header

/******************************************************************************\
* Private prototypes
\******************************************************************************/
void SysTick_Handler(void);
void lcd_static(void);
void lcd_update(void);
/******************************************************************************\
* Private memory declarations
\******************************************************************************/
static uint8_t sliderVal = 50;
static uint8_t sliderOld = 0;
static uint8_t msTicks = 0;
static uint8_t newTick = 0;

/**
 * @brief The main loop. 
 * 
 * @return NULL 
 */
int main (void) { 
	
	uint8_t sliderTemp;
	
	TSI_Init ();  													/* initialize slider */ 
	
	LED_Init ();	 													/* initialize all LEDs */ 
	LED_Welcome();  												/* blink with all LEDs */

	LCD1602_Init (); 												/* initialize LCD */ 
	lcd_static(); 													/* display default on LCD */
	
	PIT_Init_Generator (); 														// ToDo 3.2: Disable PIT initialization

	SysTick_Config(1000000); 								/* initialize system timer */
	
	TPM1_Init_InputCapture ();			// ToDo 2.2: Enable TPM1 initialization  // ToDo 3.2: Disable TPM1 initialization

	//TPM0_Init_PWM ();			// ToDo 3.3: Enable TPM0 initialization
	
  while(1) {

		__WFI();															/* sleep & wait for interrupt */

		// simple schedule
		if (newTick) {
			newTick = 0;												/* clear flag & choose task */
			// task 1 - read slider
			if( msTicks%2 == 0 ) {
				sliderTemp = TSI_ReadSlider();
				if (sliderTemp > 0) {
					sliderVal = sliderTemp;
					PIT_SetTSV(sliderVal); 		/* change frequency */
//				TPM0_SetVal(sliderVal);  // ToDo 3.3: Activate setting CnV in TPM0
				}
			}
			// task 2 - refresh display
			if( msTicks%5 == 0) {
				lcd_update();
			}
		}
	} /* end_while */
}
/**
 * @brief System time update. 
 */
void SysTick_Handler(void) {
	msTicks++;
	newTick = 1;
}
/**
 * @brief Display on LCD static text and default values. 
 */
void lcd_static(void) {
	
	LCD1602_ClearAll();
	LCD1602_SetCursor(0, 0);
	LCD1602_Print("Slider = ");
	LCD1602_SetCursor(0, 1);
	LCD1602_Print("TPM    = "); // ToDo 2.5: Enable to see result on LCD
	lcd_update();								// ToDo 2.5: Enable to see result on LCD
}
/**
 * @brief Update values on LCD. 
 */
void lcd_update(void) {
	
	if (sliderVal != sliderOld) {
		sliderOld = sliderVal;
		LCD1602_SetCursor(9, 0);
		LCD1602_PrintNum(sliderOld);
	}
	LCD1602_SetCursor(9, 1);  // ToDo 2.5: Enable to see result on LCD
	LCD1602_PrintNum((int)1/(TPM1_GetVal()*0.0000053)); // ToDo 2.5: Enable to see result on LCD
}
