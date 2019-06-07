//AVR128  ADC_7.c
//Analog to Digital Converter, FND Display
//Free Running Mode, ADCW Register, Interrupt
//�Ҽ������� ���� ����, 2���� ä�� �Է¹ޱ� (ADC1,ADC0)
#include<mega128.h>
#include<delay.h>


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
  DDRD=0x00;
  DDRF=0xFC;//PF1_IN, PF0_IN
  
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0), �ʱ� �� ����
  ADCSRA=0xC7;//ADC Control and Status Register A
  //ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADPS2-0(ADC Prescaler Bits)=111->128����
  delay_ms(1); 
  
  //���ͷ�Ʈ �ʱ�ȭ
  EICRA=0b00001010;//�ܺ� ���ͷ�Ʈ INT1(PD1), INT0(PD0) : �ϰ� ����
  EIMSK=0b00000011;//�ܺ� ���ͷ�Ʈ INT1(PD1), INT0(PD0) : �ο��̺�
  
  SREG=0x80;//���� ���ͷ�Ʈ ����
  
  
  while(1)
  {
    delay_ms(1);
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
    PORTC=cnt[N0];//������ ǥ��(��ũ���� LED ON)
    PORTA=cnt[N1];//�Ҽ��� ù°�ڸ� ǥ��(��ũ���� LED ON)
  }
 
}

 
//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  ADMUX=0x00;//�ܱؼ��Է�(ADC0->PF0)
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD1_int1(void)
{
  ADMUX=0x01;//�ܱؼ��Է�(ADC1->PF1)  
}
