//AVR128   UART_4.C
#include<mega128.h>

#define SYSTEM_CLOCK  16000000 // CLOCK (X-tal frequency) 
 
unsigned char *text_data,text_cnt;

unsigned char Ryu[]="\n\r Ryu Gi Ju    \r\n";
unsigned char Lim[]="\n\r Lim Yang Koo \r\n";
unsigned char Seo[]="\n\r Seo Hae Jun  \r\n";

void UART_Init(unsigned int setbauerate)
{
 unsigned long baudrate;
 UCSR0B=0xD8; // RXEN0,TXEN0 enable 송.수신
 baudrate =(SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // 보율 값 설정
}


interrupt[USART0_TXC] void UART_T(void)//송신완료 인터럽트
{
 text_data++;//송신 인터럽트가 발생 할 때마다 증가   
 if(--text_cnt)UDR0=*text_data;//문자 열이 종료 될때까지 실행
}

void UART_Send(unsigned char *buf,unsigned char size)
{
  if(!text_cnt)//문자열이 다 입력이 될때까지 실행
  {
   text_data=buf;//데이터가 저장되어있는 시작 주소를 저장 
   text_cnt=size;
   UDR0=*buf;
  }
}

void main()
{
 
  DDRD = 0x00; 	   
  
  EICRA=0b00101010;//외부 인터럽트PD2(INT2), PD1(INT1), PD0(INT0)하강 에지
  EIMSK=0b00000111;//3개의 외부 인터럽트 인에이블            
  SREG=0x80; 
  
  UART_Init(38400); // baud rate 설정

  while(1);
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  UART_Send(Ryu,20);
}

//외부 인터럽트 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
  UART_Send(Lim,20);
}

//외부 인터럽트 2
interrupt[EXT_INT2] void PD0_int2(void)
{
  UART_Send(Seo,20);
}

