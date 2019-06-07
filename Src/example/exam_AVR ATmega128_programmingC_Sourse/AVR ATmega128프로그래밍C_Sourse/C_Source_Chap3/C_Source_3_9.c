//AVR128   keypad_FND 
#include<mega128.h>
#include<delay.h>

void main()
{
  int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
  
  DDRA=0xFF;//A��Ʈ ��� 
  
  //D��Ʈ ��Ʈ���� ���
  DDRD.7=1;
  DDRD.6=1;  
  DDRD.5=1;
  DDRD.4=1;
  //D��Ʈ ��Ʈ���� �Է�
  DDRD.3=0;
  DDRD.2=0;
  DDRD.1=0;
  DDRD.0=0; 
  
  //PORTD.6=0;PORTD.5=1;PORTD.4=1;//011 ��� -> 1, 4, 7, * 
  PORTD.6=1;PORTD.5=0;PORTD.4=1;//101 ��� -> 2, 5, 8, 0 
  //PORTD.6=1;PORTD.5=1;PORTD.4=0;//110 ��� -> 3, 6, 9, # 
                                               
  PORTA=0xFF;//�ʱⰪ FND �ʱ� �� 
     
  //���� �ݺ�
  while(1)
  {
   if(PIND.0==0)PORTA=FND[2];
   if(PIND.1==0)PORTA=FND[5];
   if(PIND.2==0)PORTA=FND[8];
   if(PIND.3==0)PORTA=FND[0];
  }
}
