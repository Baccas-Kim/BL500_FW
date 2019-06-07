//AVR128  Timer_5.c
#include<mega128.h>
#include<delay.h>
int sec=0,cnt=0;//1�� �� 1���� ����� ���� ����
int hour=12,min=0,h2,h1,m2,m1;
char hm_flag=0;
void main()
{
 
  unsigned FND_data[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f}; 
                       //  0,  1,    2,  3,    4,   5,  6,   7,   8,   9
  DDRA=0xff;//A_PORT OUT , FND�� ��ǥ�� ������
  DDRB=0xff;//B_PORT OUT, �ÿ� ���� ���� �� 1�ʸ��� ON/OFF����    
  DDRC=0xff;//C_PORT OUT, Dynamic ������ ���� FND����     
  DDRD=0x00;//D_PORT IN, �ÿ� ���� ���߱� ���� �Է�
  
  //���ͷ�Ʈ ����
  TIMSK=0x05;//Timer/Counter Mask Register(TOIE1=1,TOIE0=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��  
 //Timer1 overflow interrupt, 1=enabled, 0 = disabled   
  TCCR1A=0x00;//Timer/Counter Control Register
  TCCR1B=0x05;
  TCNT1=49911;//Ÿ�̸� ī���� �ʱ� ��  
  
  EICRA=0b00001010;//�ܺ� ���ͷ�ƮPD1(INT1), PD0(INT0)�ϰ� ����
  EIMSK=0b00000011;//2���� �ܺ� ���ͷ�Ʈ �ο��̺�
  
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��  
  
  //�ʱ� �� ����
  h2=hour/10; h1=hour%10;
  m2=min/10; m1=min%10; 
  
  while(1)
  {     
   PORTC=0x08;PORTA=FND_data[h2];delay_ms(5);//h2:�� ��°�ڸ� 
   PORTC=0x04;PORTA=FND_data[h1];delay_ms(5);//h1:�� ù°�ڸ� 
   PORTC=0x02;PORTA=FND_data[m2];delay_ms(5);//m2:�� ��°�ڸ� 
   PORTC=0x01;PORTA=FND_data[m1];delay_ms(5);//m1:�� ù°�ڸ� 
  }
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  hm_flag=1;//�� ���� 
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
  hm_flag=2;//�� ���� 
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // (1/16)us X 1024 X 256 = 16.384ms
 // 1�ʸ� ������� 0.016384 X sec = 1
 // sec = 1/0.016384 -> �� 61.035  

//�� ����,�ѹ����� �����ֱ� ���ؼ� flag���� ���
//�ܺ� ���ͷ�Ʈ�� �޴��� ä�͸��� �����ϱ� ���ؼ�  
 if(hm_flag==1){hm_flag=0;min++;}//������
 if(hm_flag==2){hm_flag=0;hour++;}//�� ����  
 
 if(sec>=60){min++; sec=0;}//1�� ���� ����
 m2=min/10; m1=min%10;
 if(min>=60){hour++; min=0;}
 
 if(hour<10){h2=0xff; h1=hour;}//�ð�ǥ�ð� 10���� ������ ù��° �ڸ��� ǥ��
 if(hour>=10){h2=hour/10; h1=hour%10;}//�ð�ǥ�ð� 10�� ���ų�  ũ�� ù��° �ι�° �ڸ� ��� ǥ��
 if(hour>12)hour=1; 
}
 
interrupt[TIM1_OVF] void Timer1(void)
{
 sec++; 
 PORTB=PORTB^0xFF;//1�� ���� ���� 
 TCNT1=49911;   
} 
