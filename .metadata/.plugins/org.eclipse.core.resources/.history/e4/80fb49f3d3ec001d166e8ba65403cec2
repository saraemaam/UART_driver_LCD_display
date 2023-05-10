/******************************************************************************
 * Module: UART
 * File Name: MCU2.c
 * Description: Test functionality of UART using another mcu
 * Author: Sarah
 *******************************************************************************/

#include "uart.h"
#include"lcd.h"

#define MCU2_IS_READY  0x10

int main(void)
{
	uint8 Str[20];
	uint8 data;
	uart_ConfigType uart_configuration = { BAUD_RATE_9600,_8BIT_MODE,_1STOP_BIT,DISABLED};

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&uart_configuration);

	LCD_init();


	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_sendByte(MCU2_IS_READY);

	/* Receive String from MC1 through UART */
	UART_receiveString(Str);

	/* Display the received string on the LCD display */
	LCD_displayString(Str);
	while(1)
	{

	}
}
