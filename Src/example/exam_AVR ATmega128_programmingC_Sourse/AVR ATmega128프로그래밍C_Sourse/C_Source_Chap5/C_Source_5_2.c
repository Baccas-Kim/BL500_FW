//AVR128  Timer_2.c
//Timer/Counter 0, 2 Nomal Mode
#include<mega128.h>
int tcnt0=0,tcnt2=0;//타이머 설정을 위한 변수
void main()
{
  //포트 설정 
  DDRA=0xff;//A_PORT OUT 
  DDRC=0xff;//C_PORT OUT 
  
  PORTA=0xff;//초기 값 A_PORT LED OFF
  PORTC=0xff;//초기 값 C_PORT LED OFF

  //인터럽트 초기화
  TIMSK=0x41;//Timer/Counter Mask Register(01000001:TOIE2=1,TOIE0=1)
  //OCIE : Output Compare Interrupt Enable
  TCCR0=0x07;//Timer/Counter 0 Control Register(WGM01=0,WGM00=0,CS02=1,CS01=1,CS00=1) 
  TCCR2=0x05;//(WGM21=0,WGM20=0,CS22=1,CS21=0,CS20=1) 
  //WGM : Waveform Generation Mode, CS : clock select  
   
  TCNT0=0x00;//타이머/카운터 0 초기 값   
  TCNT2=0x00;//타이머/카운터 2 초기 값
  
  SREG=0x80;//상태레지스터의 최상위 비트를 1로 주어 전역 인터럽트 인에이블 비트 셋
 
  while(1);
}

//타이머/카운터 0 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 0.5초를 만들려면 0.016384 X sec = 0.5
 // tcnt0 = 0.5/0.016384 -> 약 30.517 
 
 tcnt0++;//0.016384초 
 if(tcnt0>=31)//약 0.5초 
 {
  tcnt0=0;//1초 마다 시간 값 갱신 
  PORTA=PORTA^0xFF;//LED ON/OFF 동작 
 } 
}

//타이머/카운터 2 인터럽트 
interrupt[TIM2_OVF] void Timer2(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 1초를 만들려면 0.016384 X sec = 1
 // sec = 1/0.016384 -> 약 61.035 
 
 tcnt2++;//0.016384초 
 if(tcnt2>=61)//약 1초 
 {
  tcnt2=0;//1초 마다 시간 값 갱신 
  PORTC=PORTC^0xFF;//LED ON/OFF 동작 
 } 
}