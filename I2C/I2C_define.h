#define	AA_BIT 2                    // Defines bit 2 of I2CONSET as the Assert Acknowledge (AA) bit

#define	SI_BIT 3                    // Defines bit 3 of I2CONSET as the I2C Interrupt/Status (SI) bit

#define	STO_BIT 4                   // Defines bit 4 of I2CONSET as the STOP condition control bit

#define STA_BIT 5                   // Defines bit 5 of I2CONSET as the START condition control bit

#define	I2EN_BIT 6                  // Defines bit 6 of I2CONSET as the I2C peripheral enable bit


//DEFINE FOR i2concclr sfr

#define	AAC_BIT 2                   // Defines bit 2 of I2CONCLR for clearing the Assert Acknowledge bit

#define	SIC_BIT 3                   // Defines bit 3 of I2CONCLR for clearing the I2C Interrupt/Status flag

#define	STAC_BIT 5                  // Defines bit 5 of I2CONCLR for clearing the START condition bit

#define	I2ENC_BIT 6                 // Defines bit 6 of I2CONCLR for disabling the I2C peripheral


#define	FOSC 12000000               // Defines the oscillator frequency as 12 MHz

#define	CCLK (FOSC*5)               // Defines the CPU clock frequency as five times the oscillator frequency

#define	PCLK  (CCLK/4)              // Defines the peripheral clock frequency as one-fourth of the CPU clock frequency

#define	 I2C_FREQ 100000            // Defines the required I2C clock frequency as 100 kHz

#define	 LOADVAL ((PCLK/I2C_FREQ)/2) // Calculates the value loaded into I2SCLH and I2SCLL to generate the required I2C clock frequency
