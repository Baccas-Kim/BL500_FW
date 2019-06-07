/* ========================================================================== */
/*	Exp07_1.c : Baisc English/ASCII Character Display on TFT-LCD Module   */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  unsigned char x, y, code;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp07_1.c    ");

  while(1)
    { TFT_color_screen(White);  		// display full color(white) 
      TFT_portrait();				// portrait mode
      Beep();

      TFT_string( 1, 2,White, Black," OK-128TFT ASCII Characters ");
      TFT_string( 7, 4,White, Black,      " (White Screen) ");

      TFT_string( 3,12,White,  Blue,   " Foreground White Color ");
      TFT_string( 2,14,White,  Blue,  " on Background Blue Color ");

      TFT_string( 4,18,Red,    Cyan,    " Foreground Red Color ");
      TFT_string( 2,20,Red,    Cyan,  " on Background Cyan Color ");

      TFT_string( 2,24,Yellow,Black,  " Foreground Yellow Color ");
      TFT_string( 1,26,Yellow,Black, " on Background Black Color ");

      TFT_string( 3,30,Blue,Transparent,   " Foreground Blue Color ");
      TFT_string( 1,32,Blue,Transparent, " on Transparent Background ");
      Delay_ms(5000);

      TFT_color_screen(White);  		// display full color(white) 
      TFT_landscape();				// landscape mode
      Beep();

      TFT_string( 6, 2,White, Black," OK-128TFT ASCII Characters ");
      TFT_string(12, 4,White, Black,      " (White Screen) ");

      TFT_string( 0,10,White,  Blue,"Foreground White Color / Blue Background");

      TFT_string( 1,15,Red,    Cyan, "Foreground Red Color / Cyan Background");

      TFT_string( 0,20,Yellow,Black,"Foreground Yellow Color/Black Background");

      TFT_string( 2,25,Blue,Transparent, " Foreground Blue Color / Transparent ");
      Delay_ms(5000);

      TFT_clear_screen();  			// clear screen 
      TFT_portrait();				// portrait mode
      Beep();

      TFT_string( 6, 2,White,Magenta," ASCII Code Table ");
      code = 0x00;
      for(y = 6; y <= 36; y += 2)
        { TFT_xy(4,y); TFT_color(Green,Black);
	  TFT_0x_hexadecimal(code,2);
          TFT_English(' '); TFT_English(':');
	  TFT_color(Cyan,Black);

          for(x = 0; x <= 7; x++)
            { TFT_English(' '); 
	      TFT_English(code++);
            }
        }
      Delay_ms(5000);
    }
}
