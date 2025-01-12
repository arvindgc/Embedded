/* controlling light connected to mcu2 from mcu1
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148


********************************************************/

#include <LPC214x.h>

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
	
	VPBDIV=0X02;  //30MHz pclk 
}

// UART0 Initialization for MCU1
void UART0_Init(void) {
    PINSEL0 |= 0x00000005;    // Enable UART0 pins (P0.0 TXD0, P0.1 RXD0)
    U0LCR = 0x83;             // 8-bit data, 1 stop bit, no parity, DLAB = 1
    U0DLM = 0x00;
    U0DLL = 195;               // Baud rate 9600
    U0LCR = 0x03;             // DLAB = 0
}

// Send a character over UART0
void UART0_SendChar(char c) {
    while (!(U0LSR & 0x20));  // Wait until the Transmit Holding Register is empty
    U0THR = c;                // Send the character
}

// Send a string over UART0
void UART0_SendString(const char *str) {
    while (*str) {
        UART0_SendChar(*str++);
    }
}

// Delay function
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 2000; j++);
}

// Main program for MCU1
int main(void) {
		pll();
    // Set LED pin as output
    IO0DIR |= (1 << 8);  // P0.8 as output for LED

    UART0_Init();  // Initialize UART0

    while (1) {
        IO0SET = (1 << 8);     // Turn on LED
        UART0_SendString("LED ON\n");  // Send "LED ON" to MCU2
        delay_ms(10);

        IO0CLR = (1 << 8);     // Turn off LED
        UART0_SendString("LED OFF\n");  // Send "LED OFF" to MCU2
        delay_ms(10);
    }
}
