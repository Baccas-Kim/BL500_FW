//AVR128   keypad_LED
#include<mega128.h>
#include<delay.h>

void main()
{
  DDRA=0xFF;//A��Ʈ ��� 
  DDRC=0xFF;//C��Ʈ ��� 
 
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
  
  PORTD.7=1;PORTD.6=1;PORTD.5=1;PORTD.4=0;//1110 ���
      
  //���� �ݺ�
  while(1)
  {
   PORTA.0=PIND.0;//D��Ʈ�� ������ ��Ʈ �Է� ���� A��Ʈ�� ���
   PORTA.1=PIND.1;
   PORTA.2=PIND.2;
   PORTA.3=PIND.3;
  }
}
