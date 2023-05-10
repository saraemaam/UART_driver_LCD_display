/******************************************************************************
 * Module: UART
 * File Name: MCU1.c
 * Description: Test functionality of UART using another mcu
 * Author: Sarah
 *******************************************************************************/

#include "uart.h"
#include<avr/io.h>

#define MCU2_IS_READY  0x10

int main(void)
{

	uint8 Str[20];
	uint8 data;
	uart_ConfigType uart_configuration = { BAUD_RATE_9600,_8BIT_MODE,_1STOP_BIT,DISABLED};

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&uart_configuration);

	while( UART_recieveByte() != MCU2_IS_READY);
	UART_sendString("I am micro1#");

	while(1)
	{

	}
}
