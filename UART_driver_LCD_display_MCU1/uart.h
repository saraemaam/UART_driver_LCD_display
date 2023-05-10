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
#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************
 *                         Definitions
 *******************************************************************/


#define ASYNCHRONOUS_DOUBLE_SPEED_MODE
#define UART_SYNCHRONOUS_ASYNCHRONOUS_MODE  ASYNCHRONOUS_MODE

#define UART_TRANSMIT_IMPLEMENTATION_BASED  UART_POOLLING_BASED
#define UART_RECEIVE_IMPLEMENTATION_BASED   UART_POOLLING_BASED

/* UART receive or transmit based on Interrupt or polling technique*/
#define UART_INTERRUPT_BASED     1
#define UART_POOLLING_BASED      2

#define SYNCHRONOUS_MODE         0
#define ASYNCHRONOUS_MODE        1




/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	BAUD_RATE_9600=9600,BAUD_RATE_14400=1400,BAUD_RATE_19200=19200

}uart_Baudrate;


/* 2 BITS USED FOR CHACTER SIZE SETTINGS*/
typedef enum {
	_5BIT_MODE,_6BIT_MODE,_7BIT_MODE,_8BIT_MODE,_9BIT_MODE=3

}uart_character_size;



/* STOP BITS ONLY 1 BIT USED */
typedef enum {
	_1STOP_BIT,_2STOP_BIT

}uart_stop_bits;

/*2 BITS FOR PARITY SETTINGS*/
typedef enum {
	DISABLED,ENABLED_EVEN=2,ENABLED_ODD=3

}uart_Parity_Mode;



typedef struct {


	uart_Baudrate Baudrate;
	uart_character_size num_of_data_bits;
	uart_stop_bits stop_bits;
	uart_Parity_Mode Parity_mode;

}uart_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const uart_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */

uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
