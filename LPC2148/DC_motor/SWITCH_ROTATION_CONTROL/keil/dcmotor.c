/* Working of DC motor with 3 switches
   when 1st switch is pressed motor rotates clockwise
   when 2nd switch is pressed motor rotates anticlockwise 
   when 3rd switch is pressed motor stops
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148

Switch Mapping 	: Switch1 : P0.4,Switch2 : P0.5,Switch3 : P0.6 
Motor Mapping 	: P0.0 (IN1), P0.1 (IN2)

********************************************************/
#include <lpc214x.h>

#define IN1_MASK 0x01  // IN1 connected to P0.0 (Motor Forward)
#define IN2_MASK 0x02  // IN2 connected to P0.1 (Motor Backward)
#define SWITCH1_MASK 0x10 // Switch 1 connected to P0.4 (Rotate clockwise)
#define SWITCH2_MASK 0x20 // Switch 2 connected to P0.5 (Rotate counterclockwise)
#define SWITCH3_MASK 0x40 // Switch 3 connected to P0.6 (Stop)


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

int main()
{
    unsigned int switch1_state = 0;
    unsigned int switch2_state = 0;
    unsigned int switch3_state = 0;

    PINSEL0 = 0x0000;       // Configure all Port 0 pins as GPIO
    IODIR0 = IODIR0 | (IN1_MASK | IN2_MASK); // Set motor pins as outputs
    IODIR0 = IODIR0 & ~(SWITCH1_MASK | SWITCH2_MASK | SWITCH3_MASK); // Set switches as inputs

    while (1)
    {
        // Read state of switches
        switch1_state = (IOPIN0 & SWITCH1_MASK) >> 4; // Rotate clockwise
        switch2_state = (IOPIN0 & SWITCH2_MASK) >> 5; // Rotate counterclockwise
        switch3_state = (IOPIN0 & SWITCH3_MASK) >> 6; // Stop

        // Check switch states and control motor
        if (switch1_state == 1) // Rotate clockwise
        {
            IOSET0 = IN1_MASK;  // Set IN1 to HIGH (forward)
            IOCLR0 = IN2_MASK;  // Set IN2 to LOW
        }
        else if (switch2_state == 1) // Rotate counterclockwise
        {
            IOSET0 = IN2_MASK;  // Set IN2 to HIGH (backward)
            IOCLR0 = IN1_MASK;  // Set IN1 to LOW
        }
        else if (switch3_state == 1) // Stop
        {
            IOCLR0 = IN1_MASK | IN2_MASK; // Stop the motor by setting both IN1 and IN2 to LOW
        }

        delay_ms(10); // Adjust delay for switch debounce
    }
}
