//AVR128  DC_Motor1.c
#include<mega128.h>

void main()
{
  DDRA=0x00;//A_PORT IN 
  DDRB=0xFF;//B_PORT OUT
  
  PORTB=0xFF;//Motor On
}


