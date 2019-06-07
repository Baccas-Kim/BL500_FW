/* ============================================================================ */
/*	             Exp02_2a.c : Basic C Program (2)				*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display logo

  while(1)
    { LCD_string(0xC0,"   ATmega128A   ");      // display message 1
      Delay_ms(1000);
      LCD_string(0xC0,"   2010/12/05   ");      // display message 2
      Delay_ms(1000);
    }
}

