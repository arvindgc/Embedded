/********************************************************/
/* Sine waveform at 50hz using 100 samples
*******************************************************
Author 		: Arvind G Choudhari(241039036)
            Anand G Choudhari(241039011
						
						
Board 		: arm 7
Processor 	: LPC2148


DAC pin P0.25

********************************************************/


#include <lpc214x.h>
#include "delay.h"

void analogwrite(unsigned int dacdata){
	DACR=dacdata<<6;
}

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
	
	VPBDIV=0X01;    
}

int main(){
	
	int i;
	pll();
	PINSEL1 |= (0x2 << 18); //pin 0.25 as DAC
	
	while(1)
	{
		for(i=0;i<100;i++)
		{
					analogwrite(sinedata[i]);
          delay_us(200); 
		}
		
		
	}
} 