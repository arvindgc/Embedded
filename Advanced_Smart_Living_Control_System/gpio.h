
#include <lpc214x.h>

#define LED (1U<<31)          //LED on P0.31
#define BUZZER (1<<7)       // Buzzer on P0.7
#define RELAY (1<<20)       // Relay on P0.20
#define GAS_SENSOR_PIN (1<<6)  // Gas sensor on P0.6

void init_gpio(){
	
IO0DIR = LED | BUZZER | RELAY ;  // Set LED, Buzzer, and Relay as outputs
IO0DIR &= ~GAS_SENSOR_PIN; 	// Set gas sensor as input
IO0DIR |= (1 << 25);  // Set P0.25 as output for LED bluetooth control
IO1DIR = 0xffffffff;                   // Set Port1 for Output
	
PINSEL0 |= 0x0300;                      // Set p0.4 for ADC0.6
PINSEL1 |= 0x00400000;        // P0.27 is Configured as ADC Pin AD0.0
	  
	
}