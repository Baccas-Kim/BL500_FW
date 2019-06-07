//AVR128  UART_2.C
#include<mega128.h>
#include<delay.h>

#define SYSTEM_CLOCK  8000000   // CLOCK (X-tal frequency)  


void UART_Init(unsigned int setbauerate)
{
 unsigned long baudrate;
 UCSR0B=0x18; //RXEN0,TXEN0 enable 송.수신
 baudrate = (SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // 보율값 설정
}

void putch(char c)//MCU -> PC로 문자 출력
{
 while(!(UCSR0A & 0x20));
 UDR0=c;
}

char getch()//PC -> MCU
{
 char get_byte;
 while(!(UCSR0A & 0x80));         
 get_byte =  UDR0;
 return get_byte;
}

void main()
{
   char in_data=0;
   DDRC=0xFF;
   SREG=0x80; 
   UART_Init(38400); // baudrate 설정
   PORTC=0xFF;
  while(1)
  {
  
    in_data = getch(); 
    
    switch(in_data)
    {
     case '1' : PORTC=0x7F; putch('1'); break; 
     case '2' : PORTC=0xBF; putch('2'); break;
     case '3' : PORTC=0xDF; putch('3'); break;
     case '4' : PORTC=0xEF; putch('4'); break;
     case '5' : PORTC=0xF7; putch('5'); break;
     case '6' : PORTC=0xFB; putch('6'); break;
     case '7' : PORTC=0xFD; putch('7'); break;
     case '8' : PORTC=0xFE; putch('8'); break;
    }  
         
  }
}
