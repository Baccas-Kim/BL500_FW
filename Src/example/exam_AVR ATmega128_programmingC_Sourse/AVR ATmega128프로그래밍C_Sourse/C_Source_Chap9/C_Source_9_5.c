//AVR128 Servo_Motor2.c
#include<mega128.h>

unsigned char key;
void main()
{
  DDRB=0xff;//B_PORT OUT
   
  TCCR1A=0xAA;//8비트 레지스터
  TCCR1B=0x1A;//8비트 레지스터  
  
  ICR1=10000;//16비트 레지스터 서보모터는 50Hz-400Hz범위의 주파수에서 사용  8000(400Hz)에서 40000(50Hz)사이
  
  //인터럽트 초기화
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//타이머 카운터 초기 값
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋
  
  while(1)
  {
    switch(key)
    {
     case 0xFE : OCR1A=1000; break;//0도
     case 0xFD : OCR1A=1500; break;//90도
     case 0xFB : OCR1A=2000; break;//180도
    }
  }
}

//타이머/카운터 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 if(PIND!=0xFF)key=PIND;
}