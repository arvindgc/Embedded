
#include <lpc214x.h>

void delay_ms(unsigned int ms) {
    T0TCR = 0x02;  // Reset Timer
    T0PR  = 59999; // Prescaler for 1 ms (PCLK 60 MHz, 60,000 ticks per ms)
    T0TCR = 0x01;  // Enable Timer

    while (ms > 0) {
        // Wait until Timer reaches the match value
        while (T0TC < 1);  
        T0TC = 0;  // Reset the timer counter
        ms--;      // Decrement the ms count
    }

    T0TCR = 0x00;  // Disable Timer
}