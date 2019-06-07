//AVR128  Dot matrix 8X8 Red
#include<mega128.h>
#include<delay.h>

unsigned char key;//SW �Է� ���� ���� ���� 

//1�� ����
void Step1(unsigned char dt);//dt -> �ӵ�����
//2�� ����
void Step2(unsigned char dt);
//1_2�� ����
void Step1_2(unsigned char dt);




void main()
{
  DDRA=0x00;//A��Ʈ �Է� 
  DDRC=0xFF;//C��Ʈ ��� 
  
  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ �� 
  
  PORTA=0xFF;//Ǯ������ ��� A��Ʈ�� �� ��Ʈ ����  1�� �Ѵ�.  
  
  //���� �ݺ�
  while(1)
  {
    switch(key)
    {
      case 0xfe : Step1(10); break;//1�� ����
      case 0xfd : Step2(10); break;//2�� ����
      case 0xfb : Step1_2(10); break;//1_2�� ����
    }
  }
  
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 if(PINA!=0xFF){key=PINA;PORTA=key;}
}

//1�� ����
void Step1(unsigned char dt)
{
  PORTC= 0x08;
  delay_ms(dt);
  PORTC= 0x04;
  delay_ms(dt);
  PORTC= 0x02;
  delay_ms(dt); 
  PORTC= 0x01;
  delay_ms(dt);
}

//2�� ����
void Step2(unsigned char dt)
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

//1_2�� ����
void Step1_2(unsigned char dt)
{
  PORTC= 0x08;//1��
  delay_ms(dt);
  PORTC= 0x0c;//2��
  delay_ms(dt);
  PORTC= 0x04;//1��
  delay_ms(dt);
  PORTC= 0x06;//2��
  delay_ms(dt);
  PORTC= 0x02;//1��
  delay_ms(dt);
  PORTC= 0x03;//2��
  delay_ms(dt); 
  PORTC= 0x01;//1��
  delay_ms(dt);
  PORTC= 0x09;//2��
  delay_ms(dt);
}
