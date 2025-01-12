/**************************************************************************************************
ADVANCED SMART LIVING CONTROL SYSTEM

Author       : Sahana S (241039028)
             : Arvind G Choudhari(241039036)
             
						 
Board        : LPC2148

Hardware Setup:-
    LDR input          : P0.4 (ADC0.6)
    LM35 input         : P0.27 (ADC0.0)
    Buzzer             : P0.7
    LED                : P0.25,P0.31
		GAS SENSOR         : P0.6 
		FAN                : P0.20
		LCD(I2C)           : P0.2,P0.3
		Bluetooth          : P0.1(Rx)
    cclk               : 60MHz
		pclk               : 60MHz
		                
This "Advanced Smart Living Control System automates and controls home devices based on environmental inputs. 
It uses an LDR sensor to manage lighting, a temperature sensor to operate a fan, and a gas sensor to activate a safety buzzer. 
Additionally, Bluetooth connectivity enables remote light control, enhancing convenience and safety.
    
********************************************************************************/

#include <lpc214x.h>
#include "pll.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"
#include "lcd.h"
#include "gpio.h"
#include "i2c.h"
#include "bluetooth.h"
#define TEMP_LIMIT 35        // Temperature limit



unsigned int val;
float temperature_c;
unsigned int adc_val;


int main()
{    
	  
	  pll();            //initialize pll,  pclk=cclk=60MHz
	  init_gpio();      //initialize gpio
    init_lcd();       //initialize lcd
	
    while(1) {
			
        // Gas Sensor logic
        if (IOPIN0 & GAS_SENSOR_PIN) {  // Check if gas sensor is triggered
					
					UART();
					  
            IOSET0 = BUZZER;           // Turn on the buzzer
					
					
        } else {
            IOCLR0 = BUZZER;           // Turn off the buzzer
        }
        
        
      
				senddata(0x08);
				senddata(0x18);
						                  // cmd(0x80);  
				senddata(0x00);
				senddata(0x10);
				
 
        show_on_lcd("LIGHT INT : ");
				
				
				//LDR Logic
				
				val = adc(6);  // Read LDR value from ADC0.6 (P0.23)
        if(val > 800) {
            show_on_lcd("LOW ");
            IOSET0 |= LED;                  // Turn on LED if light intensity is low
        } else {
            show_on_lcd("HIGH");
            IOCLR0 |= LED;                  // Turn off LED if light intensity is high
        }

        
				// Temperature Sensor logic (LM35)
        adc_val = adc(0);                  // Read temperature sensor value from ADC0.0 (P0.27)
        
				
				temperature_c = (adc_val * 3.3 * 100) / 1024.0;  // Convert ADC value to temperature in Celsius
 
        delay_ms(100);                   

        if (temperature_c >= TEMP_LIMIT) {
            IOSET0 |= RELAY;                // Turn on relay if temperature exceeds the limit
        } else {
            IOCLR0 |= RELAY;                // Turn off relay if temperature is below the limit
        }
				
				bluetooth();
    }
}







