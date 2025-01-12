#include <lpc214x.h>

void transmit(unsigned char ch)
{
    while (!(U0LSR & 0x20));  // Wait until the THR is empty
    U0THR = ch;               // Transmit character
}

void UART(){
    int i;
    unsigned char temp[30] = "Gas Detected  ";
    PINSEL0 |= 0x00000005;     // Select UART0 Rx and Tx pins
    U0LCR = 0x83;             // 8 bits, no Parity, 1 Stop bit, enable DLAB
    U0DLM = 0x01;              // divisor latch MSB
    U0DLL = 0x87;               // Set Baud rate 9600
    U0LCR = 0x03;             // DLAB = 0
    
    for(i = 0; temp[i] != '\0'; i++) {
        transmit(temp[i]);
			  
    }
}