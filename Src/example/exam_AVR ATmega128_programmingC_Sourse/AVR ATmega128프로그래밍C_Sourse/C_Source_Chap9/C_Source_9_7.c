//AVR128  Step_Motor1.c
#include<mega128.h>
#include<delay.h>

void main()
{
  unsigned char dt=5;//max=255 
  DDRC=0xFF;//C_PORT OUT  
  
  //2상제어 방식
  while(1)//무한반복
  {
   PORTC=0x0C;
   delay_ms(dt);
    
   PORTC=0x06;
   delay_ms(dt);
   
   PORTC=0x03;
   delay_ms(dt);
   
   PORTC=0x09;
   delay_ms(dt);
  }
}


