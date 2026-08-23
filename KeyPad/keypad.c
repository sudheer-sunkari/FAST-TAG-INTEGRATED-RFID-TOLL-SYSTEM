#include<lpc21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include"types.h"                   // Includes user-defined data types such as u8 and u32
#include"lcd.h"                     // Includes LCD function declarations such as initLCD(), cmdLCD(), strLCD(), charLCD(), and u32LCD()
#include"lcd_define.h"              // Includes LCD-related macro definitions such as LCD commands and cursor position definitions
#include"define.h"                  // Includes general project macro definitions
#include"kpm_define.h"              // Includes keypad-specific macro definitions
#include"delay.h"                   // Includes delay function declarations


// Lookup table containing the characters assigned to each keypad position
u8 kpmLUT[4][4]={{'1','2','3','+'},
				 {'4','5','6','-'},
				 {'7','8','9','*'},
				 {'*','0','#','='}};


// Checks whether any key is pressed by scanning the keypad column lines
u32 ColScan(void)
{
      return((READNIBBLE(IOPIN1,COL0)<15)? 0:1); // Returns 0 when any column is active and 1 when no key is pressed
}


// Identifies the row in which the key is pressed
u32 ROWcheck(void)
{
      u32 rNO;                            // Variable used to store the row number

      for(rNO=0;rNO<=3;rNO++)              // Scans all four keypad rows one by one
      {
          WRITENIBBLE(IOPIN1,ROW0,~(1<<rNO)); // Drives one row low at a time to identify the pressed row

          if(READNIBBLE(IOPIN1,COL0)!=15)      // Checks whether any column becomes active for the selected row
          {
                break;                              // Stops scanning when the pressed row is identified
          }
      }

      //reinitiazation all rows
      WRITENIBBLE(IOPIN1,ROW0,0);          // Resets all keypad row outputs to their default state

      return rNO;                          // Returns the detected row number
}


// Identifies the column in which the key is pressed
u32 colcheck(void)
{
        u32 cNO;                            // Variable used to store the column number

        for(cNO=0;cNO<=3;cNO++)              // Scans all four keypad columns
        {
              if(READBIT(IOPIN1,COL0+cNO)==0)     // Checks whether the current column line is active
              break;                              // Stops scanning when the pressed column is identified
        }

        return cNO;                         // Returns the detected column number
}


// Scans the keypad and returns the character corresponding to the pressed key
u8 KeyScan(void)
{
        u32 rNO,cNO;                        // Variables used to store detected row and column numbers
        u8 keyV;                            // Variable used to store the key value

        //WAIT TO DETECT ANY KEY PRESS
        while(ColScan());                   // Waits until a key press is detected

        //identify row in which   key was presse
        rNO=ROWcheck();                     // Determines the row number of the pressed key

        //identify col in which key was pressed
        cNO=colcheck();                     // Determines the column number of the pressed key

        keyV=kpmLUT[rNO][cNO];              // Gets the corresponding key character from the lookup table

        return keyV;                        // Returns the detected key character
}


// Initializes the keypad GPIO configuration
void Init_KPM(void)
{
      //cfg row lines as gpio output
      WRITENIBBLE(IODIR1,ROW0,15);         // Configures the four keypad row lines as GPIO output pins

      //HENCE ALL ROWS ARE GROUNDED
      //DEFAULTS ALL COLS LINES ARE GPIO INPUT
}                                     


/*
u32 ReadNum(void)
{
u8 keyc
	u32 num=0;
	while(1)
	{
	keyc =keyscan();
	if(key>='0'&&keyc<='9')
	{
	num=(num*10)+(keyc-48);
	while(colscan()==0);
	}
	else
	{
	break;
	}
	}
	return num;
} 
*/


// Reads a multi-digit number from the keypad
u32 ReadNum(void)
{
        u8 keyC;                     // Stores the currently pressed keypad character

        u32 num=0;                   // Stores the numeric value entered through the keypad

        u32 count=0;                 // Keeps track of the number of digits entered

		    cmdLCD(0xc0);                 // Moves the LCD cursor to the beginning of the second line

        while(1)                     // Continuously reads keypad input until the '=' key is pressed
        {
				

                keyC=KeyScan();      // Scans the keypad and obtains the pressed key

                if(keyC>='0'&& keyC<='9') // Checks whether the pressed key is a numeric digit
                {

                        count++;     // Increments the number of digits entered

                        charLCD(keyC); // Displays the entered digit on the LCD
						            delay_ms(100);  // Provides a short delay after displaying the key

                       //   cmdLCD(0x06); // Previously used LCD command, currently commented out

                        num=(num*10)+(keyC-48); // Converts the entered digit to a number and appends it to the existing value

                        while(ColScan()==0); // Waits until the currently pressed key is released
                }

                else if(keyC=='*')   // Checks whether the '*' key is pressed, which is used as backspace
                {

                        if((count>0)&&(num!=0)) // Checks whether at least one digit has been entered and number is not zero
                        {

                        num=num/10;             // Removes the last entered digit from the number

                        cmdLCD(0XC0+count);     // Moves the cursor to the position of the last entered digit

                        cmdLCD(0X10);           // Moves the LCD cursor one position to the left

                        charLCD(' ');           // Clears the previously entered digit from the LCD

                        cmdLCD(0X10);           // Moves the cursor one position to the left again

                        count--;                // Decrements the number of entered digits

                        while(ColScan()==0);    // Waits until the '*' key is released
                        }

                        else if(num==0)         // Checks whether the current number is zero
                        {

                                while(ColScan()==0); // Waits until the '*' key is released
                        }
                }

                else if(keyC=='=')             // Checks whether the '=' key is pressed to finish number entry
                {

                        while(ColScan()==0);    // Waits until the '=' key is released

                        break;                  // Exits the input loop
                }
        }

        return num;                             // Returns the final numeric value entered through the keypad
}
