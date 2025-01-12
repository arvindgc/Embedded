/********************************************************/
/* Working of 3 LEDs with one switch
   first press  --> only LED1 on
	 second press --> only LED2 on
	 third press  --> only LED3 on
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148

Switch Mapping 	: Switch1 : P0.3, Switch2 : P0.4, Switch3 : P0.5
LED Mapping 	: LED1 : P0.0, LED2 : P0.1, LED3 : P0.2,



********************************************************/

#include <lpc214x.h>
#include "delay.h"
#define LED1_MASK 0x01  // LED connected to P0.0
#define LED2_MASK 0x02  // LED connected to P0.1
#define LED3_MASK 0x04  // LED connected to P0.2
#define SWITCH_MASK 0x08 // Switch connected to P0.3


int main()
{
    unsigned int switch_state = 0;
    unsigned int last_switch_state = 0;
    unsigned int led_state = 0;
    unsigned int count = 0;

    PINSEL0 = 0x0000;       // Configure all Port 0 pins as GPIO
    IODIR0 = IODIR0 | (LED1_MASK | LED2_MASK | LED3_MASK);  // Set P0.0, P0.1, and P0.2 as outputs (LEDs)
    IODIR0 = IODIR0 & (~SWITCH_MASK); // Set P0.3 as input (Switch)

    while (1)
    {
        switch_state = (IOPIN0 & SWITCH_MASK) >> 3; // Read the switch state (active-high)

        if (switch_state == 1 && last_switch_state == 0) // Detect rising edge (switch press)
        {
            count++;
            
            if(count == 1){
                IOCLR0 = LED3_MASK; // Clear LED3
                IOSET0 = LED1_MASK; // Turn on LED1
            }
            else if(count == 2){
                IOCLR0 = LED1_MASK; // Clear LED1
                IOSET0 = LED2_MASK; // Turn on LED2
            }
            else if(count == 3){
                IOCLR0 = LED2_MASK; // Clear LED2
                IOSET0 = LED3_MASK; // Turn on LED3
            }
            else if(count == 4){
                IOCLR0 = LED3_MASK; // Clear LED3
                IOSET0 = LED1_MASK; // Turn on LED1
                count = 1; // Reset count to 1
            }
            
            delay_ms(20); // Debounce delay
        }
        
        last_switch_state = switch_state; // Update last switch state
    }
}
