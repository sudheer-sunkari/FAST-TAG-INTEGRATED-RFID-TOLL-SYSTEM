#include <LPC21xx.H>              // Includes LPC21xx microcontroller register definitions
#include <string.h>               // Includes string handling functions

#define UART_INT_ENABLE 1         // Enables UART interrupt functionality


void InitUART0 (void);            // Function prototype for initializing UART0
void UART0_Tx(char ch);           // Function prototype for transmitting one character through UART0
char UART0_Rx(void);              // Function prototype for receiving one character through UART0


char buff[20],dummy;              // buff stores received RFID data and dummy stores unused UART interrupt information
unsigned char ch,r_flag=0;        // ch stores the received character and r_flag indicates RFID frame reception status
int i=0;                          // Index used to store received RFID characters in buff


// UART0 interrupt service routine
void UART0_isr(void) __irq
{
  if((U0IIR & 0x04))              // Checks whether the UART0 interrupt was caused by received data
  {
		ch = U0RBR;                 // Reads the received character from UART0 and clears the receive interrupt

		if(ch == 0x02)              // Checks for the RFID frame start character STX
		r_flag=1;                   // Sets the flag to indicate that RFID data reception has started

		else if	((r_flag==1)&&(ch!=0x03)) // Checks whether data reception is active and received character is not ETX
		{
			buff[i++] = ch;         // Stores the received RFID character in the buffer and increments the index
			//UART0_Tx(ch);          // Previously used to transmit the received character back through UART0
		}

		else                        // Executes when the RFID frame end character ETX is received
		{
			buff[i] = '\0';          // Adds null character to terminate the received RFID string
			i=0;                     // Resets the buffer index for the next RFID card
			r_flag =2;               // Indicates that complete RFID data has been received
		}
	}
  else
  {
      dummy=U0IIR;                // Reads the UART interrupt identification register to clear a transmit-related interrupt
  }

   VICVectAddr = 0;               // Dummy write to signal the end of the interrupt service routine
}


// Initializes UART0
void InitUART0 (void)
{  
            		
  PINSEL0|= 0x00000005;           // Configures P0.0 as TXD0 and P0.1 as RXD0

  U0LCR = 0x83;                  // Enables DLAB and configures UART for 8 data bits, no parity, 1 stop bit

  U0DLL = 97;                    // Loads the divisor value for approximately 9600 baud rate

  U0LCR = 0x03;                  // Clears DLAB while retaining 8-bit, no parity, 1 stop bit configuration
  
  #if UART_INT_ENABLE > 0        // Compiles the following UART interrupt configuration when UART interrupts are enabled

  VICIntSelect = 0x00000000;     // Configures all selected VIC interrupts as IRQ interrupts

  VICVectAddr0 = (unsigned)UART0_isr; // Assigns the UART0 ISR address to VIC vector slot 0

  VICVectCntl0 = 0x20 | 6;       // Enables vector slot 0 and assigns UART0 interrupt source number 6

  VICIntEnable = 1 << 6;         // Enables UART0 interrupt in the VIC
 
 // U0IIR = 0xc0;                // Previously used UART interrupt identification configuration
 // U0FCR = 0xc7;                // Previously used UART FIFO control configuration

  U0IER = 0x03;                  // Enables UART0 receive data and THRE interrupts
             
  #endif                        // Ends the UART interrupt enable conditional compilation block
						
}


// Transmits one character through UART0
void UART0_Tx(char ch)
{ 
  while (!(U0LSR & 0x20));       // Waits until the UART transmit holding register is empty
  U0THR = ch;                    // Loads the character into the UART transmit holding register
}


// Receives one character through UART0
char UART0_Rx(void)
{                     
  while (!(U0LSR & 0x01));       // Waits until a character is available in the UART receive buffer
  return (U0RBR);                // Reads and returns the received character
}


// Transmits a null-terminated string through UART0
void UART0_Str(char *s)
{
   while(*s)                      // Continues until the null character is reached
       UART0_Tx(*s++);            // Transmits the current character and moves the pointer to the next character
}


// Transmits an unsigned integer through UART0
void UART0_Int(unsigned int n)
{
  unsigned char a[10]={0};        // Array used to store individual digits of the integer
  int i=0;                        // Index used for storing digits

  if(n==0)                        // Checks whether the number is zero
  {
    UART0_Tx('0');                // Transmits character '0'
	return;                        // Exits the function
  }
  else
  {
     while(n>0)                   // Separates the integer into individual digits
	 {
	   a[i++]=(n%10)+48;           // Converts the current digit into its ASCII representation
	   n=n/10;                     // Removes the last digit from the number
	 }

	 --i;                          // Moves index to the last stored digit

	 for(;i>=0;i--)                // Transmits digits in reverse order to obtain the correct number
	 {
	   UART0_Tx(a[i]);             // Transmits the current digit through UART0
	 }
   }
}


// Transmits a floating-point number through UART0
void UART0_Float(float f)
{
  int x;                          // Variable used to store the integer part and decimal digits
  float temp;                     // Temporary variable used to calculate the decimal part

  x=f;                            // Extracts the integer part of the floating-point number
  UART0_Int(x);                   // Transmits the integer part through UART0

  UART0_Tx('.');                  // Transmits the decimal point

  temp=(f-x)*100;                 // Extracts two decimal digits from the fractional part
  x=temp;                         // Converts the fractional part into an integer
  UART0_Int(x);                   // Transmits the decimal digits
}


// Generates a software delay in seconds
void  DelayS(unsigned int  dly)
{  
   unsigned int  i;               // Loop counter used for generating the delay

   for(; dly>0; dly--)             // Repeats the delay loop for the requested number of seconds
      for(i=12000000; i>0; i--);   // Executes a large number of iterations to create an approximate one-second delay
}
