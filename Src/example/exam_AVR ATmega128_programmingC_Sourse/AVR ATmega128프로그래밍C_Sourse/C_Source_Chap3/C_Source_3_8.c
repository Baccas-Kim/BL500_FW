//AVR128   keypad_LED
#include<mega128.h>
#include<delay.h>

void main()
{
  DDRA=0xFF;//A포트 출력 
  DDRC=0xFF;//C포트 출력 
 
  //D포트 비트단위 출력
  DDRD.7=1;
  DDRD.6=1;  
  DDRD.5=1;
  DDRD.4=1;
  //D포트 비트단위 입력
  DDRD.3=0;
  DDRD.2=0;
  DDRD.1=0;
  DDRD.0=0; 
  
  PORTD.7=1;PORTD.6=1;PORTD.5=1;PORTD.4=0;//1110 출력
      
  //무한 반복
  while(1)
  {
   PORTA.0=PIND.0;//D포트의 최하위 비트 입력 값을 A포트로 출력
   PORTA.1=PIND.1;
   PORTA.2=PIND.2;
   PORTA.3=PIND.3;
  }
}
