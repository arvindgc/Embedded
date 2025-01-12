/********************************************************/
/* Working of LCD
   The string "EMBEDDED" will display at the top and move from left to right
	 The string "SYSTEM" will display at the bottom and move from right to left
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
    delay_ms(2);            // Delay
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
    delay_ms(2);            // Delay
    IOCLR1 = (1 << 18);      // EN=0
}

// Function to initialize the LCD
void LCD_init()
{
    LCD_command(0x38);   // 8-bit mode and 5x8 dots (function set)
    delay_ms(3);        // Delay
    LCD_command(0x0C);   // Display on, cursor off (display on/off)
    delay_ms(3);        // Delay
    LCD_command(0x06);   // Cursor increment and display shift (entry mode set)
    delay_ms(3);        // Delay
    LCD_command(0x01);   // Clear LCD (clear command)
    delay_ms(3);        // Delay
}

// Function to write a string to the LCD
void LCD_write_string(unsigned char *string)
{
    while (*string)           // Check for end of string
        LCD_data(*string++);  // Sending data on LCD byte by byte
}

// Function to clear a line of the LCD
void LCD_clear_line(unsigned char line)
{
    unsigned char i;
    if (line == 1)
        LCD_command(0x80);  // Move to the start of the first line
    else if (line == 2)
        LCD_command(0xC0);  // Move to the start of the second line
    
    for (i = 0; i < 16; i++)
        LCD_data(' ');  // Write spaces to clear the line
}

// Function to display "EMBEDDED" moving left to right on the top line
void display_top_line(unsigned char *string, unsigned char pos)
{
    LCD_clear_line(1);              // Clear the top line
    LCD_command(0x80 + pos);        // Move cursor to the current position
    LCD_write_string(string);       // Write "EMBEDDED"
}

// Function to display "SYSTEM" moving right to left on the bottom line
void display_bottom_line(unsigned char *string, unsigned char pos)
{
    LCD_clear_line(2);              // Clear the bottom line
    LCD_command(0xC0 + pos);        // Move cursor to the current position
    LCD_write_string(string);       // Write "SYSTEM"
}

int main(void)
{
    unsigned char top_pos = 0;
    unsigned char bottom_pos = 16 - 6; // Start position for "SYSTEM"
    unsigned char top_len = 8;        // Length of "EMBEDDED"
    unsigned char bottom_len = 6;     // Length of "SYSTEM"

    PINSEL1 = 0x00;       // Configure PORT0 as GPIO --> LCD Data Line
    PINSEL2 = 0x00;       // Configure PORT1 as GPIO --> LCD Control Line
    IODIR1 = 0x07 << 16;  // Configure P1.18, P1.17, P1.16 as output
    IODIR0 = 0xFF << 16;  // Configure P0.23 - P0.16 as output
    
    LCD_init();  // Initialize LCD 16x2

    while (1)
    {
        // Update and display the top line
        display_top_line("EMBEDDED", top_pos);
        top_pos++;
        if (top_pos > 16 - top_len)  // Reset position when it reaches the end
            top_pos = 0;

        // Update and display the bottom line
        display_bottom_line("SYSTEM", bottom_pos);
        if (bottom_pos == 0)
            bottom_pos = 16 - bottom_len;  // Reset position when it reaches the start
        else
            bottom_pos--;

        delay_ms(3);  // Adjust speed here
    }
}
