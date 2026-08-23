#define LCD_DATA 6              // Starting bit position of the 8-bit LCD data lines

#define LCD_RS   18             // Defines the GPIO pin connected to the LCD RS (Register Select) pin
#define LCD_RW   17             // Defines the GPIO pin connected to the LCD RW (Read/Write) pin
#define LCD_EN   19             // Defines the GPIO pin connected to the LCD EN (Enable) pin

#define CLEAR_LCD 0X01          // LCD command to clear the entire display

#define RET_CUR_HOME 0X02       // LCD command to return the cursor to the home position

#define SHIFT_CUR_RIGHT 0X06    // LCD command to move the cursor position to the right after writing data

#define SHIFT_CUR_LEFT 0X07     // LCD command to move the cursor position to the left

#define DSP_OFF 0X08            // LCD command to turn OFF the display

#define DSP_ON_CUR_OFF 0X0C     // LCD command to turn ON the display and turn OFF the cursor

#define DSP_ON_CUR_ON 0X0E      // LCD command to turn ON the display and turn ON the cursor

#define DSP_ON_CUR_BLINK 0X0F   // LCD command to turn ON the display with a blinking cursor

#define SHIFT_DSP_LEFT 0X10     // LCD command to shift the complete display to the left

#define SHIFT_DSP_RIGHT 0X14    // LCD command to shift the complete display to the right

#define MODE_4BIT_1LINE 0X20    // LCD command to select 4-bit data mode with 1 display line

#define MODE_4BIT_2LINE 0X28    // LCD command to select 4-bit data mode with 2 display lines

#define MODE_8BIT_1LINE 0X30    // LCD command to select 8-bit data mode with 1 display line

#define MODE_8BIT_2LINE 0X38    // LCD command to select 8-bit data mode with 2 display lines

#define GOTO_LINE1_POS0 0X80    // LCD command to move the cursor to the beginning of line 1

#define GOTO_LINE2_POS0 0XC0    // LCD command to move the cursor to the beginning of line 2

#define GOTO_LINE3_POS0 0X94    // LCD command to move the cursor to the beginning of line 3

#define GOTO_LINE4_POS0 0XD4    // LCD command to move the cursor to the beginning of line 4

#define GOTO_CGRAM 0X40         // LCD command to set the address to the beginning of CGRAM for custom characters
