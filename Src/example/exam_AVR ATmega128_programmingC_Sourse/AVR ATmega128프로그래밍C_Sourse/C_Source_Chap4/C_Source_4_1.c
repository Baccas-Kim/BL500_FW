//AVR128  interrupt_1
#include<mega128.h>
bit flag=0;//switch동작을 위한 변수 선언
void main()
{
  DDRA=0xff;//A_PORT OUT  
  DDRD=0x00;//D_PORT IN
  PORTA=0xff;//초기 값 PORTA LED OFF
  //PORTD=0xff;//포트 D의 출력값 5V유지 
  //외부에서 10k의 Pull Up 저항을 연결할 경우에는 필요 없음   
  //인터럽트 초기화
  EICRA=0b00001010;//외부 인터럽트 PD1, PD0 : 하강 에지
  EIMSK=0b00000011;//외부 인터럽트 PD1, PD0 : 인에이블
  SREG=0x80;//     //전역 인터럽트 인에이블 비트 셋
  
  while(1)
  {
   if(flag==1)PORTA=0x00;//sink current LED_ON
   else PORTA=0xFF;
  }
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  flag=0;//LED_OFF 
}

//외부 인터럽트 1 
interrupt[EXT_INT1] void PD1_int1(void)
{
  flag=1;//LED_ON 
}