/*
 * UART.c
 *
 *      Author: ABD ALLAH
 */

#include "UART.h"

#include "avr/io.h"
#include "common_macros.h"

#define F_CPU 8000000UL

void UART_init(const UART_ConfigType * Config_Ptr){

	uint16 UBRR_value = 0;

	// U2X = 1 for double transmission speed
	UCSRA = (1<<U2X);

	//RXEN  = 1 Receiver Enable , RXEN  = 1 Transmitter Enable
	UCSRB = (1<<RXEN) | (1<<TXEN);

	//URSEL   = 1 The URSEL must be one when writing the UCSRC
	UCSRC = (1<<URSEL);

	//SET DATA_BITS IN BITS UCSZ0,UCSZ1
	UCSRC=(UCSRC & 0xF9) | (Config_Ptr->bit_data << 1);

	//SET PARITY IN BITS UPM0 , UPM1
	UCSRC=(UCSRC & 0xCF) | (Config_Ptr->parity << 4);

	//SET STOP BIT IN BIT USBS
	UCSRC=(UCSRC & 0xF7) | (Config_Ptr->stop_bit << 3);

	//Calculate the UBRR register value
	UBRR_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

	//First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH
    UBRRL=UBRR_value;
    UBRRH=UBRR_value>>8;
}


// Functional responsible for send byte to another UART device.

void UART_sendByte(const uint8 data)
{
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

}


// Functional responsible for receive byte from another UART device.

uint8 UART_recieveByte(void)
{
	// RXC flag is set when the UART receive data so wait until this flag is set to one
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;
}


//Send the required string through UART to the other UART device.

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	// Send the whole string
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}

}


// Receive the required string until the '#' symbol through UART from the other UART device.

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	// Receive the first byte
	Str[i] = UART_recieveByte();

	// Receive the whole string until the '#'
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	// After receiving the whole string plus the '#', replace the '#' with '\0'
	Str[i] = '\0';
}
