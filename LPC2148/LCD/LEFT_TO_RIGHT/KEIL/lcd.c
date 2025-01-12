/********************************************************/
/* Working of LCD
   The string "EMBEDDED" will display and move from left to right
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148


LCD Mapping 	: P0.16-P0.23: D0-D7
                RS: P1.16, RW: P1.17, E: P1.18

********************************************************/


#include <lpc214x.h> 
#include "delay.h"



// Function to send a command to the LCD
void LCD_command(unsigned char command)
{
    IOCLR0 = 0xFF << 16;     // Clear LCD Data lines
    IOCLR1 = 1 << 16;        // RS=0 for command
    IOCLR1 = 1 << 17;        // RW=0 for write
    IOSET0 = command << 16;  // Put command on data line
    IOSET1 = (1 << 18);      // EN=1 
    delay_ms(10);            // Delay
    IOCLR1 = (1 << 18);      // EN=0
}

// Function to send data to the LCD
void LCD_data(unsigned char data)
{
    IOCLR0 = 0xFF << 16;     // Clear LCD Data lines
    IOSET1 = 1 << 16;        // RS=1 for data
    IOCLR1 = 1 << 17;        // RW=0 for write
    IOSET0 = data << 16;     // Put data on data line
    IOSET1 = (1 << 18);      // EN=1 
    delay_ms(10);            // Delay
    IOCLR1 = (1 << 18);      // EN=0
}

// Function to initialize the LCD
void LCD_init()
{
    LCD_command(0x38);   // 8-bit mode and 5x8 dots (function set)
    delay_ms(10);        // Delay
    LCD_command(0x0C);   // Display on, cursor off (display on/off)
    delay_ms(10);        // Delay
    LCD_command(0x06);   // Cursor increment and display shift (entry mode set)
    delay_ms(10);        // Delay
    LCD_command(0x01);   // Clear LCD (clear command)
    delay_ms(10);        // Delay
    LCD_command(0x80);   // Set cursor to 0th location 1st line
}

// Function to write a string to the LCD
void LCD_write_string(unsigned char *string)
{
    while (*string)           // Check for end of string
        LCD_data(*string++);  // Sending data on LCD byte by byte
}

int main(void)
{
    unsigned char i;

    PINSEL1 = 0x00;       // Configure PORT0 as GPIO --> LCD Data Line
    PINSEL2 = 0x00;       // Configure PORT1 as GPIO --> LCD Control Line
    IODIR1 = 0x07 << 16;  // Configure P1.18, P1.17, P1.16 as output
    IODIR0 = 0xFF << 16;  // Configure P0.23 - P0.16 as output
    
    LCD_init();           // Initialize LCD 16x2
    LCD_write_string("EMBEDDED"); // Display the string "EMBEDDED"
    
    while (1)
    {
        for (i = 0; i < 16; i++)  // Shift the display 16 times
        {
            LCD_command(0x1C);    // Command to shift the display to the right
            delay_ms(5);        // Delay to control the speed of scrolling
        }
        // Optional: Reset to the beginning after one full scroll
        LCD_command(0x02);        // Return home (reset the display)
        delay_ms(5);            // Small delay before starting the scroll again
    }
}
