//AVR128  ADC_2.c
//Analog to Digital Converter, LED Display
//Free Running Mode
//정수부 전압 측정
#include<mega128.h>
#include<delay.h>
void main()
{
  int i_vin;
  
  //포트 설정 
  DDRA=0xFF;//A_PORT OUT 
  DDRC=0xFF;//C_PORT OUT 
  DDRF=0xFE;//PF0_IN
  
  ADMUX=0x00;//단극성입력(ADC0->PF0), ADLAR(ADC Left Adjust Result)=0 
  //ADCH(8비트) + ADCL(8비트) = 16비트를 사용 하는데 하위비트에서 부터 10비트 사용
  //ADLAR(ADC Left Adjust Result)=1이면 상위비트에서 부터 10비트 사용 
  ADCSRA=0xE7;//ADC Control and Status Register A
  //ADEN(ADC Enable)=1, ADSC(ADC Start Conversion)=1, ADFR(ADC Free Running Select)=1, 
  //ADPS2-0(ADC Prescaler Bits)=128분주
  PORTC=~ADCSRA;
  if(ADCSRA==0xE7)delay_ms(3000);//변환이 완료되면 ADIF(ADC Interrupt Flag=1)가 1이 된다.
  //너무 빨리 동작을 하여 눈으로 확인 하고자 약 3초간의 시간 지연을 주었다. 
  //리셋버튼을 누르고 난 후에 약 3초를 기다리면  ADIF에 연결된 LED가 ON이 되는 것을 확인 할수가 있다.  
  
  while(1)
  {
   i_vin = (int)ADCL;//0에서 부터 255까지 
   i_vin += (int)ADCH<<8;//0에서 부터 1023까지 
   //ADCH<<8는 ADCH*256 과 같다.  
   //8비트 프로세서이므로 한번에 8비트만 처리하므로 먼저 ADCL레지스터에 
   //데이터를 쓴 다음 ADCL레지스터에 쓴다. 
   PORTC=~ADCSRA;//ADC Control and Atatus Register A의 값을 LED로 확인 
   PORTA=~(int)(i_vin/100)>>1;//싱크전류로 LED를 구동하므로 반전 시킴. 
   //만약 5V가 입력이 된다면 1023/10=10에서 10(1010)>>1 하면 0101에서 정수 값 5V(최대 측정 값) 
   //결국 10을 우로 1비트 이동하면 2로 나눈것과 같다.  
  }
}

//PORTA의 출력은 0V(00000000), 1V(00000001), 2V(00000010), 3V(00000011), 4V(00000100), 5V(00000101)이 된다.
