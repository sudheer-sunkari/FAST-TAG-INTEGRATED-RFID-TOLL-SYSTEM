#include"types.h"
void i2c_eeprom_bytewrite(u8 SlaveAddr,u8 wBuffAddr,u8 sByte);
u8 i2c_eeprom_randomread(u8 SlaveAddr,u8 rBuffAddr) ;
void i2c_eeprom_pagewrite(u8 SlaveAddr,u8 wBuffstartAddr,u8 *ptr8Bytes,u8 nBytes);
void i2c_eeprom_seqread(u8 SlaveAddr,u8 rBuffstartAddr,u8 *ptr8Bytes,u8 nBytes);
