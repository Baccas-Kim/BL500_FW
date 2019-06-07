/* ========================================================================== */
/*		Exp07_2.c : Numeric Data Display(1) on TFT-LCD Module	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

void TFT_2d(unsigned char number)               /* display 2-digit decimal number */
{
  unsigned char i;

  i = number/10;                                // 10^1
  if(i == 0) TFT_English(' ');
  else       TFT_English(i + '0');

  i = number % 10;                              // 10^0
  TFT_English(i + '0');
}

void TFT_4d(unsigned int number)                /* display 4-digit decimal number */
{
  unsigned int i;
  unsigned char flag;

  flag = 0;
  i = number/1000;                              // 10^3
  if(i == 0) TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  number = number % 1000;                       // 10^2
  i = number/100;
  if((i == 0) && (flag == 0))
             TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  number = number % 100;                        // 10^1
  i = number/10;
  if((i == 0) && (flag == 0))
             TFT_English(' ');
  else {     TFT_English(i + '0');
             flag = 1;
       }

  i = number % 10;                              // 10^0
  TFT_English(i + '0');
}

void TFT_1d1(float number)		        /* floating-point number x.x */
{
  unsigned char i, j;

  j = (int)(number*10. + 0.5);
  i = j / 10;					// 10^0
  TFT_English(i + '0');
  TFT_English('.');
  i = j % 10;					// 10^-1
  TFT_English(i + '0');
}

void TFT_2d2(float number)		        /* floating-point number xx.xx */
{
  unsigned int i, j;

  j = (int)(number*100. + 0.5);
  i = j / 1000;					// 10^1
  if(i == 0) TFT_English(' ');
  else       TFT_English(i + '0');

  j = j % 1000;					// 10^0
  i = j / 100;
  TFT_English(i + '0');
  TFT_English('.');

  j = j % 100;					// 10^-1
  i = j / 10;
  TFT_English(i + '0');

  i = j % 10;					// 10^-2
  TFT_English(i + '0');
}

int main(void)
{
  unsigned char x, y, xx, yy;
  float a, b;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp07_2.c    ");

  while(1)
    { TFT_clear_screen();			// 정수
      TFT_string(1,4,White, Magenta," Multiplication Table(정수) ");
      TFT_string(1,6,Cyan,  Black,  "     (사용자 함수 사용)     ");

      for(x = 2, xx = 20; x <= 9; x++, xx += 10)
        { for(y = 1, yy = 10; y <= 9; y++, yy += 10)
	    { TFT_color(Green, Black);		// 단자리 구구단
	      TFT_xy(1, 7 + 3*y); TFT_English(x + '0');
	      TFT_xy(3, 7 + 3*y); TFT_English('x');
              TFT_xy(5, 7 + 3*y); TFT_English(y + '0');
              TFT_xy(7, 7 + 3*y); TFT_English('=');
              TFT_xy(9, 7 + 3*y); TFT_2d(x*y);

              TFT_color(Yellow, Black);		// 10자리 구구단
	      TFT_xy(15, 7 + 3*y); TFT_2d(xx);
	      TFT_xy(18, 7 + 3*y); TFT_English('x');
              TFT_xy(20, 7 + 3*y); TFT_2d(yy);
              TFT_xy(23, 7 + 3*y); TFT_English('=');
              TFT_xy(25, 7 + 3*y); TFT_4d(xx*yy);
            }
          Beep();
          Delay_ms(3000);
        }

      TFT_clear_screen();			// 실수
      TFT_string(1,4,White, Magenta," Multiplication Table(실수) ");
      TFT_string(1,6,Cyan,  Black,  "     (사용자 함수 사용)     ");

      for(x = 1,a = 1.1; x <= 9; x++, a += 1.1)
        { for(y = 1, b = 1.1; y <= 9; y++, b += 1.1)
	    { TFT_color(Green, Black);		// 실수 구구단
	      TFT_xy( 6, 7 + 3*y); TFT_1d1(a);
	      TFT_xy(10, 7 + 3*y); TFT_English('x');
              TFT_xy(12, 7 + 3*y); TFT_1d1(b);
              TFT_xy(16, 7 + 3*y); TFT_English('=');
              TFT_xy(18, 7 + 3*y); TFT_2d2(a*b);
            }
          Beep();
          Delay_ms(3000);
        }
    }
}
