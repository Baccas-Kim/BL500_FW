//AVR128  Dot matrix 8X8 Red
#include<mega128.h>
#include<delay.h>
void main()
{
  
  int  Row[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};          
 
  int i;          
  
  DDRA=0xFF;//A��Ʈ ���
  DDRC=0xFF;//C��Ʈ ���  
  PORTC=0x00;//sink Current
  //���� �ݺ�
  while(1)
  {
    for(i=0;i<8;i++)
    {
      PORTC=Row[i]; 
      delay_ms(200);
     }
   } 
    
}
