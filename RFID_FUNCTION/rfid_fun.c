#include<lpc21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include<stdlib.h>                  // Provides functions such as atoi() for converting strings to integers
#include<string.h>                  // Provides string functions such as strcmp() and memset()
#include"my_define.h"               // Includes project-specific macro definitions

extern char tag_no[3][10];          // Declares the RFID card number array defined in another source file
extern char vehicle_number[3][10];  // Declares the vehicle number array defined in another source file
extern char tag_buff[10];           // Declares the RFID tag buffer defined in another source file
extern unsigned char r_flag;        // Declares the RFID reception status flag defined in another source file
extern char buff[20];               // Declares the buffer used to store received RFID data
extern char tag_buff[10];           // Declares the RFID tag buffer
extern char vech_buff[5],bal_buff[4]; // Declares buffers used for vehicle number and balance data
extern int i,amount;                // Declares the global card index and balance variables

char ch_no[10];                     // Stores the card number entered through the keypad
volatile unsigned char e0_flag=0,e1_flag=0; // Flags used to indicate EINT0 and EINT1 interrupt events
unsigned int flag=0,found=0,keyV;   // flag indicates card validity, found indicates card search result, keyV stores keypad input
int j,k;                            // Temporary variables used for digit conversion and calculations
u32 adcVal;                         // Stores the digital ADC conversion result
float sensor_volt = 0;              // Stores the voltage calculated from the ADC result


// Initializes all the peripherals required by the project
void init_fun()
{
	 initLCD();                      // Initializes the LCD
	 InitUART0();                    // Initializes UART0 for RFID reader communication
	 enable_eint();                  // Enables and configures external interrupts
	 i2c_init();                     // Initializes the I2C peripheral for EEPROM communication
	 Init_ADC();                     // Initializes the ADC peripheral for distance sensor
	 Init_KPM();                     // Initializes the keypad peripheral
} 


// Detects the vehicle using the distance sensor and waits for RFID card input
void process_sensor()
{
	cmdLCD(0x01);                    // Clears the LCD display
	cmdLCD(0x80);                    // Moves the LCD cursor to the first line, first position
	strLCD("FASTAG RFID SYSTEM");    // Displays the project name on the LCD
	delay_ms(3000);                  // Waits for 3 seconds

	while(1)
	{
		Read_ADC(1,&adcVal,&sensor_volt); // Reads ADC channel 1 and stores ADC value and sensor voltage

		if(e0_flag || e1_flag)       // Checks whether recharge or manual deduction interrupt occurred
		{
			return;                  // Exits the function if an external interrupt event occurred
	 	}

		if((adcVal >= 750) && (adcVal <= 920)) // Checks whether the ADC value is within the vehicle detection range
		{
		    break;                   // Exits the loop when a vehicle is detected
		}

		delay_ms(50);                 // Waits for 50 ms before checking the sensor again
  	}

	  cmdLCD(0x01);                    // Clears the LCD display
	  cmdLCD(0x80);                    // Positions the cursor at the beginning of the first line
	  delay_ms(1000);                  // Waits for 1 second
	  strLCD("Vechile detected");      // Displays vehicle detection message
	  delay_ms(800);                   // Waits for 800 ms

	  cmdLCD(0x01);                   // Clears the LCD display
	  strLCD("WELCOME TOLL PLAZA");   // Displays the toll plaza welcome message
	  cmdLCD(0xc0);                   // Moves the cursor to the beginning of the second LCD line
	  delay_ms(1000);                 // Waits for 1 second
	  strLCD("WAITING FOR CARD");     // Displays the message asking the user to present the RFID card

  	r_flag=0;                       // Clears the RFID reception flag before receiving a new card
	  i=0;                            // Resets the RFID buffer index
	  memset(buff,'\0',20);           // Clears the RFID receive buffer

	  while(r_flag!=2)                // Waits until the complete RFID card number is received
	  {
	     if(e0_flag || e1_flag)       // Checks for recharge or manual deduction interrupt
	     {
	         return;                  // Exits if an external interrupt occurs
	     }
	  }

	  cmdLCD(CLEAR_LCD);              // Clears the LCD display
	  cmdLCD(GOTO_LINE1_POS0);        // Moves cursor to the first LCD line
	  strLCD("WELCOME TOLL PLAZA");   // Displays the toll plaza message
	  cmdLCD(GOTO_LINE2_POS0);        // Moves cursor to the second LCD line
	  strLCD("Checking..");           // Displays the card checking message
	  delay_ms(500);                  // Waits for 500 ms
	  strLCD((char *)buff);           // Displays the received RFID card number
	  delay_ms(1000);                 // Waits for 1 second

	  card_campare();                 // Compares the received RFID card number with predefined card numbers
}


