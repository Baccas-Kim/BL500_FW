//AVR128  ADC_1.c
//Analog to Digital Converter, LED Display
//Single Conversion Mode
#include<mega128.h>

void main()
{
  //포트 설정 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//단극성입력(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8비트) + ADCL(8비트) = 16비트를 사용 하는데 하위비트에서 부터 10비트 사용
  //ADLAR(ADC Left Adjust Result)=1이면 상위비트에서 부터 10비트 사용 
  ADCSRA=0x87;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADPS2-0(ADC Prescaler Bits)=128분주
  
  while(1)
  {
   ADCSRA|=0x40;//ADSC(ADC Start Conversion)=1
   while(ADCSRA & 0x10==0);//ADIF(ADC Interrupt Flag)=1이 될때까지(ADC변환이 끝날때까지)기다린다.
   PORTA=~ADCL;//싱크전류로 LED를 구동 하므로 반전(10비트 중 하위 8비트 표시) 
   PORTC=~ADCH;//싱크전류로 LED를 구동 하므로 반전(10비트 중 상위 2비트 표시)
  }
}
 //소스 코드 분석
 //ADC는 10비트를 사용을 하는데 하나의 레지스터로는 8비트밖에 처리를 하지
 //못 하므로 AHCH레지스터에 8비트저장, ADCL레지스터에 8비트를 저장 총 16비트 중에서
 //ADMUX레지스터의 ADLAR의 값이 0과 1에 따라 10비트를 상위부터 10비트를 사용할것인지
 //하위 10비트를 사용할것인지가 결정된다.
 //0(0V),1,2,3,~,1022,1023(5V)가 되는데 간단하게 정수부만 계산을 해 보면,
 //1023/5=204.6이므로 1V에 약 204에서 205가 되는데
 //소수점을 생략하고 계산을 하면, 1V(204), 2V(408), 3V(612), 4v(816), 5v(1020)정도 된다. 
 //만약 ADC0(PF0)으로 1V가 입력이 된다면 10진수 204가 되어야 하므로 ADCH=00000000이 저장되고 
 //ADCL=11001100이 저장된다. 
//응용과제  ADMUX 레지스터의 ADLAR(ADC Multuplxer Selection Regisitor)=1 로 설정하고
//PORTC와 PORTA의 LED의 동작을 보면서 비교 
