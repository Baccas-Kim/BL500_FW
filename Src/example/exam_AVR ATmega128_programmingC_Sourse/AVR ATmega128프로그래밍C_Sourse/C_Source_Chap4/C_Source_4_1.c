//AVR128  interrupt_1
#include<mega128.h>
bit flag=0;//switch������ ���� ���� ����
void main()
{
  DDRA=0xff;//A_PORT OUT  
  DDRD=0x00;//D_PORT IN
  PORTA=0xff;//�ʱ� �� PORTA LED OFF
  //PORTD=0xff;//��Ʈ D�� ��°� 5V���� 
  //�ܺο��� 10k�� Pull Up ������ ������ ��쿡�� �ʿ� ����   
  //���ͷ�Ʈ �ʱ�ȭ
  EICRA=0b00001010;//�ܺ� ���ͷ�Ʈ PD1, PD0 : �ϰ� ����
  EIMSK=0b00000011;//�ܺ� ���ͷ�Ʈ PD1, PD0 : �ο��̺�
  SREG=0x80;//     //���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  
  while(1)
  {
   if(flag==1)PORTA=0x00;//sink current LED_ON
   else PORTA=0xFF;
  }
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  flag=0;//LED_OFF 
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD1_int1(void)
{
  flag=1;//LED_ON 
}