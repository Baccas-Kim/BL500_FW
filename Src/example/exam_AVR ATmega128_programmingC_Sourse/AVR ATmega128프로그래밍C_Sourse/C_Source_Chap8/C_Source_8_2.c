//AVR128  Dot_mono1.c
#include<mega128.h>
#include<delay.h>
void main()
{
  
 int  Row[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};            
 
  int i=0;          
  
  DDRA=0xFF;//A포트 출력
  DDRC=0xFF;//C포트 출력  
 
  //무한 반복
  while(1)
  {
    PORTC=Row[i]; 
    PORTA=0x00;//Sink Current
    delay_ms(100);  

    i++;
    if(i>7)i=0;
   }     
}
