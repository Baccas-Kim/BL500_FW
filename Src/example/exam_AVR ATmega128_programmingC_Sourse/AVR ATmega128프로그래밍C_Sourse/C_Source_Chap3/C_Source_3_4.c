//AVR128 LED2_SW
#include<mega128.h>
#include<delay.h>  

void main()
{
 char key; 
 
 DDRA=0xFF;//A포트  출력   
 DDRE=0x00;//E포트  입력   
 
 PORTA=0xFF;//초기값 LED_All_OFF-> 싱크전류 
 while(1)//무한 반복                                                       
 {
  if(PINE!=0xFF)key=PINE;//PUSH SW를 누를때만  대입
   switch(key)
   {
    case 0xFE : PORTA=key; break;
    case 0xFD : PORTA=key; break;
    case 0xFB : PORTA=key; break;
    case 0xF7 : PORTA=key; break;
    case 0xEF : PORTA=key; break;
    case 0xDF : PORTA=key; break;
    case 0xBF : PORTA=key; break;
    case 0x7F : PORTA=key; break;  
   } 
 }  
}
