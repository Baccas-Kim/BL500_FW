//AVR128   FND_60mod
#include<mega128.h>
#include<delay.h>
void main()
{
  int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
            //{0,1,2,3,4,5,6,7,8,9}
  int cnt=0,N2,N1;          
  
  DDRA=0xFF;//A포트 출력 
  DDRC=0xFF;//C포트 출력 
  
  //무한 반복
  while(1)
  {
    N2=cnt/10;
    N1=cnt%10;
    PORTC=FND[N2];//싱크전류
    PORTA=FND[N1];//싱크전류
    delay_ms(300); 
    cnt++;
    if(cnt>59)cnt=0;
   }
}
