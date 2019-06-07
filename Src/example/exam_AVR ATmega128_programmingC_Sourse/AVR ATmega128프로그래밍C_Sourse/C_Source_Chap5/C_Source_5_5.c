//AVR128  Timer_5.c
#include<mega128.h>
#include<delay.h>
int sec=0,cnt=0;//1초 및 1분을 만들기 위한 변수
int hour=12,min=0,h2,h1,m2,m1;
char hm_flag=0;
void main()
{
 
  unsigned FND_data[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f}; 
                       //  0,  1,    2,  3,    4,   5,  6,   7,   8,   9
  DDRA=0xff;//A_PORT OUT , FND의 수표시 데이터
  DDRB=0xff;//B_PORT OUT, 시와 분의 구분 및 1초마다 ON/OFF동작    
  DDRC=0xff;//C_PORT OUT, Dynamic 구동을 위한 FND선택     
  DDRD=0x00;//D_PORT IN, 시와 분을 맞추기 위한 입력
  
  //인터럽트 설정
  TIMSK=0x05;//Timer/Counter Mask Register(TOIE1=1,TOIE0=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//타이머 카운터 초기 값  
 //Timer1 overflow interrupt, 1=enabled, 0 = disabled   
  TCCR1A=0x00;//Timer/Counter Control Register
  TCCR1B=0x05;
  TCNT1=49911;//타이머 카운터 초기 값  
  
  EICRA=0b00001010;//외부 인터럽트PD1(INT1), PD0(INT0)하강 에지
  EIMSK=0b00000011;//2개의 외부 인터럽트 인에이블
  
  SREG=0x80;//전역 인터럽트 인에이블 비트 셋  
  
  //초기 값 설정
  h2=hour/10; h1=hour%10;
  m2=min/10; m1=min%10; 
  
  while(1)
  {     
   PORTC=0x08;PORTA=FND_data[h2];delay_ms(5);//h2:시 둘째자리 
   PORTC=0x04;PORTA=FND_data[h1];delay_ms(5);//h1:시 첫째자리 
   PORTC=0x02;PORTA=FND_data[m2];delay_ms(5);//m2:분 둘째자리 
   PORTC=0x01;PORTA=FND_data[m1];delay_ms(5);//m1:분 첫째자리 
  }
}

//외부 인터럽트 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  hm_flag=1;//분 조정 
}

//외부 인터럽트 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
  hm_flag=2;//시 조정 
}

//타이머/카운터 인터럽트 
interrupt[TIM0_OVF] void Timer0(void)
{
 // (1/16)us X 1024 X 256 = 16.384ms
 // 1초를 만들려면 0.016384 X sec = 1
 // sec = 1/0.016384 -> 약 61.035  

//분 조정,한번씩만 더해주기 위해서 flag변수 사용
//외부 인터럽트를 받더라도 채터링을 제거하기 위해서  
 if(hm_flag==1){hm_flag=0;min++;}//분조정
 if(hm_flag==2){hm_flag=0;hour++;}//시 조정  
 
 if(sec>=60){min++; sec=0;}//1분 마다 동작
 m2=min/10; m1=min%10;
 if(min>=60){hour++; min=0;}
 
 if(hour<10){h2=0xff; h1=hour;}//시간표시가 10보다 적으면 첫번째 자리만 표시
 if(hour>=10){h2=hour/10; h1=hour%10;}//시간표시가 10과 같거나  크면 첫번째 두번째 자리 모두 표시
 if(hour>12)hour=1; 
}
 
interrupt[TIM1_OVF] void Timer1(void)
{
 sec++; 
 PORTB=PORTB^0xFF;//1초 마다 동작 
 TCNT1=49911;   
} 
