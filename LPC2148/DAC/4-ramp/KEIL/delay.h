#include <lpc214x.h>

void delay_us(unsigned int us) {
    T0TCR = 0x02;  // Reset Timer
    T0PR  = 59;    // Prescaler for 1 microsecond (PCLK 60 MHz, 60 ticks per us)
    T0TCR = 0x01;  // Enable Timer

   // while (us > 0) {
        // Wait until Timer reaches the match value
     //   while (T0TC < 1);  
       // T0TC = 0;  // Reset the timer counter
       // us--;      // Decrement the us count
   // }
while(T0TC <us);
    T0TCR = 0x00;  // Disable Timer
}
