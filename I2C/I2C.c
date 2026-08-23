#include<lpc21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include"types.h"                   // Includes user-defined data types such as u8
#include"i2c.h"                     // Includes I2C function declarations
#include"i2c_define.h"              // Includes I2C-related macro definitions


// Function used to initialize the I2C peripheral
void i2c_init(void)
{
    //cfg req gpio pins as scl&sda pins
    PINSEL0|=0X00000050;            // Configures the required GPIO pins as I2C SCL and SDA pins

    //ENABLE I2C PERIPHERALS
    I2CONSET=1<<I2EN_BIT;            // Sets the I2C enable bit to enable the I2C peripheral

    //CFG I2C SPEED/FREQ
    I2SCLH=I2SCLL=LOADVAL;           // Loads the same value into I2SCLH and I2SCLL to configure the I2C clock frequency
}


// Function used to generate an I2C START condition
void i2c_start(void)
{
    //cfg to issue start bit /cond/event
    I2CONSET=1<<STA_BIT;              // Sets the START bit to request an I2C START condition

    //WAIT FOR START EVENT COMPLETION/STATUS
    while(((I2CONSET>>SI_BIT)&1)==0); // Waits until the I2C interrupt/status flag indicates that the START condition is completed

    //clear start event /bit
    I2CONCLR=1<<STAC_BIT;              // Clears the START condition bit
}


// Function used to generate an I2C STOP condition
void i2c_stop(void)
{
    //cfg to isuue stop bit /cond/event
    I2CONSET=1<<STO_BIT;              // Sets the STOP bit to generate an I2C STOP condition

    I2CONCLR=1<<SIC_BIT;              // Clears the I2C interrupt/status flag to complete the STOP operation
}


// Function used to generate an I2C repeated START condition
void i2c_restart(void)
{
    //cfg to issue restart bit/cond/event
    I2CONSET=1<<STA_BIT;              // Sets the START bit to request a repeated START condition

    //TRIGGER RESTART EVENT
    I2CONCLR=1<<SIC_BIT;              // Clears the SI flag to trigger the repeated START operation

    //WAIT FOR RESTART EVENT COMPILATION /STATAS
    while(((I2CONSET>>SI_BIT)&1)==0); // Waits until the repeated START condition is completed

    //CLEAR START EVENT/BIT
    I2CONCLR=1<<STAC_BIT;              // Clears the START condition bit
}


// Function used to transmit one byte through the I2C bus
void i2c_write(u8 sByte)
{
    //place byte to be transmitted in tx/rx buffer
    I2DAT=sByte;                       // Loads the byte to be transmitted into the I2C data register

    //trigger transmission of byte
    I2CONCLR=1<<SIC_BIT;               // Clears the SI flag to start the byte transmission

    //WAIT FOR BYTE TRANSMISSION COMPLETIOM/STATUS
    while(((I2CONSET>>SI_BIT)&1)==0);  // Waits until the byte transmission is completed
}


// Function used to receive one byte from the I2C bus and send a NACK
u8 i2c_nack(void)
{
    //trigger reception of byte
    I2CONCLR=1<<SIC_BIT;               // Clears the SI flag to allow reception of the next byte

    //WAIT UNTIL BYTE RECEIVED COMPLETION|STATUS
    while(((I2CONSET>>SIC_BIT)&1)==0); // Waits until the byte reception is completed

    //AUTOMATICALLY ISSUES NACK
    //READ RECEVED BYTE
    return I2DAT;                      // Reads and returns the received byte from the I2C data register
}


// Function used to receive one byte from the I2C bus and send a master acknowledge
u8 i2c_mack(void)
{
    I2CONSET=1<<AA_BIT;                // Sets the acknowledge bit so the master sends an ACK after receiving the byte

    //TRIGGER RECEPTIOMof byte
    I2CONCLR=1<<SIC_BIT;               // Clears the SI flag to allow reception of the next byte

    //WAIT UNTILL BYTE received completion|stutas
    while(((I2CONSET>>SI_BIT)&1)==0);  // Waits until the byte reception is completed

    //AUTOMATICALLY ISSUES MACK
    //CLEAR aa_bit
    I2CONCLR=1<<AA_BIT;                // Clears the acknowledge bit after receiving the byte

    //read recvd byte
    return I2DAT;                      // Reads and returns the received byte from the I2C data register
}
