//AVR128  interrupt_2.c
#include<mega128.h>

void main()
{
  DDRA=0xFF;//A��Ʈ ���
  DDRD=0b11110000;//D_PORT High 4bit OUT Low 4bit IN
  DDRE=0b00001111;//D_PORT High 4bit IN Low 4bit OUT
  
  //���ͷ�Ʈ �ʱ�ȭ
  //�ܺ� ���ͷ�Ʈ PD3(INT3), PD2(INT2), PD1(INT1), PD0(INT0)
  EICRA=0b10101010;//�ϰ� ����
  
  //�ܺ� ���ͷ�Ʈ PE7(INT7), PE6(INT6), PE5(INT5), PE4(INT4)
  EICRB=0b10101010;//�ϰ� ���� 
  
  EIMSK=0b11111111;//8���� �ܺ� ���ͷ�Ʈ �ο��̺�
  SREG=0x80;//     //���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  
  PORTA=0xFF;//�ʱ� �� -> LED OFF
  while(1);
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  PORTA=0xfe;//��ũ������ ���� �ϹǷ� ������ ��Ʈ(PA0) LED ON
}

//�ܺ� ���ͷ�Ʈ 1
interrupt[EXT_INT1] void PD1_int1(void)
{
  PORTA=0xfd;//��ũ������ ���� �ϹǷ� ��Ʈ(PA1) LED ON
}

//�ܺ� ���ͷ�Ʈ 2 
interrupt[EXT_INT2] void PD2_int2(void)
{
  PORTA=0xfb;//��ũ������ ���� �ϹǷ� ��Ʈ(PA2) LED ON
}

//�ܺ� ���ͷ�Ʈ 3 
interrupt[EXT_INT3] void PD3_int3(void)
{
  PORTA=0xf7;//��ũ������ ���� �ϹǷ� ��Ʈ(PA3) LED ON
}

//�ܺ� ���ͷ�Ʈ 4 
interrupt[EXT_INT4] void PE4_int4(void)
{
  PORTA=0xef;//��ũ������ ���� �ϹǷ� ��Ʈ(PA4) LED ON
}

//�ܺ� ���ͷ�Ʈ 5 
interrupt[EXT_INT5] void PE5_int5(void)
{
  PORTA=0xdf;//��ũ������ ���� �ϹǷ� ��Ʈ(PA5) LED ON
}

//�ܺ� ���ͷ�Ʈ 6 
interrupt[EXT_INT6] void PE6_int6(void)
{
  PORTA=0xbf;//��ũ������ ���� �ϹǷ� ��Ʈ(PA6) LED ON
}

//�ܺ� ���ͷ�Ʈ 7 
interrupt[EXT_INT7] void PE7_int7(void)
{
  PORTA=0x7f;//��ũ������ ���� �ϹǷ� �ֻ��� ��Ʈ(PA7) LED ON
}

