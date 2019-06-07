//AVR128  ADC_7.c
//Analog to Digital Converter, FND Display
//Free Running Mode, ADCW Register, Interrupt
//소수점이하 전압 측정, 2개의 채널 입력받기 (ADC1,ADC0)
#include<mega128.h>
#include<delay.h>


int i_vin;//정수부 입력전압 계산 
float f_vin;//실수부 입력전압 계산 



void main()
{
  int cnt[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
            //{0,1,2,3,4,5,6,7,8,9} FND Display Data
 
char N0,N1;//전압을 표시하기 위한 변수 
//N0:정수부 전압 표시, N1:소수점 첫째자리 전압 표시 

  //포트 설정 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRD=0x00;
  DDRF=0xFC;//PF1_IN, PF0_IN
  
  ADMUX=0x00;//단극성입력(ADC0->PF0), 초기 값 설정
  ADCSRA=0xC7;//ADC Control and Status Register A
  //ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADPS2-0(ADC Prescaler Bits)=111->128분주
  delay_ms(1); 
  
  //인터럽트 초기화
  EICRA=0b00001010;//외부 인터럽트 INT1(PD1), INT0(PD0) : 하강 에지
  EIMSK=0b00000011;//외부 인터럽트 INT1(PD1), INT0(PD0) : 인에이블
  
  SREG=0x80;//전역 인터럽트 설정
  
  
  while(1)
  {
    delay_ms(1);
    i_vin = (int)ADCL;//0에서 부터 255까지 
    i_vin += (int)ADCH<<8;//ADCL(0-255)의 레지스터값과 ADCH(0,256,512,768)레지스터 값을 
    //더한 결과가 i_vin변수에 1023까지 저장 된다.
    f_vin=(float)i_vin*5.0/1023;//0(0V),1(0.0048875V), ~ ,1023(5V)
    //만일 i_vin 변수(ADC)의 값이 1023이면 입력 전압은 5V가 된다.  
    
    N0=(int)((f_vin*10.0)+0.5)/10;//0.0 ~ 5.0 사이의 정수부만 추출(offset error 보정) 
    //소수점이하를 구하려고 하면 ?
    //만약 입력 전압 f_vin= 1.2V라고 하면 1.2에서 소수부를 없애려면 10을 
    //곱해서 12가된다. 다시 12 % 10 으로 계산하면 2가 되는데 소수점 첫째자리를 구하면 된다.
    N1=(int)((f_vin*10.0)+0.5)%10;//소수점 첫째자리
    PORTC=cnt[N0];//정수부 표시(싱크전류 LED ON)
    PORTA=cnt[N1];//소수점 첫째자리 표시(싱크전류 LED ON)
  }
 
}

 
//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  ADMUX=0x00;//단극성입력(ADC0->PF0)
}

//외부 인터럽트 1 
interrupt[EXT_INT1] void PD1_int1(void)
{
  ADMUX=0x01;//단극성입력(ADC1->PF1)  
}
