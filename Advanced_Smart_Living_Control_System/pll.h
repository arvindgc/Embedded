#include <lpc214x.h>

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
	
	VPBDIV=0X01;   //pclk=cclk=60MHz
}