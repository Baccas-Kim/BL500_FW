/* ========================================================================== */
/*		Exp06_7.c : Change printf() Function			      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <stdio.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

static int USART1_putchar(char c, FILE *stream);
static FILE device1 = FDEV_SETUP_STREAM(USART1_putchar, NULL, _FDEV_SETUP_WRITE);

static int LCD_putchar(char c, FILE *stream);
static FILE device2 = FDEV_SETUP_STREAM(LCD_putchar, NULL, _FDEV_SETUP_WRITE);

static int TFT_putchar(char c, FILE *stream);
static FILE device3 = FDEV_SETUP_STREAM(TFT_putchar, NULL, _FDEV_SETUP_WRITE);

void USART1_initialize(void)		        /* initialize USART1 */
{
  UBRR1H = 0;                                   // 19200 bps
  UBRR1L = 51;
  UCSR1A = 0x00;                                // asynchronous normal mode
  UCSR1B = 0x18;                                // Rx/Tx enable, 8 data
  UCSR1C = 0x06;                                // no parity, 1 stop, 8 data
}

static int USART1_putchar(char c, FILE *stream)	/* print a character to USART1 */
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

static int LCD_putchar(char c, FILE *stream)	/* print a character to text LCD */
{
  if((c < 0x20) || (c > 0x7E))			// check from 0x20 to 0x7E
    return 0;

  LCD_data(c);
  return 0;
}

static int TFT_putchar(char c, FILE *stream)	/* print a character to TFT-LCD */
{
  if((c < 0x20) || (c > 0x7E))			// check from 0x20 to 0x7E
    return 0;

  TFT_English(c);
  return 0;
}

int main(void)
{ unsigned char i = 1;
  double x = 0.001;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode
  Beep();

  USART1_initialize();                          // initialize USART1

  LCD_string(0x80,"Press KEY1-KEY3!");          // display title
  LCD_string(0xC0,"                ");
  TFT_string(1,1, White,Blue, " C언어  printf() 함수 %서식 지정 출력 ");
  TFT_string(5,4, White,Black, "KEY1 : USART1으로 출력");
  TFT_string(5,6, White,Black, "KEY2 : 텍스트 LCD 모듈로 출력");
  TFT_string(5,8, White,Black, "KEY3 : TFT-LCD 모듈로 출력");

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : PORTD = 0x10;             // KEY1 ?
                      stdout = &device1;	// set USART0 to stdout
                      TFT_string(14,27, Magenta,Black, "KEY1 is OK !");
                      printf(" i=%03d  x=%5.3f\n",i,x);
		      i++;
                      x += 0.001;
                      break;
          case KEY2 : PORTD = 0x20;             // KEY2 ?
                      stdout = &device2;	// set text LCD to stdout
                      TFT_string(14,27, Magenta,Black, "KEY2 is OK !");
                      LCD_command(0xC0);
                      printf(" i=%03d  x=%5.3f ",i,x);
		      i++;
                      x += 0.001;
                      break;
          case KEY3 : PORTD = 0x40;             // KEY2 ?
                      stdout = &device3;	// set text LCD to stdout
                      TFT_string(14,27, Magenta,Black, "KEY3 is OK !");
                      TFT_xy(12,16);
		      TFT_color(Cyan,Black);
                      printf(" i=%03d  x=%5.3f ",i,x);
		      i++;
                      x += 0.001;
                      break;
          default:    break;
        }
    }
}
