//AVR128   LED1_OnOff 
#include<mega128.h>
#include<delay.h>  
void main()
{ 
 //A포트  출력
 DDRA=0xff; 
 while(1)
 {
  PORTA=0xFF;
  delay_ms(500);
  PORTA=0x00;
  delay_ms(500);          
 }  
}
