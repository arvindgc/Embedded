/* Working of  Stepper motor with 3 switches
   when 1st switch is pressed motor rotates clockwise
	 when 2nd switch is pressed  motor rotates anticlockwise 
	 when 3rd switch is pressed  motor stops
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148

Switch Mapping 	: Switch1 : P0.4,Switch2 : P0.5,Switch3 : P0.6 
motor Mapping 	: P0.0 P0.1 P0.2 p0.3



********************************************************/
#include <lpc214x.h>
#include "delay.h"
#define IN1_MASK 0x01  // IN1 connected to P0.0
#define IN2_MASK 0x02  // IN2 connected to P0.1
#define IN3_MASK 0x04  // IN3 connected to P0.2
#define IN4_MASK 0x08  // IN4 connected to P0.3
#define SWITCH1_MASK 0x10 // Switch 1 connected to P0.4 (Rotate clockwise)
#define SWITCH2_MASK 0x20 // Switch 2 connected to P0.5 (Rotate counterclockwise)
#define SWITCH3_MASK 0x40 // Switch 3 connected to P0.6 (Stop)



void step_motor(int step)
{
    switch(step)
    {
        case 0:
            IOSET0 = IN1_MASK; IOCLR0 = IN2_MASK | IN3_MASK | IN4_MASK;
            break;
        case 1:
            IOSET0 = IN1_MASK | IN2_MASK; IOCLR0 = IN3_MASK | IN4_MASK;
            break;
        case 2:
            IOSET0 = IN2_MASK; IOCLR0 = IN1_MASK | IN3_MASK | IN4_MASK;
            break;
        case 3:
            IOSET0 = IN2_MASK | IN3_MASK; IOCLR0 = IN1_MASK | IN4_MASK;
            break;
        case 4:
            IOSET0 = IN3_MASK; IOCLR0 = IN1_MASK | IN2_MASK | IN4_MASK;
            break;
        case 5:
            IOSET0 = IN3_MASK | IN4_MASK; IOCLR0 = IN1_MASK | IN2_MASK;
            break;
        case 6:
            IOSET0 = IN4_MASK; IOCLR0 = IN1_MASK | IN2_MASK | IN3_MASK;
            break;
        case 7:
            IOSET0 = IN1_MASK | IN4_MASK; IOCLR0 = IN2_MASK | IN3_MASK;
            break;
        default:
            IOCLR0 = IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;
            break;
    }
}

int main()
{
    unsigned int switch1_state = 0;
    unsigned int switch2_state = 0;
    unsigned int switch3_state = 0;
    int step = 0;
    int direction = 0; // 1 for clockwise, -1 for counterclockwise, 0 for stop

    PINSEL0 = 0x0000;       // Configure all Port 0 pins as GPIO
    IODIR0 = IODIR0 | (IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK); // Set stepper motor pins as outputs
    IODIR0 = IODIR0 & ~(SWITCH1_MASK | SWITCH2_MASK | SWITCH3_MASK); // Set switches as inputs

    while (1)
    {
        // Read state of switches
        switch1_state = (IOPIN0 & SWITCH1_MASK) >> 4; // Rotate clockwise
        switch2_state = (IOPIN0 & SWITCH2_MASK) >> 5; // Rotate counterclockwise
        switch3_state = (IOPIN0 & SWITCH3_MASK) >> 6; // Stop

        // Check switch states and update motor direction
        if (switch1_state == 1) // Rotate clockwise
        {
            direction = 1;
        }
        else if (switch2_state == 1) // Rotate counterclockwise
        {
            direction = -1;
        }
        else if (switch3_state == 1) // Stop
        {
            direction = 0;
        }

        // Control motor movement
        if (direction != 0)
        {
            // Rotate the motor through all steps
            step_motor(step); // Perform the current step
            step += direction; // Move to the next step
            if (step > 7) step = 0; // Loop back to the start
            if (step < 0) step = 7; // Loop back to the end
            delay_ms(20); // Adjust delay for desired speed
        }
        else
        {
            IOCLR0 = IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK; // Ensure motor is stopped
        }
    }
}
