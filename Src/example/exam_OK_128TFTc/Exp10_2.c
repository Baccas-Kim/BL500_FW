/* ========================================================================== */
/*		Exp10_2.c : 터치 스크린을 이용한 낙서장		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  unsigned int xo = 0, yo = 0;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp10_2.c    ");

  Initialize_touch_screen();			// initialize touch screen

  Rectangle(0,0, 239,319, Green);		// draw screen outline
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(4,1, White,Magenta, " 마음대로 쓰는 낙서장 ");

  while(1)
    { Touch_screen_input();			// input touch screen

      if((x_touch == 0) && (y_touch == 0))	// if not touch, skip
        { xo = 0;
          yo = 0;
        }

      if((xo == 0) && (yo == 0))		// if first touch, skip
        { xo = x_touch;
          yo = y_touch;
        }
      else					// draw line between old and new point
        { Line(xo,yo, x_touch,y_touch, White);
          xo = x_touch;
          yo = y_touch;
        }
    }
}
