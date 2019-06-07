/* ========================================================================== */
/*	     Exp04_3a.c : Integer Number Display on Text LCD Module           */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i, j;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  while(1)
    { LCD_string(0x80," INTEGER = 000  ");      // display 8-bit integer
      LCD_string(0xC0,"= 00H = 00000000");
      Beep();

      for(i = 1; i > 0; i++)			// integer number from 1 to 255
        { LCD_command(0x8B);                    // decimal number
          LCD_unsigned_decimal(i,0,3);
          LCD_command(0xC2);                    // hexadecimal number
          LCD_hexadecimal(i,2);
          LCD_command(0xC8);                    // binary number
          LCD_binary(i,8);
          Delay_ms(1000);
        }

      LCD_string(0x80," MULTI TABLE(1) ");      // display multiplication table(1)
      LCD_string(0xC0,"   0 x 0 = 00   ");
      Beep();

      for(i = 2; i <= 9; i++)
        for(j = 1; j <= 9; j++)
          { LCD_command(0xC3);			// display multiplicand
	    LCD_data(i+'0');
            LCD_command(0xC7);			// display multiplier
	    LCD_data(j+'0');
            LCD_command(0xCB);			// display multiplication
	    LCD_unsigned_decimal(i*j,0,2);
            Delay_ms(1000);
          }

      LCD_string(0x80," MULTI TABLE(2) ");      // display multiplication table(2)
      LCD_string(0xC0," 00 x 00 = 0000 ");
      Beep();

      for(i = 10; i <= 90; i += 10)
        for(j = 10; j <= 90; j += 10)
          { LCD_command(0xC1);			// display multiplicand
	    LCD_unsigned_decimal(i,0,2);
            LCD_command(0xC6);			// display multiplier
	    LCD_unsigned_decimal(j,0,2);
            LCD_command(0xCB);			// display multiplication
	    LCD_unsigned_decimal(i*j,0,4);
            Delay_ms(1000);
          }
    }
}

