/* ========================================================================== */
/*		Exp09_2.c : 난수를 이용한 랜덤 그래픽		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <stdlib.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

void Gradation(U16 x1,U16 y1, U16 x2,U16 y2, U16 color) /* gradation for rectangle */
{
  unsigned char i, limit;
  unsigned int red, green, blue;

  red   = (color >> 11) & 0x001F;
  green = (color >> 5) & 0x003F;
  blue  = color & 0x001F;

  limit = abs(x1-x2) / 2;

  for(i = 0; i < limit; i++)
  { Rectangle(x1++,y1++, x2--,y2--, color);

    red   += 1;
    green += 2;
    blue  += 1;

    color  = (red & 0x001F) >> 11;
    color |= (green & 0x003F) << 5;
    color |= blue & 0x001F;
  }
}

int main(void)
{
  unsigned char i, key = 0, color, fill;
  signed int x1, y1, x2, y2, r;
  unsigned int Color[16] = { White, DarkGrey, Red, Green, Blue, Yellow, Cyan, Magenta,
                             Pink, Purple, LightGrey, Violet, Navy, Khaki, DarkGreen, Olive };

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp09_2.c    ");

  TCCR1A = 0x00;				// CTC mode(4), don't output OC1A
  TCCR1B = 0x0A;				// 16MHz/8/(1+199) = 10kHz
  TCCR1C = 0x00;
  OCR1A = 199;
  TCNT1 = 0;

  TFT_string( 3,10,White,Blue, " 난수 이용 랜덤 그래픽 "); // wait KEY1 to start
  TFT_string( 3,20,Green,Black," Press KEY1 to start ! ");
  while(key != KEY1) key = Key_input();

  srand(TCNT1);					// initialize random number
  TFT_clear_screen();

  while(1)
    { Rectangle(0,0, 239,319, Magenta);		// graphic show 1(line 1)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 100; i++)
        { x2 = (rand() & 0x00FF) * 23 / 25;
          y2 = (rand() & 0x00FF) * 31 / 25;

	  Line(10,10, x2,y2, White);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 2(line 2)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 100; i++)
        { x1 = (rand() & 0x00FF) * 23 / 25;
          y1 = (rand() & 0x00FF) * 31 / 25;
          x2 = (rand() & 0x00FF) * 23 / 25;
          y2 = (rand() & 0x00FF) * 31 / 25;
          color = rand() & 0x000F;

	  Line(x1,y1, x2,y2, Color[color]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 3(rectangle)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 50; i++)
        { x1 = (rand() & 0x00FF) * 23 / 25;
          y1 = (rand() & 0x00FF) * 31 / 25;
          x2 = (rand() & 0x00FF) * 23 / 25;
          y2 = (rand() & 0x00FF) * 31 / 25;
          color = rand() & 0x000F;

	  Rectangle(x1,y1, x2,y2, Color[color]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 4(block)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 30; i++)
        { x1 = (rand() & 0x00FF) * 23 / 25;
          y1 = (rand() & 0x00FF) * 31 / 25;
          x2 = (rand() & 0x00FF) * 23 / 25;
          y2 = (rand() & 0x00FF) * 31 / 25;
          color = rand() & 0x000F;
          fill = rand() & 0x000F;

	  Block(x1,y1, x2,y2, Color[color], Color[fill]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 5(circle)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 50; i++)
        { x1 = (rand() & 0x00FF) * 23 / 25;
          y1 = (rand() & 0x00FF) * 31 / 25;
          r = rand() & 0x007F;
          color = rand() & 0x000F;

	  Circle(x1,y1, r, Color[color]);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();

      Rectangle(0,0, 239,319, Magenta);		// graphic show 6(gradation)
      Rectangle(1,1, 238,318, Magenta);
      Delay_ms(500);
      for(i = 0; i < 30; i++)
        { x1 = (rand() & 0x00FF) * 23 / 25;
          y1 = (rand() & 0x00FF) * 31 / 25;
          r = rand() & 0x007F;
          x2 = x1 + r;
	  y2 = y1 + r;
          color = (rand() & 0x001F) << 11;
 	  color |= (rand() & 0x003F) << 5;
	  color |= rand() & 0x001F;

	  Gradation(x1,y1, x2,y2, color);
          Delay_ms(50);
        }
      Delay_ms(3000);
      TFT_clear_screen();
      Beep();
    }
}

