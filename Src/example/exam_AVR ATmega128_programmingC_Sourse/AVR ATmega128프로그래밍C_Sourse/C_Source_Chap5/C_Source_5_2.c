//AVR128  Timer_2.c
//Timer/Counter 0, 2 Nomal Mode
#include<mega128.h>
int tcnt0=0,tcnt2=0;//Ÿ�̸� ������ ���� ����
void main()
{
  //��Ʈ ���� 
  DDRA=0xff;//A_PORT OUT 
  DDRC=0xff;//C_PORT OUT 
  
  PORTA=0xff;//�ʱ� �� A_PORT LED OFF
  PORTC=0xff;//�ʱ� �� C_PORT LED OFF

  //���ͷ�Ʈ �ʱ�ȭ
  TIMSK=0x41;//Timer/Counter Mask Register(01000001:TOIE2=1,TOIE0=1)
  //OCIE : Output Compare Interrupt Enable
  TCCR0=0x07;//Timer/Counter 0 Control Register(WGM01=0,WGM00=0,CS02=1,CS01=1,CS00=1) 
  TCCR2=0x05;//(WGM21=0,WGM20=0,CS22=1,CS21=0,CS20=1) 
  //WGM : Waveform Generation Mode, CS : clock select  
   
  TCNT0=0x00;//Ÿ�̸�/ī���� 0 �ʱ� ��   
  TCNT2=0x00;//Ÿ�̸�/ī���� 2 �ʱ� ��
  
  SREG=0x80;//���·��������� �ֻ��� ��Ʈ�� 1�� �־� ���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
 
  while(1);
}

//Ÿ�̸�/ī���� 0 ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 0.5�ʸ� ������� 0.016384 X sec = 0.5
 // tcnt0 = 0.5/0.016384 -> �� 30.517 
 
 tcnt0++;//0.016384�� 
 if(tcnt0>=31)//�� 0.5�� 
 {
  tcnt0=0;//1�� ���� �ð� �� ���� 
  PORTA=PORTA^0xFF;//LED ON/OFF ���� 
 } 
}

//Ÿ�̸�/ī���� 2 ���ͷ�Ʈ 
interrupt[TIM2_OVF] void Timer2(void)
{
 // 1/16us X 1024 X 256 = 16.384ms
 // 1�ʸ� ������� 0.016384 X sec = 1
 // sec = 1/0.016384 -> �� 61.035 
 
 tcnt2++;//0.016384�� 
 if(tcnt2>=61)//�� 1�� 
 {
  tcnt2=0;//1�� ���� �ð� �� ���� 
  PORTC=PORTC^0xFF;//LED ON/OFF ���� 
 } 
}