#include <LPC214x.h>
// Function Declarations
void UART0_Init(void);
char UART0_ReceiveChar(void);
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);
void delay_ms(unsigned int ms);
void pll(void);
