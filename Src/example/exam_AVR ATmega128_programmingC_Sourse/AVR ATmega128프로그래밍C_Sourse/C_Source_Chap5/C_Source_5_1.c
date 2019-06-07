//AVR128  Timer_1
//Timer/Counter 0 Normal Mode
#include<mega128.h>
int sec=0;//초 단위를 만들기 위한 변수
void main()
{
  DDRA=0xff;//A_PORT OUT
  PORTA=0xff;//초기 값 A_PORT LED OFF 
  //인터럽트 초기화
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//타이머 카운터 초기 값
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋

  while(1);
}

//타이머/카운터 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 1초를 만들려면 0.016384 X sec = 1
 // sec = 1/0.016384 -> 약 61.035 
 sec++;
 if(sec>=31){PORTA=PORTA^0xFF;sec=0;}//약 0.5초 마다 LED ON/OFF 동작
 //if(sec>=61){PORTA=PORTA^0xFF;sec=0;}//약 1초 마다 LED ON/OFF 동작
}