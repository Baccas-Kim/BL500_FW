//AVR128 LED2_shift
#include<mega128.h>
#include<delay.h>  

void main()
{
 unsigned char LED_data[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 char i; 
 
 DDRA=0xFF;//A��Ʈ  ���   
 
 while(1)//���� �ݺ�                                                       
 {
   for(i=0;i<8;i++)
   {
    PORTA=~LED_data[i];//��ũ���� �̹Ƿ� ����
    delay_ms(300);
   }
 }  
}
