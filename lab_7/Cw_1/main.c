/*-------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 6 - Ćwiczenie 1: nawiązywanie połączenia z komputerem
					autor: Mariusz Sokołowski
					wersja: 16.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (void)
{
	uint32_t i=0;
	uint8_t temp=0;
	char rx_buf[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
		
	UART0_Init();		// UART0 init
	
	while(1)
	{
			sprintf(rx_buf,"N=%d%c",temp,0xd);
			for(i=0;rx_buf[i]!=0;i++)
			{
				while(!(UART0->S1 & UART0_S1_TDRE_MASK));
				UART0->D = rx_buf[i];
			}
			temp+=1;
			DELAY(500)	
	}
}
