#include <lpc213x.h>
#include <stdint.h>

#define UART0_BAUD 9600
#define FREQ 60000000 // Assume a 12 MHz clock

void UART0_Init(void) {
    // Set the baud rate
    U0LCR = 0x83; // 8 bits, no parity, 1 stop bit, DLAB = 1
    U0DLL = (FREQ / (16 * UART0_BAUD)); // Load divisor latch
    U0DLM = 0; // High byte of the divisor latch
    U0LCR = 0x03; // DLAB = 0
    U0FCR = 0x07; // Enable and reset the FIFO
}

void UART0_SendChar(char ch) {
    while (!(U0LSR & 0x20)); // Wait for the transmitter to be ready
    U0THR = ch; // Transmit the character
}

void UART0_SendString(const char *str) {
    while (*str) {
        UART0_SendChar(*str++);
    }
}

void ADC_Init(void) {
    PINSEL1 |= 0x00000001; // Select ADC function for P0.31 (AD0.0)
    AD0CR = (1 << 0) | (1 << 21) | (4 << 8); // AD0.0, clock = CCLK/4
}

uint16_t ADC_Read(void) {
    AD0CR |= (1 << 24); // Start conversion
    while (!(AD0GDR & (1 << 31))); // Wait for conversion to complete
    return (AD0GDR >> 4) & 0xFFF; // Return the 12-bit result
}

int main(void) {
    uint16_t sensorValue;
	  int i;
    char buffer[20];

    // Initialize UART0 and ADC
    UART0_Init();
    ADC_Init();

    while (1) {
        sensorValue = ADC_Read(); // Read sensor value
        snprintf(buffer, sizeof(buffer), "Value: %d\n", sensorValue);
        UART0_SendString(buffer); // Send the value over UART
        for ( i = 0; i < 1000000; i++); // Simple delay
    }

    return 0;
}