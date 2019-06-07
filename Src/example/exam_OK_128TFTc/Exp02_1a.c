/* ============================================================================ */
/*	             Exp02_1a.c : Basic C Program (1)				*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit

  DDRD = 0xF0;					// initialize PORTD = output

  while(1)
    { PORTD = 0x50;				// LED1, 3 on
      Delay_ms(500);
      PORTD = 0xA0;				// LED2, 4 on
      Delay_ms(500);
    }
}

