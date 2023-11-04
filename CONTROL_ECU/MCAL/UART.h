
/*
 * UART.h
 *
 *      Author: ABD ALLAH
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"




typedef enum{

DATA_BIT_5,DATA_BIT_6,DATA_BIT_7,DATA_BIT_8

}UART_BitData;

typedef enum{

	DISABLED,RESERVED,EVEN_PARITY,ODD_PARITY

}UART_Parity;

typedef enum{

  STOP_BIT_1,STOP_BIT_2

}UART_StopBit;

typedef enum{

BAUD_RATE_9600=9600,BAUD_RATE_14400=14400,BAUD_RATE_19200=19200,BAUD_RATE_115200=115200

}UART_BaudRate;


 typedef struct{

	 UART_BitData bit_data;
	 UART_Parity parity;
	 UART_StopBit stop_bit;
	 UART_BaudRate baud_rate;

 }UART_ConfigType;


 void UART_init(const UART_ConfigType * Config_Ptr);

 void UART_sendByte(const uint8 data);

 uint8 UART_recieveByte(void);

 void UART_sendString(const uint8 *Str);

 void UART_receiveString(uint8 *Str);


#endif /* UART_H_ */
