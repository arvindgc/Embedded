#include<lpc213x.h>
#include<string.h>

////////////////////////
#define SLAVE_ADDR 0x70     
#define MAX        12
#define AA         2
#define SI         3
#define STO        4
#define STA        5
#define I2EN       6

////////////////////////////
//         Delay Function
//
void wait(int count){
      while(count-- );
}
//////////////////////////
//        Initialize I2c
//
//
//
void I2C_Init (void)
{
   VPBDIV    =   0x01;        // sets PCLK = FOSC = 60 MHz
   PINSEL0   =   0x00000050;  // set P0.2 and P0.3 to SDA and SCL
   
   I2C0SCLH   =   300;        // 50% duty cycle, I2C Frequency -> 100 kHz, PCLK = 60 MHz
   I2C0SCLL   =   300;

   I2C0CONSET  =  (1 << I2EN);  // Enable I2C module
}
//////////////////////////
//     Enter Master 
//     transmitter Mode
//
int I2C_Start ()
{
   I2C0CONCLR   =   1 << STO; 
   I2C0CONCLR = 1 << SI;   
   I2C0CONSET   =   1 << STA; 
   return 0;
}

//////////////////////////////////////
//      Delay function
//
//
void delay__ms(int count)
{
  int j=0,i=0;
 
  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}
///////////////////////////////////////////////////////
//      Function to send data from Master to slave I2C
//      device
//
void senddata(char data){ 
     while(!(I2C0CONSET & 0x08)); 		 
		 I2C0DAT      =   data; 
		 I2C0CONCLR   =   1 << SI;
		 delay__ms(200);
} 
///////////////////////////////////////////////////
//      Function to send a byte of char data
//      EX:if data = 0x12 higher nibble 
//      is sent first in this case 0x1 and then lower
//      nibble.whyy? Because slave is set to 4bit mode.
//      The Format shud be :7  6   5  4 3 2 1 0
//                          0  RS RW  E|D A T A
//
void sendchar(unsigned char data){     
	 
	   senddata(0x50 | (data>>4));delay__ms(50);
	   senddata(0x40 | (data>>4));delay__ms(50);
		 senddata(0x50 | (data&0x0f)); delay__ms(50);
	   senddata(0x40 | (data& 0x0f));delay__ms(50);
		 delay__ms(50);
} 

void init_lcd(){
	
		  char code =  SLAVE_ADDR;// 0x6e;
		  
		 
		  int i =0;

		  I2C_Init();                  //Initialize I2C moodule		
			I2C_Start();                 //start I2C module 
     	wait(4000);

			while(!(I2C0CONSET & 0x08)); //wait till SI bit set
			IO0SET = (1<<21);
      I2C0CONCLR	 =	 1 << STO;
			I2C0CONCLR	 =	 1 << STA;				
			I2C0CONSET   =   1 << AA;
			I2C0DAT      =  code;
			
			I2C0CONCLR   =   1 << SI; 
			//wait(4000);	
			while(!(I2C0CONSET & 0x08));
			if(I2C0STAT == 0x18)
			{	  
					 IO0SET = (1<<23);	  	   	 
				   I2C0CONSET   =   1 << AA;
					 I2C0DAT      =   0x00;//x00;//Buff[index]; 
				   I2C0CONCLR   =   1 << SI; 
				   while(!(I2C0CONSET & 0x08));
					 for(i=0; i < 2000;i++)wait(800);
				   if(I2C0STAT == 0x28)
					 {		 

             //SEND LCD INITIALIING COMMANDS TO SET LCD MODULE IN 4 BIT ,5X7,2 LINES							
						   
										
						 
						 senddata(0x00);
						 senddata(0x10);
					                    //02
						 senddata(0x02);
						 senddata(0x12);
				     
						 senddata(0x02);
						 senddata(0x12);
						                 //28
						 senddata(0x00);
						 senddata(0x10);
						 
						 senddata(0x00);
						 senddata(0x10);
						                //0c
						 senddata(0x0C);
						 senddata(0x1C);
						 
						 senddata(0x00);
						 senddata(0x10);
						                 //06
						 senddata(0x06);
						 senddata(0x16);
						 
						 senddata(0x00);
						 senddata(0x10);
						                 //01
						 senddata(0x01);
						 senddata(0x11);
						 
						 senddata(0x08);
						 senddata(0x18);
						                 //80
						 senddata(0x00);
						 senddata(0x10);
				
					
					  
					 
			    }			
			}
}

void show_on_lcd(unsigned char *s)
{
    while(*s) {
        sendchar(*s++);
    }
}