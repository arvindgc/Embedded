/* Send Light intensity data via uart
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

#define LED (1<<31)          //LED on P0.31

#include "delay.h"


#define bit(x) (1<<x)


void UART(char []);
void transmit(unsigned char );
unsigned int adc(int ch);

unsigned int val;

unsigned int adc_val;


void ADC_INIT(void);
int ADC(void);
unsigned char high[30] = "Intensity high  ";
unsigned char low[30] = "Intensity low ";
void pll()
{
	PLL0CFG=0X24; //P=4(2 bits), M=2(5 bits)
	PLL0CON=0X01;  //enable PLL
	PLL0FEED=0XAA;  //feed sequence
	PLL0FEED=0X55;  //feed sequence
	while((PLL0STAT & (1<<10))==0);      //wait for PLL to latch on target freq
	PLL0CON=0X03;   //enable & connect PLL
	PLL0FEED=0XAA;    //feed sequence
	PLL0FEED=0X55; 
	
	VPBDIV=0X02;   
}

void transmit(unsigned char ch)
{
    while (!(U0LSR & 0x20));  // Wait until the THR is empty
    U0THR = ch;               // Transmit character
}

void UART(char *str){
    int i;
    
    PINSEL0 = 0x00000005;     // Select UART0 Rx and Tx pins
    U0LCR = 0x83;             // 8 bits, no Parity, 1 Stop bit, enable DLAB
    U0DLM = 0x00;              // divisor latch MSB
    U0DLL = 195;               // Set Baud rate 9600
    U0LCR = 0x03;             // DLAB = 0
    
    for(i = 0; str[i] != '\0'; i++) {
        transmit(str[i]);
    }
}

int main()
{    
	  
	  pll();
	
	
    IO1DIR = 0xffffffff;                   // Set Port1 for Output
    IO0DIR = 0x80000000 ;  // Set LED, Buzzer, and Relay as outputs
    
    PINSEL0 = 0x0300;                      // Set ADC pins
    VPBDIV = 0x02;                         // Set PCLK=30MHz 
    
    ADC_INIT();                            // Initialize ADC
    
    
    while(1) {
        // Gas Sensor logic
        
        val = adc(6);                      // Read LDR value from ADC0.6 (P0.23)
        
        if(val > 800) {
					
            UART(low);
            delay_ms(500);          // Turn on LED if light intensity is low
        } else {
           UART(high);
					// Turn off LED if light intensity is high
					 delay_ms(500);
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



