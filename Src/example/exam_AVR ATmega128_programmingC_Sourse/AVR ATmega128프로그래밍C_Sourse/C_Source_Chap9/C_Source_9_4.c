//AVR128  Servo_Motor1.c
#include<mega128.h>
#include<delay.h>

void main()
{
  unsigned int i=0;
  
  DDRB=0xFF;//B_PORT OUT
 
  while(1)//무한반복
  {
   //time_base 10ms(0.5ms + 9.5ms)
   for(i=0;i<100;i++)//0도
     {
      PORTB=0xFF;
      delay_us(500);
      PORTB=0x00;
      delay_us(9500);
      }
     
     //time_base 10ms(2.5ms + 7.5ms)
     for(i=0;i<100;i++)//180도
     {
      PORTB=0xFF;
      delay_us(2500);
      PORTB=0x00;
      delay_us(7500);
      } 
   }
 }


