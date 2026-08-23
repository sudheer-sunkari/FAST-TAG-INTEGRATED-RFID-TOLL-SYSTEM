#include"types.h"                    // Includes user-defined data types such as u32 and f32

#define FOSC 12000000                 // Defines the crystal oscillator frequency as 12 MHz
#define CCLK (FOSC*5)                 // Defines the CPU clock frequency as FOSC multiplied by 5
#define PCLK (CCLK/4)                 // Defines the peripheral clock frequency as CCLK divided by 4
#define ADCCLK 3000000                // Defines the required ADC clock frequency as 3 MHz
#define CLKDIV ((PCLK/ADCCLK)-1)      // Calculates the ADC clock divider value from PCLK and ADCCLK

//defines for ADCR sfr
#define CLKDIV_START_BIT 8            // Defines bit 8 as the starting bit position of the ADC clock divider field
#define PDN_BIT          21           // Defines bit 21 of ADCR as the power-down/power-up control bit
#define ADC_START_BIT    24           // Defines bit 24 of ADCR as the ADC conversion start control bit

//define for ADDR sfr
#define RESULT_START_BIT 6            // Defines bit 6 as the starting bit position of the 10-bit ADC result
#define DONE_BIT         31           // Defines bit 31 of ADDR as the ADC conversion completion status bit

void Init_ADC(void);                  // Function prototype for initializing the ADC peripheral
void Read_ADC(u32 chNo,u32 *adcVal,f32 *eAR); // Function prototype for reading ADC value and converting it to analog voltage
