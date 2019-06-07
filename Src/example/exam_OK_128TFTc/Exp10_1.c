/* ========================================================================== */
/*		Exp10_1.c : Touch Screen Input			              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp10_1.c    ");

  Initialize_touch_screen();			// initialize touch screen

  Rectangle(0,0, 239,319, Green);		// draw screen outline
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string( 1, 1, Yellow,Black, "(0,0)");
  TFT_string(22, 1, Yellow,Black, "(239,0)");
  TFT_string( 1,37, Yellow,Black, "(0,319)");
  TFT_string(20,37, Yellow,Black, "(239,319)");

  TFT_string( 3, 6, Green,  Black, "************************");
  if(touch_flag == 'A')
    TFT_string( 3, 8, Magenta,Black, " 터치 스크린  (ADS7846) ");
  else
    TFT_string( 3, 8, Magenta,Black, " 터치 스크린  (TSC2003) ");
  TFT_string( 3,10, Green,  Black, "************************");
  TFT_string( 3,14, Green,  Black, "  원시 데이터");
  TFT_string(16,14, Magenta,Black,              " x = 0000  ");
  TFT_string( 3,16, Magenta,Black, "              y = 0000  ");
  TFT_string( 3,20, Green,  Black, "  픽셀 데이터");
  TFT_string(16,20, Magenta,Black,              " x = 000   ");
  TFT_string( 3,22, Magenta,Black, "              y = 000   ");

  TFT_string(10,28, White,Blue, "비프음 1회");	// display menu icon
  Rectangle(76,220, 164,244, Magenta);
  TFT_string(10,32, White,Blue, "비프음 3회");
  Rectangle(76,252, 164,276, Magenta);

  while(1)
    { Touch_screen_input();			// input touch screen
      TFT_color(Cyan,Black);
      TFT_xy(21,14);				// display 12-bit original X data
      TFT_unsigned_decimal(x_12bit,0,4);
      TFT_xy(21,16);				// display 12-bit original Y data
      TFT_unsigned_decimal(y_12bit,0,4);
      TFT_xy(21,20);				// display pixel X data
      TFT_unsigned_decimal(x_touch,0,3);
      TFT_xy(21,22);				// display pixel Y data
      TFT_unsigned_decimal(y_touch,0,3);

      if((x_touch >= 76) && (x_touch <= 164) && (y_touch >= 220) && (y_touch <= 244))
        { Beep();
          Delay_ms(200);
        }
      else if((x_touch >= 76) && (x_touch <= 164) && (y_touch >= 252) && (y_touch <= 276))
        { Beep_3times();
          Delay_ms(200);
	}    

      Delay_ms(20);
    }
}
