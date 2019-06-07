/* ============================================================================ */
/*			Exp03_1.c : Bit Access					*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit

  DDRD = 0xF0;					// initialize PORTD = output

  while(1)
    { PORTD = (1 << 6) | (1 << 4);		// LED1, 3 on
      Delay_ms(500);
      PORTD = _BV(7) | _BV(5);                  // LED2, 4 on
      Delay_ms(500);
    }
}

