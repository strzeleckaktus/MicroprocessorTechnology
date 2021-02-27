/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Tutorial (C).            *
 ******************************************************************************/

/**
 * @file main.c
 * @author Koryciak
 * @date Sep 2020
 * @brief File containing the main function.
 * @ver 0.1
 */

#include "frdm_bsp.h"
#include "led.h"
#include "lcd1602.h"
#include "tsi.h"


/**
 * @brief The main loop.
 *
 * @return NULL
 */
int main (void) {
	
	TSI_Init ();       /* initialize slider   */
  LED_Init ();       /* initialize all LEDs */
	LCD1602_Init();		 /* initialize LCD		  */
	LCD1602_Backlight(TRUE);
	
	LED_Welcome();
	LCD1602_SetCursor(3,0);
	LCD1602_Print("- A G H -");
	LCD1602_SetCursor(3,1);
	LCD1602_Print("KE @ WIET");
	
	DELAY(1000)
	
	LED_Welcome();
	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	LCD1602_Print("Microprocessor");
	LCD1602_SetCursor(0,1);
	LCD1602_Print("Technology");
	
  while(1) if (TSI_ReadSlider() > 0) LED_Blink(LED_RED, 100);
}
