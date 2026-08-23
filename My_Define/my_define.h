#include"lcd.h"
#include"define.h"
#include"delay.h"
#include"types.h"
#include"i2c.h"
#include"kpm.h"
#include"lcd_define.h"
#include"i2c_define.h"
#include"i2c_eeprom.h"
#include"adc_define.h"
#include"kpm_define.h"
#include"dummy_fun.h"

 #define MAX_VEHICLES 3
 #define MOTOR_PIN1 1<<20
#define MOTOR_PIN2  1<<21
 #define ID_ADDR 0X0000
#define VECH_ADDR 0X1000
#define BAL_ADDR 0X4000
#define SLAVE_ADDR 0x50
#define SENSOR_THRESHOLD 15
#define EINT0_CHNO  15
#define EINT1_CHNO	16
