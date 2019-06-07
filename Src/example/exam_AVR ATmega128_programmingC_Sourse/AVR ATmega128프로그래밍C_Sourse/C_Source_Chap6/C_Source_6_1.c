//AVR128  ADC_1.c
//Analog to Digital Converter, LED Display
//Single Conversion Mode
#include<mega128.h>

void main()
{
  //��Ʈ ���� 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8��Ʈ) + ADCL(8��Ʈ) = 16��Ʈ�� ��� �ϴµ� ������Ʈ���� ���� 10��Ʈ ���
  //ADLAR(ADC Left Adjust Result)=1�̸� ������Ʈ���� ���� 10��Ʈ ��� 
  ADCSRA=0x87;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADPS2-0(ADC Prescaler Bits)=128����
  
  while(1)
  {
   ADCSRA|=0x40;//ADSC(ADC Start Conversion)=1
   while(ADCSRA & 0x10==0);//ADIF(ADC Interrupt Flag)=1�� �ɶ�����(ADC��ȯ�� ����������)��ٸ���.
   PORTA=~ADCL;//��ũ������ LED�� ���� �ϹǷ� ����(10��Ʈ �� ���� 8��Ʈ ǥ��) 
   PORTC=~ADCH;//��ũ������ LED�� ���� �ϹǷ� ����(10��Ʈ �� ���� 2��Ʈ ǥ��)
  }
}
 //�ҽ� �ڵ� �м�
 //ADC�� 10��Ʈ�� ����� �ϴµ� �ϳ��� �������ͷδ� 8��Ʈ�ۿ� ó���� ����
 //�� �ϹǷ� AHCH�������Ϳ� 8��Ʈ����, ADCL�������Ϳ� 8��Ʈ�� ���� �� 16��Ʈ �߿���
 //ADMUX���������� ADLAR�� ���� 0�� 1�� ���� 10��Ʈ�� �������� 10��Ʈ�� ����Ұ�����
 //���� 10��Ʈ�� ����Ұ������� �����ȴ�.
 //0(0V),1,2,3,~,1022,1023(5V)�� �Ǵµ� �����ϰ� �����θ� ����� �� ����,
 //1023/5=204.6�̹Ƿ� 1V�� �� 204���� 205�� �Ǵµ�
 //�Ҽ����� �����ϰ� ����� �ϸ�, 1V(204), 2V(408), 3V(612), 4v(816), 5v(1020)���� �ȴ�. 
 //���� ADC0(PF0)���� 1V�� �Է��� �ȴٸ� 10���� 204�� �Ǿ�� �ϹǷ� ADCH=00000000�� ����ǰ� 
 //ADCL=11001100�� ����ȴ�. 
//�������  ADMUX ���������� ADLAR(ADC Multuplxer Selection Regisitor)=1 �� �����ϰ�
//PORTC�� PORTA�� LED�� ������ ���鼭 �� 
