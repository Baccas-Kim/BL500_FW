/*========================================================================
//PROJECT : BL500_FW
//AUTOR : Dong Hun Kim
//LICENSE : MIT License
========================================================================*/
#define F_CPU 16000000UL  // 16 MHz, depend on user component
#define QBUFFER 150
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

void Hw_Init(void);
void UART0_Init(void);
void UART1_Init(void);
void UART0_Read_Byte(unsigned char data);
void UART1_Read_Byte(unsigned char data);
void TX1_Byte(unsigned char data);
void Timer_Init(void);
void print(unsigned char *message);
void UI_Handler(unsigned char *qdata);


//regeistor involved variables : located in .data Section
volatile unsigned char Tii_count = 0;
volatile unsigned char ss = 0, ms = 0;
volatile unsigned char rbuf_in = 0, rbuf_out = 0;
volatile unsigned char receive_data = 0, receive_completion = 0;
volatile unsigned char rbuf[2000] = {0};
unsigned char prompt[]="ATmaga128> ";
unsigned char str1[] = "Hello AVR World!";



int main(void)
{
  unsigned char qdata[QBUFFER];
  UART1_Init();  // baud rate : 9600
	UART0_Init();  // baud rate : 9600
  print(prompt);
    
    do{
		print(str1);
		_delay_ms(1000);
		PORTA = ~PORTA;
		UI_Handler(qdata);

    }while(1);

}

void Hw_Init(void)
{
  sei();        //enable all interrupts 
}

void UART0_Init(void)
{
  UBRR0H = 0;//baud = 9600!!
  UBRR0L = 207;//baud = 9600!
  UCSR0A = 0x02;                  //asynchronous normal mode
  UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //interrupt, Rx/Tx enable
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);//no parity, 1 stop, 8 data
}

void UART1_Init(void)
{
  UBRR1H = 0;//baud = 9600!!
  UBRR1L = 207;//baud = 9600!!
  UCSR1A = 0x02;                  //asynchronous normal mode
  UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); //interrupt, Rx/Tx enable
  UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);//no parity, 1 stop, 8 data
}

void UART0_Read_Byte(unsigned char data)
{
  while(!(UCSR0A & (1<<UDRE0)));  //wait for empty transmit buffer
  UDR0 = data;                    //put data into buffer, send the data
}

void UART1_Read_Byte(unsigned char data)
{
  while(!(UCSR1A & (1<<UDRE1)));  //wait for empty transmit buffer
  UDR1 = data;                    //put data into buffer, send the data
}

void print(unsigned char *message)
{
  while(*message != '\0'){
    UART0_Read_Byte(*message);
    UART1_Read_Byte(*message);
    message++;
  }
	
  while(*message != '\0'){
    UART1_Read_Byte(*message);
    message++;
  }
}

void UI_Handler(unsigned char *qdata)
{
  if(receive_completion){
    receive_completion = 0;
    switch(receive_data){
        case '\r' : UART0_Read_Byte('\r'); UART0_Read_Byte('\n');
                    print(prompt);
                    break;
        case '\n' : UART0_Read_Byte('\r'); UART0_Read_Byte('\n');
                    print(prompt);
                    break;
        default   : UART0_Read_Byte(receive_data);
                    UART0_Read_Byte('\r'); UART0_Read_Byte('\n');
                    print(prompt);
                    break;
    }
  }
  return;
}

SIGNAL(SIG_UART0_RECV)
{
  receive_data = UDR0;
  receive_completion = 1;
}


SIGNAL(SIG_UART1_RECV)
{
  unsigned char ret = 0;
  //ret = rbuf[rx_out];


  receive_data = UDR1;
  PORTA = ~PORTA;
  receive_completion = 1;
}




//=================================================================================timer0 
SIGNAL(SIG_OVERFLOW0)//timer0 Overflow interrupt 
{ 
  interrupt_count--; ms++; 
    if(!interrupt_count){//10ms * 100 = 1000ms delay = 1s 
        interrupt_count = 100; ss++; ms=0; 
    } 
  TCNT0 = 0x70; 
} 
  
  
//=================================================================================timer1 
SIGNAL(SIG_OVERFLOW2)//timer1 Overflow interrupt 
{ 
  stw_count--; 
    if(!stw_count){//10ms * 100 = 1000ms delay = 1s 
        stw_count = 100; sws++; 
    } 
  TCNT2 = 0x70; 
} 
  
  
//================================================================================= 
SIGNAL(SIG_INTERRUPT0)//Exint0 Overflow interrupt 
{//스위치를 누를때마다 부저가 켜집니다. 
 mode++; buzzer_on;_delay_ms(2); if(mode>7){mode=0;}  
} 
  
//================================================================================= 
SIGNAL(SIG_INTERRUPT1)//Exint1 외부인터럽트1 스위치는 모드별로 동작이 바뀌므로 케이스문을 사용합니다. 
{ 
  LED_on; 
  switch(mode){ 
  case 0: mi++;buzzer_on; break; 
  case 1: mi++;buzzer_on; break; 
  case 2: dd++;buzzer_on; break; 
  case 3: mo++;buzzer_on; break; 
  case 4: _delay_ms(2); break; 
  case 5: _delay_ms(2); break; 
  case 6: _delay_ms(2); break; 
  case 7: _delay_ms(2); break; 
  }
}


void Timer_Init(void);(void)//1초를 만들기 위해서 타이머카운트 인터럽트를 사용합니다. 
{ 
  TCCR0 = 0x07; 
  TCNT0 = 0x70; //{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  TCCR2 = 0x07; 
  TCNT2 = 0x70; //{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  TIMSK = 0x41; 
  TIFR = 0x40; 
} 

//======================================
//Timer0 
//======================================
SIGNAL(SIG_OVERFLOW0)//timer0 Overflow interrupt 
{ 
  Tii_count--; ms++; 
  if(!Tii_count){//10ms * 100 = 1000ms delay = 1s 
    Tii_count = 100; ss++; ms=0; 
  } 
  TCNT0 = 0x70; 
} 


//======================================
//Timer1 
//======================================
SIGNAL(SIG_OVERFLOW2)//timer1 Overflow interrupt 
{ 
  stw_count--; 
    if(!stw_count){//10ms * 100 = 1000ms delay = 1s 
      stw_count = 100; sws++; 
    } 
  TCNT2 = 0x70; 
} 

  

//======================================
//Exint1
//======================================  
SIGNAL(SIG_INTERRUPT1) 
{ 
  LED_on; 
  switch(mode){ 
  
  case 0: 
  do{

  }while(0);
  break; 
   
  }
}
