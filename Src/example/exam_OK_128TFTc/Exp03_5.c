/* ============================================================================ */
/*		Exp03_5.c : SRAM Variables					*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

unsigned char table1[32] = { };			// table 1 with SRAM variables
unsigned char table2[32] = { };			// table 2 with SRAM variables

int main(void)
{
  unsigned char i, value;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp03_5.c    ");

  value = 0x00;					// make SRAM table 1
  for(i = 0; i < 32; i++)
    { table1[i] = value;
      value++;
    }

  value = 0xE0;					// make SRAM table 2
  for(i = 0; i < 32; i++)
    { table2[i] = value;
      value++;
    }

  while(1)
    { TFT_string(12,8, White,Magenta, " SRAM Table(1) ");
      TFT_xy(0,12);
      TFT_color(Cyan,Black);
      for(i = 0; i < 32; i++)			// output SRAM table 1
        { TFT_0x_hexadecimal(table1[i], 2);
          TFT_English(' ');
        }
      Beep();
      Delay_ms(3000);

      TFT_string(12,8, White,Magenta, " SRAM Table(2) ");
      TFT_xy(0,12);
      TFT_color(Yellow,Black);
      for(i = 0; i < 32; i++)			// output SRAM table 2
        { TFT_0x_hexadecimal(table2[i], 2);
          TFT_English(' ');
        }
      Beep();
      Delay_ms(3000);
    }
}
