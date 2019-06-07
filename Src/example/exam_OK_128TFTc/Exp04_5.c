/* ========================================================================== */
/*	      Exp04_5.c : Floating Point Number Multiplication Table	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i, j;
  float x, y;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  while(1)
    { Beep();
      LCD_string(0x80," FLOAT MULTIPLY ");	// display title
      LCD_string(0xC0,"0.0 x 0.0 =00.00");
      Delay_ms(1000);

      x = 0.0;					// initial floating-point number
      for(i = 1; i <= 9; i++)
        { x += 1.1;
          y = 0.0;
          for(j = 1; j <= 9; j++)
            { y += 1.1;
              LCD_command(0xC0);		// display multiplicand
	      LCD_unsigned_float(x,1,1);
              LCD_command(0xC6);		// display multiplier
	      LCD_unsigned_float(y,1,1);
              LCD_command(0xCB);		// display multiplication
	      LCD_unsigned_float(x*y,2,2);
              Delay_ms(1000);
            }
        }
    }
}

