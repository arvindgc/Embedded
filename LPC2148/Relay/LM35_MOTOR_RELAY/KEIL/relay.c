/**************************************************************************************************
/* Use of relay to run the motor which operates on temperature data
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						Darshan K M(241039037)
						
Board 		: arm 7
Processor 	: LPC2148



********************************************************/
    


#include <lpc214x.h>


#include "delay.h"
#define RELAY (1<<20)       // Relay on P0.20

#define TEMP_LIMIT 35        // Temperature limit




unsigned int adc(int ch);

unsigned int val;
float temperature_c;
unsigned int adc_val;


void ADC_INIT(void);
int ADC(void);




int main()
{    
	   
    IO0DIR |= RELAY;  // Set LED, Buzzer, and Relay as outputs
    
    PINSEL0 = 0x0300;                      // Set ADC pins
    VPBDIV = 0x02;                         // Set PCLK=30MHz 
    
    ADC_INIT();                            // Initialize ADC
    
    
    while(1) {
        
       

        // Temperature Sensor logic (LM35)
        adc_val = adc(0);                  // Read temperature sensor value from ADC0.0 (P0.27)
        temperature_c = (adc_val * 3.3) / 10.24; // Convert ADC value to temperature in Celsius
        delay_ms(1000);                    // Delay for 1 second

        if (temperature_c >= TEMP_LIMIT) {
            IOSET0 = RELAY;                // Turn on relay if temperature exceeds the limit
        } else {
            IOCLR0 = RELAY;                // Turn off relay if temperature is below the limit
        }
    }
}



// ADC Functions
unsigned int adc(int ch)
{
    AD0CR = 0x00200600 | (1 << ch);  // Select channel for ADC0
    AD0CR |= (1 << 24);              // Start conversion
    while((AD0GDR & (1U << 31)) == 0); // Wait for conversion to complete
    val = AD0GDR;
    val = (val >> 6) & 0x03FF;       // Extract 10-bit ADC value
    return val;
}

void ADC_INIT(void)
{
    PINSEL1 &= 0xFF7FFFFF; // (PINSEL1<23> = 0)
    PINSEL1 |= 0x00400000; // (PINSEL1<22> = 1)
    // P0.27 is Configured as ADC Pin AD0.0
    PCONP |= (1<<12);      // Enable Power/Clock to ADC0
}

// Delay Function

