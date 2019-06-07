//AVR128  ADC_3.c
//Analog to Digital Converter
//Free Running Mode
//�Ҽ������� ���� ����
#include<mega128.h>

void main()
{
  int i_vin;//������ �Է����� ��� 
  float f_vin;//�Ǽ��� �Է����� ��� 
  char N0,N1;//������ ǥ���ϱ� ���� ���� 
  //N0:������ ���� ǥ��, N1:�Ҽ��� ù°�ڸ� ���� ǥ�� 

  //��Ʈ ���� 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8��Ʈ) + ADCL(8��Ʈ) = 16��Ʈ�� ��� �ϴµ� ������Ʈ���� ���� 10��Ʈ ���
  //ADLAR(ADC Left Adjust Result)=1�̸� ������Ʈ���� ���� 10��Ʈ ��� 
  ADCSRA=0xE7;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADIE(ADC Interrupt Enable)=1, ADPS2-0(ADC Prescaler Bits)=111->128����
 
  while(1)
  {
    i_vin = (int)ADCL;//0���� ���� 255���� 
    i_vin += (int)ADCH<<8;//ADCL(0-255)�� �������Ͱ��� ADCH(0,256,512,768)�������� ���� 
    //���� ����� i_vin������ 1023���� ���� �ȴ�.
   
    f_vin=(float)i_vin*5.0/1023;//0(0V),1(0.0048875V), ~ ,1023(5V)
    //���� i_vin ����(ADC)�� ���� 1023�̸� �Է� ������ 5V�� �ȴ�.
    N0=(int)((f_vin*10.0)+0.5)/10;//0.0 ~ 5.0 ������ �����θ� ����(offset error ����) 
    //�Ҽ������ϸ� ���Ϸ��� �ϸ� ?
    //���� �Է� ���� f_vin= 1.2V��� �ϸ� 1.2���� �Ҽ��θ� ���ַ��� 10�� 
    //���ؼ� 12���ȴ�. �ٽ� 12 % 10 ���� ����ϸ� 2�� �Ǵµ� �Ҽ��� ù°�ڸ��� ���ϸ� �ȴ�.
    N1=(int)((f_vin*10.0)+0.5)%10;//�Ҽ��� ù°�ڸ�
    PORTC=~N0;//������ ǥ��(��ũ���� LED ON)
    PORTA=~N1;//�Ҽ��� ù°�ڸ� ǥ��(��ũ���� LED ON)  
  }
  
}


//���� �Է� ���� f_vin= 1.23V��� �ϸ� 1.23���� �Ҽ��θ� ���ַ��� 100�� 
   //���ؼ� 123�̵ȴ�. �ٽ� 123 % 100���� ����ϸ� 23�� �Ǵµ� �Ҽ��� ù°�ڸ��� ���ϱ� ���ؼ���
   // 23�� 23/10 �ϸ� 2, �Ҽ��� ��°�ڸ��� ���ϱ� ���ؼ��� 23%10�� ����ϸ� 3�̵ȴ�.

//������� �Ҽ��� ��°�ڸ��� �Ҽ��� ��°�ڸ� ���� ǥ�õǵ��� ���α׷� �Ͻÿ�.