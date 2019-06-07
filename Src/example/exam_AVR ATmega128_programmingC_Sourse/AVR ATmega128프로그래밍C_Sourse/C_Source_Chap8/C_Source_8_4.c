//AVR128  Dot matrix8X8_heart.c

#include<mega128.h>
#include<delay.h>
void main()
{
  int  Sel[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; 
  int heart[8]={0x66,0x99,0x99,0x81,0x81,0x42,0x24,0x18};
         
  int i,j;          
  
  DDRA=0xFF;//A포트 출력                        
  DDRC=0xFF;//C포트 출력  
  
   //무한 반복
  while(1)
  {
     for(i=0;i<150;i++)//loop
     { 
       //Ryu
       for(j=0;j<8;j++)
       {
        PORTC=Sel[j];
        PORTA=~heart[j];
        delay_ms(1);//값을 변경해 가면서 동작원리를 이해 
        //PORTA=0x00;//Display_OFF
        //PORTC=0x00;//Display_OFF         
       } 
      } 
  }
 
}
