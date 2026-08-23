#include <LPC21xx.h>              // Includes LPC21xx microcontroller register definitions and hardware-specific declarations

#include "types.h"                // Includes user-defined data types such as u32 and f32
#include "define.h"               // Includes project-specific macro definitions
#include "adc_define.h"            // Includes ADC-related macro definitions such as CLKDIV, PDN_BIT, DONE_BIT, etc.
#include "delay.h"                 // Includes the delay_ms() function declaration
	  

// Function used to initialize the ADC peripheral and configure the required ADC pin
void Init_ADC(void)
{
  PINSEL1 &= ~(0xff<<22);         // Clears the pin function selection bits from bit 22 to bit 29 of PINSEL1
  PINSEL1|=0x01000000;             // Configures the required microcontroller pin for ADC functionality
  ADCR |=(CLKDIV<<CLKDIV_START_BIT)|(1<<PDN_BIT); // Sets the ADC clock divider and powers up the ADC module
}


// Function used to read the ADC value from the selected ADC channel
// chNo  : ADC channel number to be selected
// adcVal: Pointer used to store the converted digital ADC value
// eAR   : Pointer used to store the corresponding analog voltage
void Read_ADC(u32 chNo,u32 *adcVal,f32 *eAR)
{
	ADCR &= 0xffffff00;             // Clears the ADC channel selection bits while preserving the remaining ADC control settings
	ADCR |= (1<<chNo)|(1<<ADC_START_BIT); // Selects the requested ADC channel and starts the ADC conversion
	delay_ms(3);                    // Provides a short delay to allow the ADC conversion to take place
	while(((ADDR>>DONE_BIT)&1)==0); // Continuously checks the DONE bit until the ADC conversion is completed
	ADCR &= ~(1<<ADC_START_BIT);    // Clears the ADC START bit to stop the current ADC conversion command
	*adcVal = ((ADDR>>RESULT_START_BIT)&1023); // Extracts the 10-bit ADC conversion result from ADDR and stores it in adcVal
	*eAR = ((*adcVal)*(3.3/1023));  // Converts the 10-bit ADC value into the corresponding voltage assuming a 3.3V reference
}
