#include<lpc21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include"my_define.h"               // Includes project-specific macro definitions

// Stores the predefined RFID card numbers for the three vehicles
char tag_no[3][10]={{"12638593"},{"12527137"},{"12547508"}}; 

// Stores the vehicle numbers corresponding to the three RFID cards
char vehicle_number[3][10]={{"3244"},{"5776"},{"3456"}}; 

// Stores the initial balance values corresponding to the three RFID cards
char bal[3][4]={"100","200","300"}; 

// Buffer used to store the RFID card number received from the RFID reader
char tag_buff[10];

// Buffer used to store the vehicle number and current balance read from EEPROM
char vech_buff[5],bal_buff[4];

// Stores the balance amount as an integer for arithmetic operations
int amount;

// External interrupt flags set by the EINT0 and EINT1 interrupt routines
extern volatile unsigned char e0_flag,e1_flag;


int main()
{
     // Configures the motor control pins as output pins
     IODIR0 |= MOTOR_PIN1 | MOTOR_PIN2;  

     // Initially clears both motor control pins to keep the motor stopped
     IOCLR0 = MOTOR_PIN1 | MOTOR_PIN2;

     // Initializes the peripherals and project modules
     init_fun();

     // Stores the predefined RFID IDs, vehicle numbers and initial balances into EEPROM
     save_cards();

     // Clears the LCD display
     cmdLCD(0X01);

     // Continuously executes the main application
     while(1)
     {
        // Checks the distance sensor and performs the main toll-gate operation
        process_sensor();

        // Checks whether the recharge interrupt event has occurred
        if(e0_flag)
        {
            // Clears the recharge interrupt flag before processing the operation
            e0_flag = 0;

            // Performs the RFID card recharge operation
            recharge_card();
        }

        // Checks whether the manual deduction interrupt event has occurred
        if(e1_flag)
        {
            // Clears the manual deduction interrupt flag before processing the operation
            e1_flag = 0;

            // Performs the manual toll deduction operation
            manual_deduct();
        }
     }	
}
