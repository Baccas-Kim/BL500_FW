//AVR128 Servo_Motor3.c
#include<mega128.h>
#include<delay.h>

void main()
{
  DDRB=0xff;//B_PORT OUT
   
  TCCR1A=0xAA;//CTC 모드 OCnx핀 토글
  TCCR1B=0x1A;//8비트 레지스터  
 
  TCNT1=0x0000;
  ICR1=40000;//16비트 레지스터 서보모터는 50Hz-400Hz범위의 주파수에서  사용  8000(400Hz)에서 40000(50Hz)사이
  
  while(1)
  {
   OCR1AH=0b00000011; OCR1AL=0b11101000; //10진수 1000
   OCR1BH=0b00000101; OCR1BL=0b11011100; //10진수 1500

   //C레지스터는 헤더파일에서 상위8비트와 하위8비트로 나누어져있어서 
   //16비트로 제어 하려고 하면 에러가 발생한다.
   OCR1CH=0b00000111; OCR1CL=0b11010000; //10진수2000   
  }
}

