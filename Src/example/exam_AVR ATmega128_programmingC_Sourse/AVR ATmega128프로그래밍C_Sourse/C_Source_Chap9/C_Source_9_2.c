//AVR128  DC_Motor2.c
#include<mega128.h>

void main()
{
  DDRA=0x00;//A_PORT IN 
  DDRC=0xFF;//C_PORT OUT
  
  PORTA=0xFF;//���� Ǯ��    
  PORTC=0x01;//CW
  //PORTC=0x02;//CCW
  //PORTC=0x00;//Stop
 }


