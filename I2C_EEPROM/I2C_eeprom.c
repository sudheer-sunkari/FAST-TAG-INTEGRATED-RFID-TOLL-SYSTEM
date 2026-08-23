#include"types.h"                    // Includes user-defined data types such as u8 and u16
#include"i2c_define.h"              // Includes I2C-related macro definitions
#include"i2c.h"                     // Includes I2C function declarations
#include"delay.h"                   // Includes delay function declarations


// Function used to write a single byte of data into the I2C EEPROM
void i2c_eeprom_bytewrite(u8 SlaveAddr,u16 wBuffAddr,u8 sByte)
{
	i2c_start();                      // Generates an I2C START condition
	i2c_write(SlaveAddr<<1);          // Sends the EEPROM slave address with the write bit
	i2c_write(wBuffAddr>>8);          // Sends the higher 8 bits of the EEPROM memory address
	i2c_write(wBuffAddr);              // Sends the lower 8 bits of the EEPROM memory address
	i2c_write(sByte);                  // Sends the data byte to be stored in the EEPROM
	i2c_stop();                        // Generates an I2C STOP condition
	delay_ms(10);                      // Waits for the EEPROM internal write cycle to complete
}


// Function used to read one byte from a specific EEPROM memory address
u8 i2c_eeprom_randomread(u8 SlaveAddr,u16 rBuffAddr) 
{
	u8 rByte;                          // Variable used to store the byte read from EEPROM

	i2c_start();                       // Generates an I2C START condition
	i2c_write(SlaveAddr<<1);           // Sends the EEPROM slave address with the write bit
	i2c_write(rBuffAddr>>8);            // Sends the higher 8 bits of the EEPROM memory address
	i2c_write(rBuffAddr);               // Sends the lower 8 bits of the EEPROM memory address

	i2c_restart();                     // Generates a repeated START condition before switching to read mode

	//i2c_write(rBuffAddr);             // Sends the EEPROM address if required before another repeated START
	//i2c_restart();                    // Generates another repeated START condition if required

	i2c_write((SlaveAddr<<1)|1);       // Sends the EEPROM slave address with the read bit set
	rByte=i2c_nack();                  // Receives one byte from EEPROM and sends NACK after the last byte
	i2c_stop();                        // Generates an I2C STOP condition

	return rByte;                      // Returns the byte read from EEPROM
}


// Function used to write multiple bytes sequentially into EEPROM
void i2c_eeprom_pagewrite(u8 SlaveAddr,u16 wBuffstartAddr,u8* ptr8Bytes, int nBytes)
{
	u32 i;                             // Loop counter used to transmit multiple bytes

	i2c_start();                       // Generates an I2C START condition
	i2c_write(SlaveAddr<<1);           // Sends the EEPROM slave address with the write bit
	i2c_write(wBuffstartAddr>>8);      // Sends the higher 8 bits of the EEPROM memory address
	i2c_write(wBuffstartAddr);         // Sends the lower 8 bits of the EEPROM memory address

	for(i=0;i<nBytes;i++)              // Loops through all bytes that need to be written
	{
		//send one byte at5 atime,get ack
		i2c_write(ptr8Bytes[i]);        // Sends one byte of data from the buffer to the EEPROM
	}

	i2c_stop();                        // Generates an I2C STOP condition after all bytes are transmitted
	delay_ms(10);                      // Waits for the EEPROM internal write cycle to complete
}


// Function used to read multiple bytes sequentially from EEPROM
void i2c_eeprom_seqread(u8 SlaveAddr,u16 rBuffstartAddr,u8 *ptr8Bytes,int nBytes)
{
	u32 i;                             // Loop counter used to receive multiple bytes

	i2c_start();                       // Generates an I2C START condition
	i2c_write(SlaveAddr<<1);           // Sends the EEPROM slave address with the write bit

	i2c_write(rBuffstartAddr>>8);      // Sends the higher 8 bits of the EEPROM memory address
	i2c_write(rBuffstartAddr);         // Sends the lower 8 bits of the EEPROM memory address

	i2c_restart();                     // Generates a repeated START condition before switching to read mode

	i2c_write((SlaveAddr<<1)|1);       // Sends the EEPROM slave address with the read bit

	for(i=0;i<(nBytes-1);i++)          // Reads all bytes except the last byte and sends ACK after each byte
	{
		//read byte & give mack
		ptr8Bytes[i]=i2c_mack();        // Receives one byte and sends master acknowledge to continue reading
	}

	//read last byte &give nack
	ptr8Bytes[i]=i2c_nack();           // Receives the final byte and sends NACK to indicate the end of reception

	i2c_stop();                        // Generates an I2C STOP condition

	ptr8Bytes[i+1]='\0';               // Adds a null terminator after the received data to treat it as a string
}
