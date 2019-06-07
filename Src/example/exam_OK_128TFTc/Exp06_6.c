/* ========================================================================== */
/*		Exp06_6.c : printf() Function to TFT-LCD Module		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <stdio.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

static int TFT_putchar(char c, FILE *stream);
static FILE device = FDEV_SETUP_STREAM(TFT_putchar, NULL, _FDEV_SETUP_WRITE);

static int TFT_putchar(char c, FILE *stream)	/* print a character to TFT-LCD */
{
  if((c < 0x20) || (c > 0x7E))			// check from 0x20 to 0x7E
    return 0;

  TFT_English(c);
  return 0;
}

int main(void)
{
  unsigned char i = 1;
  double x = 0.001;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode
  Beep();

  LCD_string(0x80," printf to TFT  ");          // display title
  LCD_string(0xC0,"Press KEY1-KEY4!");
  TFT_string(1,1, White,Blue, " C언어  printf() 함수 %서식 지정 출력 ");

  stdout = &device;				// set TFT-LCD to stdout

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : PORTD = 0x10;             // KEY1 ?
                      TFT_string(14,10, Magenta,Black, "KEY1 is OK !");
                      TFT_xy(16,16); TFT_color(Cyan,Black);
                      printf("i = %03d",i);
                      TFT_xy(16,19); TFT_color(Yellow,Black);
                      printf("x = %5.3f",x);
                      i++;
                      x += 0.001;
                      break;
          case KEY2 : PORTD = 0x20;             // KEY2 ?
                      TFT_string(14,10, Magenta,Black, "KEY2 is OK !");
                      TFT_xy(16,16); TFT_color(Cyan,Black);
                      printf("i = %03d",i);
                      TFT_xy(16,19); TFT_color(Yellow,Black);
                      printf("x = %5.3f",x);
		      i++;
                      x += 0.001;
                      break;
          case KEY3 : PORTD = 0x40;             // KEY3 ?
                      TFT_string(14,10, Magenta,Black, "KEY3 is OK !");
                      TFT_xy(16,16); TFT_color(Cyan,Black);
                      printf("i = %03d",i);
                      TFT_xy(16,19); TFT_color(Yellow,Black);
                      printf("x = %5.3f",x);
		      i++;
                      x += 0.001;
                      break;
          case KEY4 : PORTD = 0x80;             // KEY4 ?
                      TFT_string(14,10, Magenta,Black, "KEY4 is OK !");
                      TFT_xy(16,16); TFT_color(Cyan,Black);
                      printf("i = %03d",i);
                      TFT_xy(16,19); TFT_color(Yellow,Black);
                      printf("x = %5.3f",x);
		      i++;
                      x += 0.001;
                      break;
          default:    break;
        }
    }
}
