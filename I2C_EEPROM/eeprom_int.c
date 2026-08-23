#include<stdlib.h>
#include "my_define.h"

extern char tag_no[3][10];
extern char vehicle_number[3][10];
extern char bal[3][4];
extern char tag_buff[10];
extern char vech_buff[5],bal_buff[4];
extern int i,amount;
	
void save_cards()
{
	for(i=0;i<3;i++)
	{
		i2c_eeprom_pagewrite(SLAVE_ADDR,(ID_ADDR+i*10),(u8*)tag_no[i],10);		
	}

	for(i=0;i<3;i++)
	{	
		i2c_eeprom_pagewrite(SLAVE_ADDR,(VECH_ADDR+i*5),(u8*)vehicle_number[i],5);		
	}

	for(i=0;i<3;i++)
	{
		i2c_eeprom_pagewrite(SLAVE_ADDR,(BAL_ADDR+i*4),(u8*)bal[i],4);
	}
}
