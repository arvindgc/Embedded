/* Working of DC motor with gas sensor
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148


Motor Mapping 	: P0.0 (IN1), P0.1 (IN2)

********************************************************/
#include <lpc214x.h>

#define IN1_MASK 0x01  // IN1 connected to P0.0 (Motor Forward)
#define IN2_MASK 0x02  // IN2 connected to P0.1 (Motor Backward)
#define GAS_SENSOR_PIN (1<<6)  // Gas sensor on P0.6

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

int main() {
    // Configure all Port 0 pins as GPIO
    PINSEL0 = 0x00000000;  // Configure Port 0 pins as GPIO

    // Set motor control pins as output
    IODIR0 |= (IN1_MASK | IN2_MASK);  // P0.0 and P0.1 as output for motor control

    // Set gas sensor pin as input
    IODIR0 &= ~GAS_SENSOR_PIN;  // P0.6 as input for gas sensor

    while (1) {
        // Gas Sensor logic
        if (IOPIN0 & GAS_SENSOR_PIN) {
            // If gas sensor is triggered (high)
            IOSET0 = IN1_MASK;  // Set IN1 to HIGH (motor forward)
            IOCLR0 = IN2_MASK;  // Set IN2 to LOW (ensure motor moves forward)
        } else {
            // If gas sensor is not triggered, stop the motor
            IOCLR0 = IN1_MASK | IN2_MASK;  // Stop the motor
        }

        delay_ms(100);  // Small delay to avoid rapid toggling
    }
}
