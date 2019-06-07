/* ========================================================================== */
/*		Exp07_2a.c : Numeric Data Display(2) on TFT-LCD Module	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  unsigned char x, y, xx, yy;
  float a, b;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp07_2a.c   ");

  while(1)
    { TFT_clear_screen();			// 정수
      TFT_string(1,4,White, Magenta," Multiplication Table(정수) ");
      TFT_string(1,6,Cyan,  Black,  "   (OK-128TFT.h 함수 사용)  ");

      for(x = 2, xx = 20; x <= 9; x++, xx += 10)
        { for(y = 1, yy = 10; y <= 9; y++, yy += 10)
	    { TFT_color(Green, Black);		// 단자리 구구단
	      TFT_xy(1, 7 + 3*y); TFT_English(x + '0');
	      TFT_xy(3, 7 + 3*y); TFT_English('x');
              TFT_xy(5, 7 + 3*y); TFT_English(y + '0');
              TFT_xy(7, 7 + 3*y); TFT_English('=');
              TFT_xy(9, 7 + 3*y); TFT_unsigned_decimal(x*y,0,2);

              TFT_color(Yellow, Black);		// 10자리 구구단
	      TFT_xy(15, 7 + 3*y); TFT_unsigned_decimal(xx,0,2);
	      TFT_xy(18, 7 + 3*y); TFT_English('x');
              TFT_xy(20, 7 + 3*y); TFT_unsigned_decimal(yy,0,2);
              TFT_xy(23, 7 + 3*y); TFT_English('=');
              TFT_xy(25, 7 + 3*y); TFT_unsigned_decimal(xx*yy,0,4);
            }
          Beep();
          Delay_ms(3000);
        }

      TFT_clear_screen();			// 실수
      TFT_string(1,4,White, Magenta," Multiplication Table(실수) ");
      TFT_string(1,6,Cyan,  Black,  "   (OK-128TFT.h 함수 사용)  ");

      for(x = 1,a = 1.1; x <= 9; x++, a += 1.1)
        { for(y = 1, b = 1.1; y <= 9; y++, b += 1.1)
	    { TFT_color(Green, Black);		// 실수 구구단
	      TFT_xy( 6, 7 + 3*y); TFT_unsigned_float(a,1,1);
	      TFT_xy(10, 7 + 3*y); TFT_English('x');
              TFT_xy(12, 7 + 3*y); TFT_unsigned_float(b,1,1);
              TFT_xy(16, 7 + 3*y); TFT_English('=');
              TFT_xy(18, 7 + 3*y); TFT_unsigned_float(a*b,2,2);
            }
          Beep();
          Delay_ms(3000);
        }
    }
}
