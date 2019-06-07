/* ========================================================================== */
/*		Exp06_4.c : printf() Function to USART1		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <stdio.h>
#include "OK-128LCD.h"

static int USART1_putchar(char c, FILE *stream);
static FILE device = FDEV_SETUP_STREAM(USART1_putchar, NULL, _FDEV_SETUP_WRITE);

void USART1_initialize(void)		        /* initialize USART0 */
{
  UBRR1H = 0;                                   // 19200 bps
  UBRR1L = 51;
  UCSR1A = 0x00;                                // asynchronous normal mode
  UCSR1B = 0x18;                                // Rx/Tx enable, 8 data
  UCSR1C = 0x06;                                // no parity, 1 stop, 8 data
}

static int USART1_putchar(char c, FILE *stream)	/* print a character to USART0 */
{
  if(c == '\n')                                 // process CR(carriage return)
    { loop_until_bit_is_set(UCSR1A,UDRE1);
      UDR1 = 0x0D;
      loop_until_bit_is_set(UCSR1A,UDRE1);
      UDR1 = 0x0A;
    }
  else
    { loop_until_bit_is_set(UCSR1A,UDRE1);	// Tx ready ?
      UDR1 = c;					// if yes, print
    }
  return 0;
}

int main(void)
{
  unsigned char i = 1;
  double x = 0.001;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  USART1_initialize();                          // initialize USART1

  LCD_string(0x80,"printf to USART1");          // display title
  LCD_string(0xC0,"Press KEY1-KEY4!");

  stdout = &device;				// set USART1 to stdout

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : PORTD = 0x10;             // KEY1 ?
                      printf("KEY1 is OK !  i=%03d  x=%5.3f\n",i,x);
		      i++;
                      x += 0.001;
                      break;
          case KEY2 : PORTD = 0x20;             // KEY2 ?
                      printf("KEY2 is OK !  i=%03d  x=%5.3f\n",i,x);
		      i++;
                      x += 0.001;
                      break;
          case KEY3 : PORTD = 0x40;             // KEY3 ?
                      printf("KEY3 is OK !  i=%03d  x=%5.3f\n",i,x);
		      i++;
                      x += 0.001;
                      break;
          case KEY4 : PORTD = 0x80;             // KEY4 ?
                      printf("KEY4 is OK !  i=%03d  x=%5.3f\n",i,x);
		      i++;
                      x += 0.001;
                      break;
          default:    break;
        }
    }
}
