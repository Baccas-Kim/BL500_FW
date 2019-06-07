//AVR128  interrupt_3
//10진 카운터
#include<mega128.h>
#include<delay.h>
int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
//FND표시   { 0  ,  1 , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  }
int cnt=0;//10진수를 표시하기 위한 변수
void main()
{
  DDRA=0xff;//A_PORT OUT
  DDRD=0x00;//D_PORT IN
  PORTA=0xff;//초기 값 A_PORT LED OFF
 
 //인터럽트 초기화
  EICRA=0b00000010;//외부 인터럽트 0 : 하강 에지
  EIMSK=0b00000001;//외부 인터럽트 0 : 인에이블
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋
  while(1)
  {   
   PORTA=FND[cnt];//FND(+com) Sink_current   
   if(cnt>9)cnt=0; 
  }
  
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  delay_ms(10);
  if(PIND.0==0)cnt++;
  if(PIND.0==1)cnt=cnt; 
  delay_ms(10);
}
