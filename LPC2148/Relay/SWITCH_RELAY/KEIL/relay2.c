/**************************************************************************************************
/* Use of relay which opeates on swicth status
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148



********************************************************/


#include <lpc214x.h>
#include "delay.h"
#define RELAY_PIN (1 << 0)  // Bitmask for pin 0.0 (Relay Control)
#define SWITCH_PIN (1 << 1) // Bitmask for pin 0.1 (Switch Input)



int main() {
    // Configure Pin 0.0 as output for relay
    IO0DIR |= RELAY_PIN;
    IO0CLR = RELAY_PIN;  // Ensure the relay is initially off

    // Configure Pin 0.1 as input for switch
    IO0DIR &= ~SWITCH_PIN;  // Set pin 0.1 as input
    IO0PIN &= ~SWITCH_PIN;  // Ensure the pin is initially low

    while (1) {
        if (IO0PIN & SWITCH_PIN) {   // Check if the switch is pressed
            IO0SET = RELAY_PIN;      // Turn ON the relay
        } else {
            IO0CLR = RELAY_PIN;      // Turn OFF the relay
        }

        delay_ms(20); // Short delay to debounce the switch
    }

    return 0;  // Although this will never be reached
}