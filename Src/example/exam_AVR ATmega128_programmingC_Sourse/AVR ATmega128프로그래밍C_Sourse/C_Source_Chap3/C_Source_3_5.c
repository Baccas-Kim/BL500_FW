//AVR128   FND_10mod
#include<mega128.h>
#include<delay.h>
void main()
{
  int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
            //{0,1,2,3,4,5,6,7,8,9}
  int i;          
  
  DDRA=0xFF;//A포트 출력 
  
  //무한 반복
  while(1)
  {
    for(i=0;i<10;i++)
    {
      PORTA=FND[i];//싱크전류
      delay_ms(100); 
    }
  }
}
