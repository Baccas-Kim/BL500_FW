//AVR128  ADC_4.c
//Analog to Digital Converter, FND Display
//Free Running Mode, ADCW Register
//소수점이하 전압 측정
#include<mega128.h>

void main()
{
  int cnt[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
            //{0,1,2,3,4,5,6,7,8,9} FND Display Data
  int i_vin;//정수부 입력전압 계산 
  float f_vin;//실수부 입력전압 계산 
  char N0,N1;//전압을 표시하기 위한 변수 
  //N0:정수부 전압 표시, N1:소수점 첫째자리 전압 표시 

  //포트 설정 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//단극성입력(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8비트) + ADCL(8비트) = 16비트를 사용 하는데 하위비트에서 부터 10비트 사용
  //ADLAR(ADC Left Adjust Result)=1이면 상위비트에서 부터 10비트 사용 
  ADCSRA=0xE7;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADPS2-0(ADC Prescaler Bits)=111->128분주
 
  while(1)
  {
    i_vin=(int)ADCW;//ADCW(ADC Word)는 16비트 레지스터로 ADC값을 간단하게 처리 할수있다. 
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


//만약 입력 전압 f_vin= 1.23V라고 하면 1.23에서 소수부를 없애려면 100을 
   //곱해서 123이된다. 다시 123 % 100으로 계산하면 23이 되는데 소수점 첫째자리를 구하기 위해서는
   // 23을 23/10 하면 2, 소수점 둘째자리를 구하기 위해서는 23%10을 계산하면 3이된다.

//응용과제 소수점 둘째자리와 소수점 세째자리 까지 표시되도록 프로그램 하시오.