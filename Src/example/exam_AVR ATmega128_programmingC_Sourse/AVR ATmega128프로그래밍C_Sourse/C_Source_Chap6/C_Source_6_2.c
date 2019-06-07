//AVR128  ADC_2.c
//Analog to Digital Converter, LED Display
//Free Running Mode
//������ ���� ����
#include<mega128.h>
#include<delay.h>
void main()
{
  int i_vin;
  
  //��Ʈ ���� 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8��Ʈ) + ADCL(8��Ʈ) = 16��Ʈ�� ��� �ϴµ� ������Ʈ���� ���� 10��Ʈ ���
  //ADLAR(ADC Left Adjust Result)=1�̸� ������Ʈ���� ���� 10��Ʈ ��� 
  ADCSRA=0xE7;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADPS2-0(ADC Prescaler Bits)=128����
  PORTC=~ADCSRA;
  if(ADCSRA==0xE7)delay_ms(3000);//��ȯ�� �Ϸ�Ǹ� ADIF(ADC Interrupt Flag=1)�� 1�� �ȴ�.
  //�ʹ� ���� ������ �Ͽ� ������ Ȯ�� �ϰ��� �� 3�ʰ��� �ð� ������ �־���. 
  //���¹�ư�� ������ �� �Ŀ� �� 3�ʸ� ��ٸ���  ADIF�� ����� LED�� ON�� �Ǵ� ���� Ȯ�� �Ҽ��� �ִ�.  
  
  while(1)
  {
   i_vin = (int)ADCL;//0���� ���� 255���� 
   i_vin += (int)ADCH<<8;//0���� ���� 1023���� 
   //ADCH<<8�� ADCH*256 �� ����.  
   //8��Ʈ ���μ����̹Ƿ� �ѹ��� 8��Ʈ�� ó���ϹǷ� ���� ADCL�������Ϳ� 
   //�����͸� �� ���� ADCL�������Ϳ� ����. 
   PORTC=~ADCSRA;//ADC Control and Atatus Register A�� ���� LED�� Ȯ�� 
   PORTA=~(int)(i_vin/100)>>1;//��ũ������ LED�� �����ϹǷ� ���� ��Ŵ. 
   //���� 5V�� �Է��� �ȴٸ� 1023/10=10���� 10(1010)>>1 �ϸ� 0101���� ���� �� 5V(�ִ� ���� ��) 
   //�ᱹ 10�� ��� 1��Ʈ �̵��ϸ� 2�� �����Ͱ� ����.  
  }
}

//PORTA�� ����� 0V(00000000), 1V(00000001), 2V(00000010), 3V(00000011), 4V(00000100), 5V(00000101)�� �ȴ�.
