//AVR128  Timer_1
//Timer/Counter 0 Normal Mode
#include<mega128.h>
int sec=0;//�� ������ ����� ���� ����
void main()
{
  DDRA=0xff;//A_PORT OUT
  PORTA=0xff;//�ʱ� �� A_PORT LED OFF 
  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��

  while(1);
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 1�ʸ� ������� 0.016384 X sec = 1
 // sec = 1/0.016384 -> �� 61.035 
 sec++;
 if(sec>=31){PORTA=PORTA^0xFF;sec=0;}//�� 0.5�� ���� LED ON/OFF ����
 //if(sec>=61){PORTA=PORTA^0xFF;sec=0;}//�� 1�� ���� LED ON/OFF ����
}