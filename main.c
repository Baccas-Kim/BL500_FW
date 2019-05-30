/*========================================================================
//PROJECT : BL500_FW
//AUTOR : Dong Hun Kim
//LICENSE : MIT License
========================================================================*/
#define F_CPU 16000000UL  // 16 MHz, depend on user component
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

volatile unsigned char receive_data, receive_completion=0;

unsigned char prompt[]="ATmaga128> ";
unsigned char str1[] = "Hello AVR World!";

void USART_Init(void);
void TX0_Byte(unsigned char data);
void TX1_Byte(unsigned char data);
//void PRINT_M(unsigned char *message);
void print(unsigned char *message);


int main(void)
{
	
    USART_Init();  // baud rate : 9600
    sei();
    print(prompt);
    do{
		print(str1);
		_delay_ms(1000);
		PORTA = ~PORTA;
		
        if(receive_completion){
            receive_completion = 0;
            switch(receive_data){
                case '\r' : TX0_Byte('\r'); TX0_Byte('\n');
                            print(prompt);
                            break;
                case '\n' : TX0_Byte('\r'); TX0_Byte('\n');
                            print(prompt);
                            break;
                default   : TX0_Byte(receive_data);
                            TX0_Byte('\r'); TX0_Byte('\n');
                            print(prompt);
                            break;
            }
        }
    }while(1);
}

void USART_Init(void)
{
    UBRR0H = 0;//baud = 9600!!
    UBRR0L = 207;//baud = 9600!!
    UCSR0A = 0x02;                  //asynchronous normal mode
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //interrupt, Rx/Tx enable
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);//no parity, 1 stop, 8 data
	
	UBRR1H = 0;//baud = 9600!!
    UBRR1L = 207;//baud = 9600!!
    UCSR1A = 0x02;                  //asynchronous normal mode
    UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); //interrupt, Rx/Tx enable
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);//no parity, 1 stop, 8 data

}

void TX0_Byte(unsigned char data)
{
    while(!(UCSR0A & (1<<UDRE0)));  //wait for empty transmit buffer
    UDR0 = data;                    //put data into buffer, send the data
}

void TX1_Byte(unsigned char data)
{
    while(!(UCSR1A & (1<<UDRE1)));  //wait for empty transmit buffer
    UDR1 = data;                    //put data into buffer, send the data
}

void print(unsigned char *message)
{
    while(*message != '\0'){
        TX0_Byte(*message);
        TX1_Byte(*message);
        message++;

    }
	
    while(*message != '\0'){
        TX1_Byte(*message);
        message++;
    }
}



SIGNAL(SIG_UART0_RECV)
{
    receive_data = UDR0;
    receive_completion = 1;
}


SIGNAL(SIG_UART1_RECV)
{
    	receive_data = UDR1;
		PORTA = ~PORTA;
		receive_completion = 1;
}
