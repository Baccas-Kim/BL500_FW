/* ========================================================================== */
/*		Exp07_2b.c : Numeric Data Display(3) on TFT-LCD Module	      */
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
  unsigned char x, y, xx, yy;
  double a, b;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp07_2b.c   ");

  stdout = &device;				// set TFT-LCD to stdout

  while(1)
    { TFT_clear_screen();			// 정수
      TFT_string(1,4,White, Magenta," Multiplication Table(정수) ");
      TFT_string(1,6,Cyan,  Black,  "    (printf() 함수 사용)    ");

      for(x = 2, xx = 20; x <= 9; x++, xx += 10)
        { for(y = 1, yy = 10; y <= 9; y++, yy += 10)
	    { TFT_color(Green, Black);		// 단자리 구구단
	      TFT_xy(1, 7 + 3*y); TFT_English(x + '0');
	      TFT_xy(3, 7 + 3*y); TFT_English('x');
              TFT_xy(5, 7 + 3*y); TFT_English(y + '0');
              TFT_xy(7, 7 + 3*y); TFT_English('=');
              TFT_xy(9, 7 + 3*y); printf("%2d",x*y);

              TFT_color(Yellow, Black);		// 10자리 구구단
	      TFT_xy(15, 7 + 3*y); printf("%2d",xx);
	      TFT_xy(18, 7 + 3*y); TFT_English('x');
              TFT_xy(20, 7 + 3*y); printf("%2d",yy);
              TFT_xy(23, 7 + 3*y); TFT_English('=');
              TFT_xy(25, 7 + 3*y); printf("%4d",xx*yy);
            }
          Beep();
          Delay_ms(3000);
        }

      TFT_clear_screen();			// 실수
      TFT_string(1,4,White, Magenta," Multiplication Table(실수) ");
      TFT_string(1,6,Cyan,  Black,  "    (printf() 함수 사용)    ");

      for(x = 1,a = 1.1; x <= 9; x++, a += 1.1)
        { for(y = 1, b = 1.1; y <= 9; y++, b += 1.1)
	    { TFT_color(Green, Black);		// 실수 구구단
	      TFT_xy( 6, 7 + 3*y); printf("%3.1f",a);
	      TFT_xy(10, 7 + 3*y); TFT_English('x');
              TFT_xy(12, 7 + 3*y); printf("%3.1f",b);
              TFT_xy(16, 7 + 3*y); TFT_English('=');
              TFT_xy(18, 7 + 3*y); printf("%5.2f",a*b);
            }
          Beep();
          Delay_ms(3000);
        }
    }
}
