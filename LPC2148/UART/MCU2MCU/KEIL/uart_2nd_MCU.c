/* controlling light connected to mcu2 via mcu1 using UART
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148


********************************************************/




#include "header.h"

void pll()
{
	PLL0CFG=0x24; //P=4(2 bits), M=2(5 bits)
	PLL0CON=0x01;  //enable PLL
	PLL0FEED=0xAA;  //feed sequence
	PLL0FEED=0x55;  //feed sequence
	while((PLL0STAT & (1<<10))==0);      //wait for PLL to latch on target freq
	PLL0CON=0x03;   //enable & connect PLL
	PLL0FEED=0xAA;    //feed sequence
	PLL0FEED=0x55; 
	
	VPBDIV=0X02;   // pclk 30MHz
}

// UART0 Initialization for MCU2
void UART0_Init(void) {
    PINSEL0 |= 0x00000005;    // Enable UART0 pins (P0.0 RXD0, P0.1 TXD0)
    U0LCR = 0x83;             // 8-bit data, 1 stop bit, no parity, DLAB = 1
    U0DLM = 0x00;
    U0DLL = 195;               // Baud rate 9600 
    U0LCR = 0x03;             // DLAB = 0
}


// Receive a character over UART0
char UART0_ReceiveChar(void) {
    while (!(U0LSR & 0x01));  // Wait for the Receive Data Ready flag
    return U0RBR;             // Return the received character
}

// LCD Initialization in 8-bit mode
void LCD_Init(void) {
    IO0DIR |= (1 << 16) | (1 << 17) | (0xFF << 8);  // Set LCD control and data pins as output

    // Commands to initialize LCD in 8-bit mode
    LCD_Command(0x38);  // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C);  // Display on, cursor off
    LCD_Command(0x06);  // Auto increment cursor
    LCD_Command(0x01);  // Clear display
    delay_ms(2);
}

// Send LCD Command (8-bit mode)
void LCD_Command(unsigned char cmd) {
    IO0CLR = (1 << 16);        // RS = 0 (command mode)
    IO0CLR = (1 << 17);        // RW = 0 (write mode)

    IO0CLR = 0xFF << 8;        // Clear all data lines (D0-D7)
    IO0SET = cmd << 8;         // Place command on data lines (D0-D7)

    IO0SET = (1 << 17);        // Enable high
    delay_ms(1);               // Delay for LCD to latch the command
    IO0CLR = (1 << 17);        // Enable low
    delay_ms(2);
}

// Send a Character to LCD (8-bit mode)
void LCD_Char(unsigned char data) {
    IO0SET = (1 << 16);        // RS = 1 (data mode)
    IO0CLR = (1 << 17);        // RW = 0 (write mode)

    IO0CLR = 0xFF << 8;        // Clear all data lines (D0-D7)
    IO0SET = data << 8;        // Place data on data lines (D0-D7)

    IO0SET = (1 << 17);        // Enable high
    delay_ms(1);               // Delay for LCD to latch the data
    IO0CLR = (1 << 17);        // Enable low
    delay_ms(2);
}

// Send a String to LCD
void LCD_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);  // Send each character to LCD
    }
}

// Delay Function
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 2000; j++);
}

// Main Program for MCU2
int main(void) {
    char receivedChar;
		pll();

    // Set LED pin as output (P1.17 for LED control)
    IO1DIR |= (1 << 17);  // Set P1.17 as output for LED control

    LCD_Init();  // Initialize LCD in 8-bit mode
    UART0_Init();  // Initialize UART0 for communication

    while (1) {
        receivedChar = UART0_ReceiveChar();  // Receive data from MCU1 via UART

        if (receivedChar == 'L') {  // If 'L' is received, LED ON
            IO1SET = (1 << 17);  // Turn on LED (P1.17)
            LCD_Command(0x80);  // Move to the first line, first position
            LCD_String("LED ON");  // Display "LED ON" on the LCD
						delay_ms(200);
        } else if (receivedChar == 'F') {  // If 'F' is received, LED OFF
            IO1CLR = (1 << 17);  // Turn off LED (P1.17)
            LCD_Command(0x80);  // Move to the first line, first position
            LCD_String("LED OFF");  // Display "LED OFF" on the LCD
						delay_ms(200);
						LCD_Command(0x01);  // Send clear display command
        }
    }
}
