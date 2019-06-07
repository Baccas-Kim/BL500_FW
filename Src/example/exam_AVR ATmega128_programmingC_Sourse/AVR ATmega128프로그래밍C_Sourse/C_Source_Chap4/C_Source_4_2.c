//AVR128  interrupt_2.c
#include<mega128.h>

void main()
{
  DDRA=0xFF;//A포트 출력
  DDRD=0b11110000;//D_PORT High 4bit OUT Low 4bit IN
  DDRE=0b00001111;//D_PORT High 4bit IN Low 4bit OUT
  
  //인터럽트 초기화
  //외부 인터럽트 PD3(INT3), PD2(INT2), PD1(INT1), PD0(INT0)
  EICRA=0b10101010;//하강 에지
  
  //외부 인터럽트 PE7(INT7), PE6(INT6), PE5(INT5), PE4(INT4)
  EICRB=0b10101010;//하강 에지 
  
  EIMSK=0b11111111;//8개의 외부 인터럽트 인에이블
  SREG=0x80;//     //전역 인터럽트 인에이블 비트 셋
  
  PORTA=0xFF;//초기 값 -> LED OFF
  while(1);
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  PORTA=0xfe;//싱크전류로 동작 하므로 최하위 비트(PA0) LED ON
}

//외부 인터럽트 1
interrupt[EXT_INT1] void PD1_int1(void)
{
  PORTA=0xfd;//싱크전류로 동작 하므로 비트(PA1) LED ON
}

//외부 인터럽트 2 
interrupt[EXT_INT2] void PD2_int2(void)
{
  PORTA=0xfb;//싱크전류로 동작 하므로 비트(PA2) LED ON
}

//외부 인터럽트 3 
interrupt[EXT_INT3] void PD3_int3(void)
{
  PORTA=0xf7;//싱크전류로 동작 하므로 비트(PA3) LED ON
}

//외부 인터럽트 4 
interrupt[EXT_INT4] void PE4_int4(void)
{
  PORTA=0xef;//싱크전류로 동작 하므로 비트(PA4) LED ON
}

//외부 인터럽트 5 
interrupt[EXT_INT5] void PE5_int5(void)
{
  PORTA=0xdf;//싱크전류로 동작 하므로 비트(PA5) LED ON
}

//외부 인터럽트 6 
interrupt[EXT_INT6] void PE6_int6(void)
{
  PORTA=0xbf;//싱크전류로 동작 하므로 비트(PA6) LED ON
}

//외부 인터럽트 7 
interrupt[EXT_INT7] void PE7_int7(void)
{
  PORTA=0x7f;//싱크전류로 동작 하므로 최상위 비트(PA7) LED ON
}

