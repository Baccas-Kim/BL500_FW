/* ========================================================================== */
/*		Exp13_2.c : PWM Output by OC3A Pin			      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i;
  unsigned int duty;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  LCD_string(0x80,"Period =  1250*2");          // display title
  LCD_string(0xC0,"Width  =  0000*2");

  TCCR3A = 0x82;                                // Phase Correct PWM mode(10)
  TCCR3B = 0x13;
  TCCR3C = 0x00;
  ICR3 = 1250;                                  // f(PWM) = 16MHz/64/2/1250 = 100Hz

  while(1)
    { for(i = 0,duty = 0; i < 6; i++, duty += 250)
        { OCR3A = duty;                         // set duty ratio = duty/1250
          LCD_command(0xCA);                    // display duty
          LCD_unsigned_decimal(duty,0,4);
          Beep();
          Delay_ms(5000);
        }
    }
}
