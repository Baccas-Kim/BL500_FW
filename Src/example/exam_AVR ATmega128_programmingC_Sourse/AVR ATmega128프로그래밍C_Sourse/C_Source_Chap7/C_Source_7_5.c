//AVR128   UART_4.C
#include<mega128.h>
#include<stdio.h>//표준 입.출력함수를 사용하기 위해서 추가 한다.

#define SYSTEM_CLOCK  8000000   // CLOCK (X-tal frequency)  

void UART_Init(unsigned int setbauerate)
{
 unsigned long baudrate;
 UCSR0B=0x18; //RXEN0,TXEN0 enable 송.수신
 baudrate =(SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // 보율 값 설정
}

void main()
{
 
  DDRC=0xFF; //C포트 출력
  
  EICRA=0b00001010;//외부 인터럽트PD2(INT2), PD1(INT1), PD0(INT0)하강 에지
  EIMSK=0b00000011;//2개의 외부 인터럽트 인에이블            
  SREG=0x80;  
  
  UART_Init(38400); // baudrate 설정
  PORTC=0xFF;
 
  while(1)
  {
   	switch(getchar())
    {
     case '1' : puts("PC key1 : LED1_ON\r\n");//1번 LED ON
                PORTC=0x7F; break;
     case '2' : puts("PC key2 : LED2_ON\r\n");
                PORTC=0xBF; break;
     case '3' : puts("PC key3 : LED3_ON\r\n");
                PORTC=0xDF; break;
     case '4' : puts("PC key4 : LED4_ON\r\n");
                PORTC=0xEF; break;
     case '5' : puts("PC key5 : LED5_ON\r\n");
                PORTC=0xF7; break;
     case '6' : puts("PC key6 : LED6_ON\r\n");
                PORTC=0xFB; break;
     case '7' : puts("PC key7 : LED7_ON\r\n");
                PORTC=0xFD; break;
     case '8' : puts("PC key8 : LED8_ON\r\n"); 
                PORTC=0xFE; break; 
	}   
  }
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
 PORTC=0xFF; printf("MCU SW0->LED All OFF\r\n");//LED OFF
}

//외부 인터럽트 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
  PORTC=0x00; printf("MCU SW1->LED All ON\r\n");//LED ON
}
