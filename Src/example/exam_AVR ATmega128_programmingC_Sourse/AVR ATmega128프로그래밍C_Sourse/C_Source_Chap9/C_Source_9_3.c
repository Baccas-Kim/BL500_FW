//AVR128  DC_Motor3.c
#include<mega128.h>

void main()
{
  DDRA=0x00;//A_PORT IN 
  DDRC=0xFF;//C_PORT OUT
  
  PORTA=0xFF;//내부 풀업    
  //PORTC=0x0A;//0000 1010 CW
  PORTC=0x05;//0000 0101 CCW
 }


