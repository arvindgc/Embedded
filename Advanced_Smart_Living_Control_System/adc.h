#include <lpc214x.h>

unsigned int adc(int ch)
{
    unsigned int val;

    // Step 1: Reset the ADC control register (AD0CR)
    AD0CR = 0x00000000; // Clear the control register to avoid unintended settings

    // Step 2: Select the ADC channel (e.g., channel 6 for LDR)
    AD0CR |= (1 << ch);  // Select the channel for ADC0

    // Step 3: Configure the ADC clock (ensure it's <= 4.5 MHz, using a prescaler of 13)
    AD0CR |= (13 << 8);  // Set the prescaler to ensure ADC clock <= 4.5 MHz

    // Step 4: Power up the ADC (PDN bit = 1)
    AD0CR |= (0x01 << 21); // Set the Power-Down bit to power up the ADC

    // Step 5: Start the conversion (bursts are not needed here)
    AD0CR |= (1 << 24);  // Start the conversion

    // Step 6: Wait for conversion to complete
    while ((AD0GDR & (1U << 31)) == 0); // Wait for the conversion to complete

    // Step 7: Extract the 10-bit ADC result from AD0GDR register
    val = AD0GDR;
    val = (val >> 6) & 0x03FF; // Extract the 10-bit value

    return val;  // Return the ADC value
}
