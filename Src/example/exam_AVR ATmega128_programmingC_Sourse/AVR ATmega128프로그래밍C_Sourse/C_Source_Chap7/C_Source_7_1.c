//AVR128  UART_1.C
#include<mega128.h>

#define SYSTEM_CLOCK  16000000   // CLOCK (X-tal frequency)  

void putch(char c)//MCU -> PC로 문자 출력
{
 while(!(UCSR0A & 0x20));
 UDR0=c;
}

char getch()//PC -> MCU
{
 while(!(UCSR0A & 0x80));
 return UDR0;
}


void UART_Init(unsigned int setbauerate)
{
 unsigned long baudrate;
 UCSR0B=0x18; //RXEN0,TXEN0 enable 송.수신
 baudrate = (SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // 보율값 설정
          
}


void main()
{
  UART_Init(38400);
   
  while(1)
  {
   putch(getch());//키보드에서 받은 문자를 터미널로 출력한다.
  }
}
