//AVR128 LED2_SW
#include<mega128.h>
#include<delay.h>  

void main()
{
 char key; 
 
 DDRA=0xFF;//A��Ʈ  ���   
 DDRE=0x00;//E��Ʈ  �Է�   
 
 PORTA=0xFF;//�ʱⰪ LED_All_OFF-> ��ũ���� 
 while(1)//���� �ݺ�                                                       
 {
  if(PINE!=0xFF)key=PINE;//PUSH SW�� ��������  ����
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
