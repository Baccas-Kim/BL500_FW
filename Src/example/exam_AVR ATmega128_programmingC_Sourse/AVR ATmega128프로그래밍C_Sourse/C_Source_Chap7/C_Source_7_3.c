//AVR128   UART_3.C
#include<mega128.h>
#include<delay.h>

#define SYSTEM_CLOCK  16000000   // CLOCK (X-tal frequency)  

void putch(char c)//문자 입력
{
 while(!(UCSR0A & 0x20));
 UDR0=c;
}

char getch()
{
 while(!(UCSR0A & 0x80));
 return UDR0;
}

void UART_Init(unsigned int setbauerate)
{
 unsigned long baudrate;
 UCSR0B=0x18; //RXEN0,TXEN0 enable 송.수신
 baudrate =(SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // 보율 값 설정
}

void main()
{
   int key;
  
   DDRA = 0x00; // all Aprot Switchh input  	   

   SREG=0x80; 
   UART_Init(38400); // baudrate 설정

  while(1)
  {
    switch(PINA)
    {
     case 0x7F : putch('1'); delay_ms(300); break;//약 0.3초 마다 PC로 한 문자씩 전송
     case 0xBF : putch('2'); delay_ms(300); break;
     case 0xDF : putch('3'); delay_ms(300); break;
     case 0xEF : putch('4'); delay_ms(300); break;
     case 0xF7 : putch('5'); delay_ms(300); break;
     case 0xFB : putch('6'); delay_ms(300); break;
     case 0xFD : putch('7'); delay_ms(300); break;
     case 0xFE : putch('8'); delay_ms(300); break; 
	}  
  }
}
