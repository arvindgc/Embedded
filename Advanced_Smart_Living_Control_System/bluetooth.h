

#include <lpc214x.h>

void delams(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 2000; j++);
}




// UART0 Initialization for MCU2
void UART0_Init(void) {
    
	
    PINSEL0 |= 0x00000005;     // Select UART0 Rx and Tx pins
    U0LCR = 0x83;             // 8 bits, no Parity, 1 Stop bit, enable DLAB
    U0DLM = 0x01;              // divisor latch MSB
    U0DLL = 0x87;               // Set Baud rate 9600
    U0LCR = 0x03;             // DLAB = 0
}


// Receive a character over UART0
char UART0_ReceiveChar(void) {
  //  while (!(U0LSR & 0x01));  // Wait for the Receive Data Ready flag
    return U0RBR;             // Return the received character
}



// Main Program for MCU2
void bluetooth(void) {
    char receivedChar;
		


   
    UART0_Init();  // Initialize UART0 for communication

    
        receivedChar = UART0_ReceiveChar();  // Receive data from MCU1 via UART

        if (receivedChar == 'O') {  // If 'L' is received, LED ON
            IO0SET = (1 << 25);  // Turn on LED (P0.25)
           
						delams(200);
        } else if (receivedChar == 'F') {  // If 'F' is received, LED OFF
            IO0CLR = (1 << 25);  // Turn off LED (P0.25)
            
						delams(200);
						
        }
    
}
