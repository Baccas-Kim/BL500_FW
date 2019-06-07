//AVR128 Servo_Motor2.c
#include<mega128.h>

unsigned char key;
void main()
{
  DDRB=0xff;//B_PORT OUT
   
  TCCR1A=0xAA;//8��Ʈ ��������
  TCCR1B=0x1A;//8��Ʈ ��������  
  
  ICR1=10000;//16��Ʈ �������� �������ʹ� 50Hz-400Hz������ ���ļ����� ���  8000(400Hz)���� 40000(50Hz)����
  
  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  
  while(1)
  {
    switch(key)
    {
     case 0xFE : OCR1A=1000; break;//0��
     case 0xFD : OCR1A=1500; break;//90��
     case 0xFB : OCR1A=2000; break;//180��
    }
  }
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 if(PIND!=0xFF)key=PIND;
}