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
 UCSR0B=0xD8; // RXEN0,TXEN0 enable ��.����
 baudrate =(SYSTEM_CLOCK/setbauerate/16) - 1; 
 UBRR0L = baudrate;    // ���� �� ����
}


interrupt[USART0_TXC] void UART_T(void)//�۽ſϷ� ���ͷ�Ʈ
{
 text_data++;//�۽� ���ͷ�Ʈ�� �߻� �� ������ ����   
 if(--text_cnt)UDR0=*text_data;//���� ���� ���� �ɶ����� ����
}

void UART_Send(unsigned char *buf,unsigned char size)
{
  if(!text_cnt)//���ڿ��� �� �Է��� �ɶ����� ����
  {
   text_data=buf;//�����Ͱ� ����Ǿ��ִ� ���� �ּҸ� ���� 
   text_cnt=size;
   UDR0=*buf;
  }
}

void main()
{
 
  DDRD = 0x00; 	   
  
  EICRA=0b00101010;//�ܺ� ���ͷ�ƮPD2(INT2), PD1(INT1), PD0(INT0)�ϰ� ����
  EIMSK=0b00000111;//3���� �ܺ� ���ͷ�Ʈ �ο��̺�            
  SREG=0x80; 
  
  UART_Init(38400); // baud rate ����

  while(1);
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  UART_Send(Ryu,20);
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
  UART_Send(Lim,20);
}

//�ܺ� ���ͷ�Ʈ 2
interrupt[EXT_INT2] void PD0_int2(void)
{
  UART_Send(Seo,20);
}

