#ifndef _UART_DEFINES_H_

#define _UART_DEFINES_H_


#define UART_INT_ENABLE 1

/*

BAUDRATE FORMULA:

BAUD RATE = PCLK/(16*DIVISOR);

DIVISOR = (U0DLM*256) + U0DLL;

- DIVISOR LOWER BYTE NEED TO LOAD IN TO U0DLL

- DIVISOR HIGHER BYTE NEED TO LOAD IN TO U0DLM

*/

#define FOSC      12000000

#define CCLK  	  5*FOSC

#define PCLK  	  CCLK/4

#define BAUD  	  9600

#define DIVISOR   (PCLK/(16 * BAUD))

//defines for UART

#define TXD0_EN   1<<0

#define RXD0_EN   1<<2


//bit defines for U0LCR

#define DLAB_BIT 7


//bit defines for U0LSR

#define RDR_BIT  0

#define THRE_BIT 5

#define TEMT_BIT 6


#endif
