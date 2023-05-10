/****************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header File for AVR uart driver
 *
 * Author: Sarah
 ******************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h"
#include <avr/interrupt.h>


#if (UART_RECEIVE_IMPLEMENTATION_BASED == UART_INTERRUPT_BASED)
/**************************************************************
 *                         Global Variable
 ***************************************************************/
volatile uint8 g_UART_result = 0 ;


/**************************************************************
 *                           ISR
 *************************************************************/

ISR (USART_RXC_vect)
{
	g_UART_result = UDR;

}

#endif


/*******************************************************************************
 *                      Functions definitions                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const uart_ConfigType *Config_Ptr){

	uint16 ubrr_value =0;

	/* Double Speed or Normal mode*/
#ifdef ASYNCHRONOUS_DOUBLE_SPEED_MODE
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

#endif

	/************************** UCSRB Description **************************
	 * RXCIE Disable/Enable USART RX Complete Interrupt Enable
	 * TXCIE Disable/Enable USART Tx Complete Interrupt Enable
	 * UDRIE Disable/Enable USART Data Register Empty Interrupt Enable
	 ***********************************************************************/

#if (UART_TRANSMIT_IMPLEMENTATION_BASED == UART_POOLLING_BASED)

	CLEAR_BIT(UCSRB,TXCIE);
	CLEAR_BIT(UCSRB,UDRIE);

#elif (UART_TRANSMIT_IMPLEMENTATION_BASED == UART_INTERRUPT_BASED)

	SET_BIT(UCSRB,TXCIE);
	SET_BIT(UCSRB,UDRIE);

#endif

#if (UART_RECEIVE_IMPLEMENTATION_BASED == UART_POOLLING_BASED)

	CLEAR_BIT(UCSRB,RXCIE);

#elif (UART_RECEIVE_IMPLEMENTATION_BASED == UART_INTERRUPT_BASED)

	SET_BIT(UCSRB,RXCIE);

#endif

	/*
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * RXB8 & TXB8 not used for 8-bit data mode
	 */
	SET_BIT(UCSRB,RXEN);
	SET_BIT(UCSRB,TXEN);

	/* UCSZ2 = 0 For 5,6,7,8-bit data mode in case of 9bit mode
	 * UCSZ2 = 1 */
	CLEAR_BIT(UCSRB,UCSZ2);

	/************************** UCSRC Description **************************
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	/*URSEL   = 1 The URSEL must be one when writing the UCSRC*/
	SET_BIT(UCSRC,URSEL);

	/*Number of bits is configurable
	 * UCSZ1:0 = 11 For 8-bit data mode*/

	UCSRC = (UCSRC & 0xF9)| ((Config_Ptr->num_of_data_bits)<<1);

	/*UMSEL   = 0 Asynchronous Operation*/
#if ( UART_SYNCHRONOUS_ASYNCHRONOUS_MODE == ASYNCHRONOUS_MODE)

	/*if Clock polarity asynchronous that's mean this bit UMSEL=0*/
	CLEAR_BIT(UCSRC,UMSEL);

#elif
	/*if Clock polarity synchronous that's mean this bit UMSEL=1*/
	SET_BIT(UCSRC,UMSEL);

#endif

	/* Parity mode settings UPM1:0  = 00 Disable parity bit */
	UCSRC = (UCSRC & 0xCF) | ((Config_Ptr->Parity_mode)<<4);

	/* Stop bits settings */
	UCSRC = (UCSRC & 0xF7) | ((Config_Ptr->stop_bits)<<3);

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)( (F_CPU / (( Config_Ptr->Baudrate )* 8UL) ) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data){

	/* Polling based*/
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
		UDR = data;
		while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
		SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/

}
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */


#if (UART_RECEIVE_IMPLEMENTATION_BASED == UART_INTERRUPT_BASED)
uint8 UART_recieveByte(void){

	return g_UART_result;
	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */

}
#else

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

#endif

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){

	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
		while(*Str != '\0')
		{
			UART_sendByte(*Str);
			Str++;
		}
	 *******************************************************************/
}



/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */

void UART_receiveString(uint8 *Str){

	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';

}
