//AVR128  Timer_4.c
#include<mega128.h>
#include<delay.h>
int dt=300;//FND�� ���ۼӵ��� ���� ����
void main()
{
  DDRA=0xff;//A_PORT OUT  
  DDRC=0xff;//C_PORT OUT     
  DDRD=0x00;//D_PORT IN
   
  PORTA=0xff;//�ʱ� �� FND OFF  
  PORTC=0xff;//�ʱ� �� FND OFF  
  PORTD=0xff;//�ʱ� �� PULL UP 
  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  //Dynamic ����
  while(1)
  {
   PORTC=0x08;PORTA=0x06;delay_ms(dt);//1
   PORTC=0x04;PORTA=0x5b;delay_ms(dt);//2
   PORTC=0x02;PORTA=0x4f;delay_ms(dt);//3
   PORTC=0x01;PORTA=0x66;delay_ms(dt);//4
  }
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms ���� �Է��� �޴´�.
 if(PIND==0xFE)dt=5;
 if(PIND==0xFD)dt=300;
}