// Compares the received RFID card number with the stored card numbers
// and deducts the toll amount from the corresponding EEPROM balance
void card_campare()
{
 	for(i=0;i<MAX_VEHICLES;i++)    // Loops through all registered vehicles
 	{	     
		delay_ms(100);                // Provides a short delay before comparison

		if(strcmp(tag_no[i],(char*)buff)==0) // Compares the received RFID number with the stored RFID number
		{
			flag=1;                   // Sets the flag to indicate that a valid card was found

			cmdLCD(GOTO_LINE3_POS0);  // Moves the LCD cursor to the third line
			strLCD("valid card");      // Displays valid card message

			i2c_eeprom_seqread(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Reads the current balance of the card from EEPROM
			amount=atoi(bal_buff);    // Converts the balance stored as a string into an integer

			cmdLCD(GOTO_LINE4_POS0);  // Moves the cursor to the fourth LCD line
			u32LCD(amount);           // Displays the current balance
			delay_ms(1000);           // Waits for 1 second
			cmdLCD(0x01);             // Clears the LCD display

			if(amount>=40)             // Checks whether the balance is sufficient for the ?40 toll
			{
				amount=amount-40;      // Deducts ?40 from the available balance

				k=amount;              // Copies the updated balance to a temporary variable
				j=0;                   // Initializes the digit counter

				while(k)               // Counts the number of digits in the updated balance
				{
					j++;               // Increments the digit counter
					k/=10;             // Removes the last digit
				}

				k=amount;              // Restores the updated balance value
				bal_buff[j--]='\0';    // Adds the null character at the end of the balance string

				while(k)               // Converts the integer balance into ASCII characters
				{
					bal_buff[j--]=(k%10)+48; // Converts each digit into its ASCII representation
					k/=10;             // Removes the processed digit
				}

				i2c_eeprom_pagewrite(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Writes the updated balance back to EEPROM

				cmdLCD(GOTO_LINE4_POS0); // Moves cursor to the fourth LCD line
				strLCD("MONEY DEDUCTED"); // Displays money deducted message
			    display(i,amount);        // Displays card details and opens the gate
			}	
			else
			{
				cmdLCD(GOTO_LINE4_POS0); // Moves cursor to the fourth LCD line
				strLCD("INSUFFICIENT BAL"); // Displays insufficient balance message
			}	 
		}
    }

	if(flag==0)                       // Checks whether no valid card was found
	{
		cmdLCD(0x01);                 // Clears the LCD display
		cmdLCD(GOTO_LINE3_POS0);      // Moves cursor to the third LCD line
		strLCD("invalid card");       // Displays invalid card message
		delay_ms(1000);               // Waits for 1 second
	}
}


// Opens the toll gate by rotating the DC motor in the opening direction
void open_gate()
{
	IOSET0 = MOTOR_PIN1;             // Sets motor control pin 1
	IOCLR0 = MOTOR_PIN2;             // Clears motor control pin 2 to rotate the motor in the opening direction

	cmdLCD(0X01);                    // Clears the LCD display
	cmdLCD(DSP_ON_CUR_ON);           // Configures the LCD display and cursor
	strLCD("Gate opening.....");      // Displays gate opening message
	delay_ms(2000);                  // Keeps the motor running for 2 seconds

	IOCLR0=MOTOR_PIN1;               // Stops motor control pin 1
	IOCLR0=MOTOR_PIN2;               // Stops motor control pin 2
}


// Closes the toll gate after detecting that the vehicle has moved away
void close_gate()
{
  while(1)
  {
    Read_ADC(1,&adcVal,&sensor_volt); // Reads the distance sensor ADC value

    if((adcVal >= 150))              // Checks whether the vehicle is still within the sensor detection range
    {														           
      delay_ms(100);                 // Waits for 100 ms before checking again
    }
    else
    {
      break;                         // Exits the loop when the vehicle moves away from the sensor
    }
  }

	IOCLR0=MOTOR_PIN1;                // Clears motor control pin 1
	IOSET0= MOTOR_PIN2;               // Sets motor control pin 2 to rotate the motor in the closing direction

	cmdLCD(0X01);                     // Clears the LCD display
	cmdLCD(DSP_ON_CUR_ON);            // Configures the LCD display and cursor
	strLCD("Gate closing .....");      // Displays gate closing message
	delay_ms(2000);                   // Keeps the motor running for 2 seconds

	IOCLR0=MOTOR_PIN1;                // Stops motor control pin 1
	IOCLR0=MOTOR_PIN2;                // Stops motor control pin 2
}


// Performs recharge operation for a registered RFID card
void recharge_card()
{
	int j=7;                           // Initializes the index used for converting the entered card number into a string

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(GOTO_LINE1_POS0);           // Moves cursor to the first LCD line
	strLCD("recharge card");            // Displays recharge card message
	delay_ms(2000);                    // Waits for 2 seconds

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(GOTO_LINE1_POS0);           // Moves cursor to the first LCD line
	strLCD("enter card no");            // Asks the user to enter the RFID card number
	delay_ms(1000);                    // Waits for 1 second

	keyV=ReadNum();                    // Reads the card number entered through the keypad

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(GOTO_LINE3_POS0);           // Moves cursor to the third LCD line
	u32LCD(keyV);                      // Displays the entered card number
	delay_ms(1000);                    // Waits for 1 second

	for(j=7;j>=0;j--)                  // Converts the numeric card number into an 8-digit character string
	{
		ch_no[j]=(keyV%10)+48;          // Converts the current digit into its ASCII character
		keyV/=10;                       // Removes the processed digit
	}

	ch_no[8]='\0';                     // Adds null terminator to the card number string

	cmdLCD(GOTO_LINE4_POS0);           // Moves cursor to the fourth LCD line
	strLCD(ch_no);                     // Displays the entered card number
	delay_ms(2000);                    // Waits for 2 seconds

	flag=0;                            // Clears the card validity flag

	for(i=0;i<3;i++)                   // Searches through all registered RFID cards
	{
		if(strcmp(ch_no,tag_no[i])==0)  // Compares entered card number with stored card number
		{
			flag=1;                     // Sets flag when a valid card is found

			cmdLCD(0x01);               // Clears the LCD display
			cmdLCD(GOTO_LINE4_POS0);    // Moves cursor to the fourth LCD line
			strLCD("valid");            // Displays valid card message
			delay_ms(1000);             // Waits for 1 second

			cmdLCD(CLEAR_LCD);          // Clears the LCD display

			// Read current balance from EEPROM
			i2c_eeprom_seqread(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Reads the current balance of the selected card from EEPROM

			amount=atoi(bal_buff);      // Converts the EEPROM balance string into an integer

			// Ask user for recharge amount
			cmdLCD(0x80);               // Moves cursor to the first LCD position
			strLCD("enter amount");     // Prompts the user to enter recharge amount
			keyV=ReadNum();             // Reads recharge amount from keypad

			// Add recharge amount to existing balance
			amount+=keyV;               // Adds recharge amount to current EEPROM balance

			// Convert updated balance into ASCII format
			k=amount;                   // Copies updated balance to temporary variable
			j=0;                        // Resets digit counter

			while(k)                    // Counts the number of digits in the updated balance
			{
				j++;                    // Increments digit counter
				k/=10;                  // Removes the last digit
			}

			k=amount;                   // Restores updated balance value

			bal_buff[j--]='\0';         // Adds null terminator at the end of the balance string

			while(k)                    // Converts each digit of the balance into ASCII
			{
				bal_buff[j--]=(k%10)+48; // Converts the current digit into its ASCII representation
				k/=10;                   // Removes the processed digit
			}

			// Write updated balance into EEPROM
			i2c_eeprom_pagewrite(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Stores the updated balance permanently in EEPROM

			cmdLCD(GOTO_LINE4_POS0);    // Moves cursor to the fourth LCD line
			strLCD("MONEY ADDED");      // Displays money added message
			delay_ms(2000);             // Waits for 2 seconds

			display1(i,amount);          // Displays the updated card information and balance

			// Card found, so no need to check remaining cards
			break;                      // Exits the card search loop
		}
	} 

	if(flag==0)                       // Checks whether the entered card was not found
	{
		cmdLCD(GOTO_LINE4_POS0);       // Moves cursor to the fourth LCD line
		strLCD("INVALID CARD");        // Displays invalid card message
		delay_ms(2000);                // Waits for 2 seconds
	}
}


// Performs manual balance deduction for a registered RFID card
void manual_deduct()
{
	int j=7;                           // Initializes the index for card number conversion

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(GOTO_LINE1_POS0);           // Moves cursor to the first LCD line
	strLCD("MANUAL DEDUCTION");        // Displays manual deduction message
	delay_ms(1000);                    // Waits for 1 second

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(0x80);                      // Moves cursor to the first LCD position
	strLCD("enter card no");           // Prompts the user to enter the card number
	keyV=ReadNum();                    // Reads the card number from the keypad

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(GOTO_LINE2_POS0);           // Moves cursor to the second LCD line
	u32LCD(keyV);                      // Displays the entered card number
	delay_ms(1000);                    // Waits for 1 second

	for(j=7;j>=0;j--)                  // Converts the entered card number into an 8-digit character string
	{
		ch_no[j]=(keyV%10)+48;          // Converts the current digit into ASCII
		keyV/=10;                       // Removes the processed digit
	}

	ch_no[8]='\0';                     // Adds null terminator to the card number string

	cmdLCD(GOTO_LINE3_POS0);           // Moves cursor to the third LCD line
	strLCD(ch_no);                     // Displays the entered card number

	flag=0;                             // Clears the card validity flag

	for(i=0;i<3;i++)                   // Searches through all registered cards
	{
		if(strcmp(ch_no,tag_no[i])==0)  // Compares entered card number with stored card number
		{
			 flag=1;                  // Indicates that a valid card was found
			 found=1;                 // Indicates that the card search was successful

			 i2c_eeprom_seqread(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Reads current balance from EEPROM
			 amount=atoi(bal_buff);   // Converts EEPROM balance string into integer
			 break;                   // Stops searching after finding the card
		 }
	}

	if(found == 0)                     // Checks whether no matching card was found
	{
	   cmdLCD(GOTO_LINE4_POS0);        // Moves cursor to the fourth LCD line
	   strLCD("INVALID CARD");         // Displays invalid card message
	   delay_ms(2000);                 // Waits for 2 seconds
	   return;                         // Exits the function
	}

	cmdLCD(GOTO_LINE4_POS0);           // Moves cursor to the fourth LCD line
	strLCD("VALID CARD");              // Displays valid card message
	delay_ms(2000);                    // Waits for 2 seconds

	cmdLCD(0x01);                      // Clears the LCD display
	cmdLCD(0x80);                      // Moves cursor to the first LCD position
	strLCD("enter amount ");            // Prompts the user to enter deduction amount
	keyV=ReadNum();                    // Reads deduction amount from keypad

	if(amount>=keyV)                   // Checks whether sufficient balance is available
	{
		amount-=keyV;                  // Deducts the entered amount from the current balance

		k=amount;                      // Copies updated balance into temporary variable
		j=0;                           // Resets digit counter

		while(k)                       // Counts the number of digits in the updated balance
		{
			j++;                       // Increments digit counter
			k/=10;                     // Removes the last digit
		}

		k=amount;                      // Restores updated balance value
		bal_buff[j--]='\0';            // Adds null terminator to the balance string

		while(k)                       // Converts the updated balance into ASCII characters
		{
			bal_buff[j--]=(k%10)+48;   // Converts the current digit into ASCII
			k/=10;                     // Removes the processed digit
		}

		i2c_eeprom_pagewrite(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal_buff,4); // Writes the updated balance back to EEPROM

		cmdLCD(GOTO_LINE3_POS0);       // Moves cursor to the third LCD line
		strLCD("MONEY DEDUCTED");      // Displays money deducted message
		delay_ms(2000);                // Waits for 2 seconds

		display(i,amount);             // Displays card information and starts the gate operation
	}
	else
	{
		cmdLCD(GOTO_LINE4_POS0);       // Moves cursor to the fourth LCD line
		strLCD("INSUFFICIENT BAL");    // Displays insufficient balance message
		delay_ms(2000);                // Waits for 2 seconds
		return;                        // Exits the function
	}
}


// Configures and enables external interrupts EINT0 and EINT1
void enable_eint(void)
{
	PINSEL0|=0x20000000|0x80000000;   // Configures the required pins for EINT0 and EINT1 functions
	SETBIT(VICIntEnable ,EINT0_CHNO); // Enables EINT0 interrupt in the VIC
	SETBIT(VICIntEnable ,EINT1_CHNO); // Enables EINT1 interrupt in the VIC

	VICVectCntl1 =(1<<5)|EINT0_CHNO;  // Enables vector slot 1 and assigns EINT0 interrupt source
	VICVectAddr1=(unsigned int)eint0_isr; // Assigns the EINT0 ISR address to vector slot 1

	VICVectCntl2 =(1<<5)|EINT1_CHNO;  // Enables vector slot 2 and assigns EINT1 interrupt source
	VICVectAddr2=(unsigned int)eint1_isr; // Assigns the EINT1 ISR address to vector slot 2

	EXTMODE = (1<<1)|(1<<2);          // Configures EINT0 and EINT1 as edge-triggered interrupts
}


// EINT0 interrupt service routine used for recharge operation
void eint0_isr(void)__irq	
{
	e0_flag=1;                         // Sets the EINT0 flag to request recharge processing in the main loop
	EXTINT = 1<<1;                    // Clears the EINT0 interrupt flag
	VICVectAddr=0;                    // Signals the end of the VIC interrupt service routine
}


// EINT1 interrupt service routine used for manual deduction
void eint1_isr(void) __irq	
{
	   
	e1_flag=1;                         // Sets the EINT1 flag to request manual deduction processing in the main loop
	EXTINT = 1<<2;                    // Clears the EINT1 interrupt flag
	VICVectAddr=0;                    // Signals the end of the VIC interrupt service routine
}


// Displays RFID card number, vehicle number and current balance, then opens and closes the gate
void display(int j,int amount)
{
	cmdLCD(0x01);                     // Clears the LCD display
	cmdLCD(GOTO_LINE1_POS0);          // Moves cursor to the first LCD line
	strLCD(tag_no[j]);                // Displays the RFID card number
	cmdLCD(GOTO_LINE2_POS0);          // Moves cursor to the second LCD line
	strLCD(vehicle_number[j]);        // Displays the vehicle number
	cmdLCD(GOTO_LINE3_POS0);          // Moves cursor to the third LCD line
	u32LCD(amount);                   // Displays the current balance
	cmdLCD(GOTO_LINE4_POS0);          // Moves cursor to the fourth LCD line
	strLCD("THANK YOU ");             // Displays thank-you message
	delay_ms(2000);                   // Waits for 2 seconds

	open_gate();                      // Opens the toll gate
	cmdLCD(0x01);                     // Clears the LCD display
	close_gate();                     // Waits for vehicle departure and closes the gate
} 


// Displays RFID card number, vehicle number and current balance after recharge
void display1(int j,int amount)
{
	cmdLCD(0x01);                     // Clears the LCD display
	cmdLCD(GOTO_LINE1_POS0);          // Moves cursor to the first LCD line
	strLCD(tag_no[j]);                // Displays the RFID card number
	cmdLCD(GOTO_LINE2_POS0);          // Moves cursor to the second LCD line
	strLCD(vehicle_number[j]);        // Displays the vehicle number
	cmdLCD(GOTO_LINE3_POS0);          // Moves cursor to the third LCD line
	u32LCD(amount);                   // Displays the updated balance
	cmdLCD(GOTO_LINE4_POS0);          // Moves cursor to the fourth LCD line
	strLCD("THANK YOU ");             // Displays thank-you message
	delay_ms(2000);                   // Waits for 2 seconds
}
