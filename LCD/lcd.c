#include<lpc21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include"lcd_define.h"              // Includes LCD-related macro definitions
#include"delay.h"                   // Includes delay function declarations
#include"types.h"                   // Includes user-defined data types such as u8, u32, f32, and s32


// Sends an 8-bit command or data value to the LCD
void writeLCD(unsigned int dat)
{
         IOCLR0=1<<LCD_RW;                   // Clears the LCD R/W pin to select write operation
         IOPIN0=(IOPIN0&~(255<<LCD_DATA))|(dat<<LCD_DATA); // Places the 8-bit command/data value on the LCD data pins
          IOSET0=1<<LCD_EN;                   // Sets the LCD Enable pin high to start the data/command transfer
          delay_us(1);                        // Provides the required Enable pulse width
         IOCLR0=1<<LCD_EN;                   // Clears the LCD Enable pin to complete the data/command transfer
         delay_ms(2);                        // Waits for the LCD to complete the command/data operation
}


// Sends a command instruction to the LCD
void cmdLCD(unsigned char cmd)
{
         IOCLR0=1<<LCD_RS;                   // Clears the RS pin to select the LCD command register
          writeLCD(cmd);                      // Sends the command byte to the LCD
}


// Sends an ASCII character to the LCD
void charLCD(unsigned char asciival)
{
        IOSET0=1<<LCD_RS;                   // Sets the RS pin to select the LCD data register
        writeLCD(asciival);                 // Sends the ASCII character to the LCD
}


// Initializes the LCD in 8-bit, 2-line mode
void initLCD(void)
{
        IODIR0|=0XFF<<LCD_DATA;              // Configures the 8 LCD data lines as output pins
        IODIR0|=7<<LCD_RS;                   // Configures the LCD RS, RW, and EN control pins as output pins
        delay_ms(15);                        // Waits for the LCD power-up initialization time

        cmdLCD(0x30);                       // Sends the first initialization command to the LCD
        delay_ms(5);                        // Waits for LCD initialization
        cmdLCD(0x30);                       // Sends the second initialization command
        delay_us(100);                      // Waits for LCD initialization
        cmdLCD(0x30);                       // Sends the third initialization command
        cmdLCD(0x30);                       // Sends another initialization command
        cmdLCD(MODE_8BIT_2LINE);            // Configures the LCD for 8-bit data mode and 2-line operation
        cmdLCD(DSP_ON_CUR_ON);              // Turns ON the LCD display and cursor
        cmdLCD(CLEAR_LCD);                  // Clears the LCD display
        cmdLCD(SHIFT_CUR_RIGHT);            // Configures the cursor movement direction to the right
}


// Displays a null-terminated string on the LCD
void strLCD(char *s)
{
  while(*s)                           // Continues until the null character '\0' is reached
  {
      charLCD(*s++);                      // Sends the current character to the LCD and moves the pointer to the next character
  }
}


// Displays an unsigned 32-bit integer value on the LCD
void u32LCD(unsigned int n)
{
      int i=0;                            // Variable used to count the number of digits
      unsigned char a[11];                // Array used to temporarily store the individual ASCII digits

      if(n==0)                            // Checks whether the number is zero
      {
          charLCD('0');                       // Displays '0' on the LCD
      }
      else
      {
          while(n)                            // Separates the number into individual digits
          {
		            a[i++]=(n%10)+'0';         // Converts the least significant digit into its ASCII representation
		            n=n/10;                    // Removes the least significant digit
		      }
		  
		      /*for(--i;i>=0;i--)
		      {
		      charLCD(a[i]);
		      }	*/
		  
		      while(i>0)                 // Displays the stored digits in reverse order to form the correct number
		      {
		          i--;                     // Moves to the previous stored digit
			        charLCD(a[i]);            // Displays the digit on the LCD
		      }
		  }
 }


// Displays a floating-point value on the LCD with the specified number of decimal places
void f32LCD(f32 f,u8 nDP)
{
      u32 n,i;                            // Variables used for integer and decimal digit processing

      if(f<0.0)                           // Checks whether the floating-point value is negative
      {
          charLCD ('-');                      // Displays the negative sign
          f=-f;                               // Converts the value to positive for further processing
      }

      n=f;                                // Extracts the integer part of the floating-point value
      u32LCD(n);                          // Displays the integer part on the LCD
      charLCD('.');                       // Displays the decimal point

      for(i=0;i<nDP;i++)                  // Loops according to the required number of decimal places
      {
          f=(f-n)*10;                         // Extracts the next decimal digit
          n=f;                                // Stores the decimal digit as an integer
          charLCD(n+48);                      // Converts the decimal digit to ASCII and displays it
      }
}


// Displays a number in binary format on the LCD
void binLCD(u32 n,u8 nBD)
{
        s32 i;                              // Variable used to iterate through the binary bit positions

        for(i=nBD;i>=0;i--);                // Iterates through the requested bit positions
        {
              charLCD((n>>i&1)+48);               // Extracts each bit, converts it to ASCII, and displays it
        }
}


// Displays a number in hexadecimal format on the LCD
void hexLCD(u32 n)
{
      s32 i=0;                            // Variable used to count the number of hexadecimal digits
      u8 a[8],t;                          // Array stores hexadecimal digits and t stores the current digit

      if(n==0)                            // Checks whether the number is zero
      charLCD('0');                       // Displays zero on the LCD
      else
      {
          while(n>0)                          // Separates the number into hexadecimal digits
          {
              t=(n%16);                           // Gets the least significant hexadecimal digit
              (t<10)?(t=t+48):(t=(t-10)+'A');     // Converts the hexadecimal digit into its ASCII representation
                a[i++]=t;                           // Stores the ASCII digit in the array
              n/=16;                              // Removes the least significant hexadecimal digit
                a[i]=n+48;                          // Stores the next value in the array
          }

          for(--i;i>=0;i--)                   // Traverses the hexadecimal digits in reverse order
          charLCD(a[i]);                      // Displays each hexadecimal digit on the LCD
      }
}


// Displays a number in octal format on the LCD
void octLCD(u32 n)
{
        s32 i=0;                            // Variable used to count the number of octal digits
        u8 a[8],t;                          // Array stores octal digits and t stores the current digit

        if(n==0)                            // Checks whether the number is zero
        charLCD('0');                       // Displays zero on the LCD
        else
        {
            while(n>0)                          // Separates the number into octal digits
            {
                ((t=(n%8))<10)?(t=t+48):(t=(t-10)+'A'); // Converts the octal digit into its ASCII representation
                a[i++]=t;                           // Stores the ASCII digit in the array
                n/=8;                               // Removes the least significant octal digit
                a[i]=n+48;                          // Stores the next value in the array
              }
              for(--i;i>=0;i--)                   // Traverses the octal digits in reverse order
              charLCD(a[i]);                      // Displays each octal digit on the LCD
        }
}


// Writes user-defined character data into the LCD CGRAM
void buildCGRAM(u8 *p,u8 nbytes)
{
      u32 i;                              // Loop counter used to process each CGRAM byte

      //point to cgram start
      cmdLCD(GOTO_CGRAM);                 // Sends the command to set the LCD address to the beginning of CGRAM

       for(i=0;i<nbytes;i++)                // Loops through the number of bytes in the custom character data
       {
           //write to cgram via data reg
           charLCD(p[i]);                  // Writes each custom character pattern byte into CGRAM
       }
}
