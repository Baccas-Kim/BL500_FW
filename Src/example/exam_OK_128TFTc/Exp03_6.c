/* ============================================================================ */
/*		Exp03_6.c : Flash Memory Constants				*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

prog_uchar table1[32] = {			// table 1 with constants
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F };

prog_uchar table2[32] = {			// table 2 with constants
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
    0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
    0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF };

int main(void)
{
  unsigned char i;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp03_6.c    ");

  while(1)
    { TFT_string(8,8, White,Magenta, " Flash Memory Table(1) ");
      TFT_xy(0,12);
      TFT_color(Cyan,Black);
      for(i = 0; i < 32; i++)			// output constant table 1
        { TFT_0x_hexadecimal(pgm_read_byte(&table1[i]), 2);
          TFT_English(' ');
        }
      Beep();
      Delay_ms(3000);

      TFT_string(8,8, White,Magenta, " Flash Memory Table(2) ");
      TFT_xy(0,12);
      TFT_color(Yellow,Black);
      for(i = 0; i < 32; i++)			// output constant table 2
        { TFT_0x_hexadecimal(pgm_read_byte(&table2[i]), 2);
          TFT_English(' ');
        }
      Beep();
      Delay_ms(3000);
    }
}
