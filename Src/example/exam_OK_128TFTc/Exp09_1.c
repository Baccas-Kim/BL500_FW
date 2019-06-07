/* ========================================================================== */
/*		Exp09_1.c : 수학함수를 이용한 그래픽 쇼		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  unsigned int i, x, y;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp09_1.c    ");

  while(1)
    { Beep();
      Rectangle(0,0, 239,319, Magenta);		// graphic show 1(line)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(x = 10; x <= 230; x += 10)
        { Line(x,0, x,319, Green);
          Delay_ms(50);
        }
      for(y = 10; y <= 310; y += 10)
        { Line(0,y, 239,y, Green);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 239,319, Magenta);		// (ramp)
      Rectangle(1,1, 238,318, Magenta);
      for(i = 0,x = 12,y = 20; i <= 34; i++,x += 12,y += 20)
        { Line(x,0, 0,y, Green);
          Line(0,319-y, x,319, Green);
          Delay_ms(50);
        }
      for(i = 0,x = 420,y = 700; i <= 34; i++,x -= 12,y -= 20)
        { Line(x,0, 0,y, Red);
          Line(0,319-y, x,319, Red);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 2(rectangle)
      Rectangle(1,1, 238,318, Magenta);
      for(i = 0,x = 114,y = 152; i <= 18; i++,x -= 6,y -= 8)
        { Rectangle(x,y, 239-x,319-y, Cyan);
          Delay_ms(50);
        }
      Delay_ms(1000);
      for(i = 0,x = 6,y = 8; i <= 18; i++,x += 6,y += 8)
        { Rectangle(x,y, 239-x,319-y, White);
          Delay_ms(50);
        }
      for(i = 0,x = 114,y = 152; i <= 18; i++,x -= 6,y -= 8)
        { Rectangle(x,y, 239-x,319-y, Green);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 3(circle)
      Rectangle(1,1, 238,318, Magenta);
      for(i = 10; i <= 160; i += 10)
        { Circle(120,160,i, Green);
          Delay_ms(50);
        }
      Delay_ms(1000);
      for(i = 160; i >= 10; i -= 10)
        { Circle(120,160,i, Red);
          Delay_ms(50);
        }
      for(i = 10; i <= 160; i += 10)
        { Circle(120,160,i, White);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 239,319, Magenta);		// (two)
      Rectangle(1,1, 238,318, Magenta);
      for(i = 10; i <= 120; i += 10)
        { Circle(120,80,i, Green);
          Circle(120,240,i, Green);
          Delay_ms(50);
        }
      Delay_ms(1000);
      for(i = 120; i >= 10; i -= 10)
        { Circle(120,80,i, White);
          Circle(120,240,i, White);
          Delay_ms(50);
        }
      for(i = 10; i <= 120; i += 10)
        { Circle(120,80,i, Cyan);
          Circle(120,240,i, Cyan);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 4(sine)
      Rectangle(1,1, 238,318, Magenta);
      Line(120,0, 120,319, Magenta);
      for(i = 10; i <= 120; i += 10)
        { Sine(i, 0, Green);
          Delay_ms(20);
        }
      Delay_ms(1000);
      for(i = 10; i <= 120; i += 10)
        { Sine(i, 1, Blue);
          Delay_ms(20);
        }
      Delay_ms(3000);
      TFT_clear_screen();
    }
}

