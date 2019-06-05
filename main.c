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
//#include @@@ do{}while(0);



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
unsigned char str1[] = "Hello AVR World!\r\n";
unsigned char str2[] = "\r\n";



int main(void)
{
  //unsigned char qdata[QBUFFER];
  UART1_Init();  // baud rate : 9600
  UART0_Init();  // baud rate : 9600
  Hw_Init();
  Timer_Init();
  print(prompt);
    
    do{
		_delay_ms(100);
		PORTA = ~PORTA;
		//UI_Handler(qdata);

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
  //unsigned char ret = 0;
  //ret = rbuf[rx_out];

  receive_data = UDR1;
  PORTA = ~PORTA;
  receive_completion = 1;
}






void Timer_Init(void)//1초를 만들기 위해서 타이머카운트 인터럽트를 사용합니다. 
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
  unsigned char ret = 0;
  unsigned char ptr;
  Tii_count--; ms++; 
  if(!Tii_count){//10ms * 100 = 1000ms delay = 1s 
    Tii_count = 100; ss++; ms=0; 
	PORTB = ~PORTB;
    print(str1);
    ret =(unsigned char)PORTC;
    ptr = &ret;
	UART1_Read_Byte(ptr);
	print(str2);
  } 
  TCNT0 = 0x70; 
} 


//======================================
//Timer1 
//======================================
SIGNAL(SIG_OVERFLOW2)//timer1 Overflow interrupt 
{ 
  
  TCNT2 = 0x70; 
} 

  
//====================================== 
//Exint0
//====================================== 
SIGNAL(SIG_INTERRUPT0)//Exint0 Overflow interrupt 
{
  //@@@
} 


//======================================
//Exint1
//======================================  
SIGNAL(SIG_INTERRUPT1) 
{ 
  //@@@
}
