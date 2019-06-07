/* ========================================================================== */
/*		Exp05_1.c : Key Input - No Debouncing		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char key;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"   KEY INPUT    ");          // display title
  LCD_string(0xC0,"PORTG = 11110000");

  while(1)
    { LCD_command(0xC8);			// cursor position
      key = PING & 0x0F;                        // read key input
      LCD_binary(key,8);			// display binary data
      PORTD = (key << 4) ^ 0xF0;		// display LED
    }
}
