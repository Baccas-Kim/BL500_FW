//AVR128  ADC_5.c
//Analog to Digital Converter, FND Display
//Free Running Mode, ADCW Register, Interrupt
//�Ҽ������� ���� ����
#include<mega128.h>

int i_vin;//������ �Է����� ��� 
float f_vin;//�Ǽ��� �Է����� ��� 

void main()
{
  int cnt[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
            //{0,1,2,3,4,5,6,7,8,9} FND Display Data
 
  char N0,N1;//������ ǥ���ϱ� ���� ���� 
  //N0:������ ���� ǥ��, N1:�Ҽ��� ù°�ڸ� ���� ǥ�� 

  //��Ʈ ���� 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8��Ʈ) + ADCL(8��Ʈ) = 16��Ʈ�� ��� �ϴµ� ������Ʈ���� ���� 10��Ʈ ���
  //ADLAR(ADC Left Adjust Result)=1�̸� ������Ʈ���� ���� 10��Ʈ ��� 
  ADCSRA=0xEF;//ADC Control and Atatus Register A
  //ADEN(ADC Enable)=1, ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADIE(ADC Interrupt Enable)=1, ADPS2-0(ADC Prescaler Bits)=111->128����
  
  SREG=0x80;//���� ���ͷ�Ʈ ����
  
  
  while(1)
  {
   
    N0=(int)((f_vin*10.0)+0.5)/10;//0.0 ~ 5.0 ������ �����θ� ����(offset error ����) 
    //�Ҽ������ϸ� ���Ϸ��� �ϸ� ?
    //���� �Է� ���� f_vin= 1.2V��� �ϸ� 1.2���� �Ҽ��θ� ���ַ��� 10�� 
    //���ؼ� 12���ȴ�. �ٽ� 12 % 10 ���� ����ϸ� 2�� �Ǵµ� �Ҽ��� ù°�ڸ��� ���ϸ� �ȴ�.
    N1=(int)((f_vin*10.0)+0.5)%10;//�Ҽ��� ù°�ڸ�
    PORTC=cnt[N0];//������ ǥ��(��ũ���� LED ON)
    PORTA=cnt[N1];//�Ҽ��� ù°�ڸ� ǥ��(��ũ���� LED ON)  
  }
  
}

//ADC ���ͷ�Ʈ 
interrupt[ADC_INT] void adc_isr(void)
{
 i_vin=(int)ADCW;//ADCW(ADC Word)�� 16��Ʈ �������ͷ� ADC���� �����ϰ� ó�� �Ҽ��ִ�. 
 f_vin=(float)i_vin*5.0/1023;//0(0V),1(0.0048875V), ~ ,1023(5V)
 //���� i_vin ����(ADC)�� ���� 1023�̸� �Է� ������ 5V�� �ȴ�.
}
//���� �Է� ���� f_vin= 1.23V��� �ϸ� 1.23���� �Ҽ��θ� ���ַ��� 100�� 
   //���ؼ� 123�̵ȴ�. �ٽ� 123 % 100���� ����ϸ� 23�� �Ǵµ� �Ҽ��� ù°�ڸ��� ���ϱ� ���ؼ���
   // 23�� 23/10 �ϸ� 2, �Ҽ��� ��°�ڸ��� ���ϱ� ���ؼ��� 23%10�� ����ϸ� 3�̵ȴ�.

//������� �Ҽ��� ��°�ڸ��� �Ҽ��� ��°�ڸ� ���� ǥ�õǵ��� ���α׷� �Ͻÿ�.