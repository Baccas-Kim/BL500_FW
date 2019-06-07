//AVR128  Dot matrix 8X8 Red
#include<mega128.h>
#include<delay.h>

unsigned char key;//SW 입력 값에 따른 제어 

//1상 제어
void Step1(unsigned char dt);//dt -> 속도변수
//2상 제어
void Step2(unsigned char dt);
//1_2상 제어
void Step1_2(unsigned char dt);




void main()
{
  DDRA=0x00;//A포트 입력 
  DDRC=0xFF;//C포트 출력 
  
  //인터럽트 초기화
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//타이머 카운터 초기 값
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋 
  
  PORTA=0xFF;//풀업저항 대신 A포트의 각 비트 값을  1로 한다.  
  
  //무한 반복
  while(1)
  {
    switch(key)
    {
      case 0xfe : Step1(10); break;//1상 제어
      case 0xfd : Step2(10); break;//2상 제어
      case 0xfb : Step1_2(10); break;//1_2상 제어
    }
  }
  
}

//타이머/카운터 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 if(PINA!=0xFF){key=PINA;PORTA=key;}
}

//1상 제어
void Step1(unsigned char dt)
{
  PORTC= 0x08;
  delay_ms(dt);
  PORTC= 0x04;
  delay_ms(dt);
  PORTC= 0x02;
  delay_ms(dt); 
  PORTC= 0x01;
  delay_ms(dt);
}

//2상 제어
void Step2(unsigned char dt)
{
  PORTC= 0x0c;
  delay_ms(dt);
  PORTC= 0x06;
  delay_ms(dt);
  PORTC= 0x03;
  delay_ms(dt); 
  PORTC= 0x09;
  delay_ms(dt);
}

//1_2상 제어
void Step1_2(unsigned char dt)
{
  PORTC= 0x08;//1상
  delay_ms(dt);
  PORTC= 0x0c;//2상
  delay_ms(dt);
  PORTC= 0x04;//1상
  delay_ms(dt);
  PORTC= 0x06;//2상
  delay_ms(dt);
  PORTC= 0x02;//1상
  delay_ms(dt);
  PORTC= 0x03;//2상
  delay_ms(dt); 
  PORTC= 0x01;//1상
  delay_ms(dt);
  PORTC= 0x09;//2상
  delay_ms(dt);
}
