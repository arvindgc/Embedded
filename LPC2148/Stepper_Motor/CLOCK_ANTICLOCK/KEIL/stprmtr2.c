/* Working of  Stepper motor
   motor rotates clockwise for few seconds and anticlockwise for few seconds
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148

motor Mapping 	: P0.0 P0.1 P0.2 p0.3



********************************************************/

#include <lpc214x.h>
#include "delay.h"
#define IN1_MASK 0x01  // IN1 connected to P0.0
#define IN2_MASK 0x02  // IN2 connected to P0.1
#define IN3_MASK 0x04  // IN3 connected to P0.2
#define IN4_MASK 0x08  // IN4 connected to P0.3



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

void rotate_clockwise()
{
    int i,step = 0;
    for (i = 0; i < 20; i++)  
    {
        step_motor(step);
        step++;
        if (step > 7) step = 0;
        delay_ms(20);
    }
}

void rotate_counterclockwise()
{
    int i,step = 7;
    for (i = 0; i < 20; i++)  
    {
        step_motor(step);
        step--;
        if (step < 0) step = 7;
        delay_ms(20);
    }
}

int main()
{
    PINSEL0 = 0x00000000;  // Configure all Port 0 pins as GPIO
    IODIR0 = IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;  // Set stepper motor pins as outputs

    while (1)
    {
        rotate_clockwise();
        delay_ms(50);  
        rotate_counterclockwise();
        delay_ms(50);  
    }
}
