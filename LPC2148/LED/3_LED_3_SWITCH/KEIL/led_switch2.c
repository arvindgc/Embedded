/********************************************************/
/* Working of 3 LEDs with 3 switchs
   when 1st switch is pressed LED1 turn on 
	 when 2nd switch is pressed LED2 turn on 
	 when 3rd switch is pressed LED3 turn on 
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148

Switch Mapping 	: Switch : P0.3
LED Mapping 	: LED1 : P0.0, LED2 : P0.1, LED3 : P0.2,



********************************************************/


#include <lpc214x.h>
#include "delay.h"
#define LED1_MASK 0x01  // LED connected to P0.0
#define LED2_MASK 0x02  // LED connected to P0.1
#define LED3_MASK 0x04  // LED connected to P0.2
#define SWITCH1_MASK 0x08 // Switch 1 connected to P0.3
#define SWITCH2_MASK 0x10 // Switch 2 connected to P0.4
#define SWITCH3_MASK 0x20 // Switch 3 connected to P0.5

int main()
{
    unsigned int switch1_state = 0;
    unsigned int switch2_state = 0;
    unsigned int switch3_state = 0;

    PINSEL0 = 0x0000;       // Configure all Port 0 pins as GPIO
    IODIR0 = IODIR0 | (LED1_MASK | LED2_MASK | LED3_MASK); // Set P0.0, P0.1, and P0.2 as outputs (LEDs)
    IODIR0 = IODIR0 & ~(SWITCH1_MASK | SWITCH2_MASK | SWITCH3_MASK); // Set P0.3, P0.4, and P0.5 as inputs (Switches)

    while (1)
    {
        switch1_state = (IOPIN0 & SWITCH1_MASK) >> 3; // Read the state of Switch 1
        switch2_state = (IOPIN0 & SWITCH2_MASK) >> 4; // Read the state of Switch 2
        switch3_state = (IOPIN0 & SWITCH3_MASK) >> 5; // Read the state of Switch 3

        if (switch1_state == 1) // Active-high switch, so 1 means pressed
        {
            IOSET0 = LED1_MASK; // Turn on LED1
        }
        else
        {
            IOCLR0 = LED1_MASK; // Turn off LED1
        }

        if (switch2_state == 1) // Active-high switch, so 1 means pressed
        {
            IOSET0 = LED2_MASK; // Turn on LED2
        }
        else
        {
            IOCLR0 = LED2_MASK; // Turn off LED2
        }

        if (switch3_state == 1) // Active-high switch, so 1 means pressed
        {
            IOSET0 = LED3_MASK; // Turn on LED3
        }
        else
        {
            IOCLR0 = LED3_MASK; // Turn off LED3
        }

        delay_ms(20); // Small delay to debounce the switches
    }
}
