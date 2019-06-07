//AVR128  Timer_3.c
#include<mega128.h>

void main()
{
  //포트 설정 
  DDRB=0xff;//B_PORT OUT 
 
  TCCR1A=0xAA;//
  TCCR1B=0x1A;//
  
  ICR1=20000;
  
  while(1)
  {
   OCR1AH=0b00000011;//1ms -> 10진수 1000
   OCR1AL=0b11101000;
    
   OCR1BH=0b00000111;//2ms -> 10진수 2000
   OCR1BL=0b11010000;
  
   OCR1CH=0b00001011;//3ms -> 10진수 3000
   OCR1CL=0b10111000;
  } 
  
}
