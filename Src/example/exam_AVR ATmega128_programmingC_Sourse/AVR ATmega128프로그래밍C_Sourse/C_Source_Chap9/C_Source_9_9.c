//AVR128  Dot matrix 8X8 Red
#include<mega128.h>
#include<delay.h>

void Step_For(unsigned char dt);
void Step_Rev(unsigned char dt);
unsigned char key;

void main()
{
  DDRD=0x00;//D��Ʈ �Է�  
  DDRA=0xFA;//A��Ʈ ��� 
  DDRC=0xFF;//C��Ʈ ��� 
   
  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  
  //���� �ݺ�
  while(1)
  {
    switch(key)
    {
      case 0xfe : key=key; break;//1111 1110
      case 0xfd : Step_For(10); break;//1111 1101
      case 0xfb : Step_Rev(10); break;//1111 1011
    }
  }
  
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 if(PIND!=0xFF){key=PIND;PORTA=key;} 
 //PORTA=key;�� �ԷµǴ� key���� �˱� ����
}
  
  
void Step_For(unsigned char dt)//max=255
{
  PORTC= 0x0c;
  delay_ms(dt);
  PORTC= 0x06;
  delay_ms(dt);
  PORTC= 0x03;
  delay_ms(dt); 
  PORTC= 0x09;
  delay_ms(dt);
}

void Step_Rev(unsigned char dt)//max=255
{
  PORTC= 0x09;
  delay_ms(dt);
  PORTC= 0x03;
  delay_ms(dt);
  PORTC= 0x06;
  delay_ms(dt); 
  PORTC= 0x0c;
  delay_ms(dt);
}

