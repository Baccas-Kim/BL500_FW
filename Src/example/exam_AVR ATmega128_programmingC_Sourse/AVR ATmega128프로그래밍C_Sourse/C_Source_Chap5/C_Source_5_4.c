//AVR128  Timer_4.c
#include<mega128.h>
#include<delay.h>
int dt=300;//FND의 동작속도를 위한 변수
void main()
{
  DDRA=0xff;//A_PORT OUT  
  DDRC=0xff;//C_PORT OUT     
  DDRD=0x00;//D_PORT IN
   
  PORTA=0xff;//초기 값 FND OFF  
  PORTC=0xff;//초기 값 FND OFF  
  PORTD=0xff;//초기 값 PULL UP 
  //인터럽트 초기화
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//타이머 카운터 초기 값
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋
  //Dynamic 구동
  while(1)
  {
   PORTC=0x08;PORTA=0x06;delay_ms(dt);//1
   PORTC=0x04;PORTA=0x5b;delay_ms(dt);//2
   PORTC=0x02;PORTA=0x4f;delay_ms(dt);//3
   PORTC=0x01;PORTA=0x66;delay_ms(dt);//4
  }
}

//타이머/카운터 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms 마다 입력을 받는다.
 if(PIND==0xFE)dt=5;
 if(PIND==0xFD)dt=300;
}