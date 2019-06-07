/* ========================================================================== */
/*	    Exp20_7.c : Analog Comparator - VR1 vs 1.23V		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"  Analog Comp.  ");          // display title
  LCD_string(0xC0,"  VR1 < 1.23V   ");

  ACSR = 0x43;                                  // +input = 1.23V
  SFIOR = 0x08;                                 // use ADC input pin
  ADCSRA = 0x00;                                // ADEN = 0
  ADMUX = 0x03;                                 // -input = ADC3

  while(1)
    { if((ACSR & 0x20) == 0x20)                 // if ACO=1, ADC3 < 1.23V
        { PORTD = 0x80;                         //   LED4 on
          LCD_command(0xC6);                    //   display "<"
          LCD_data('<');
        }
      else                                      // if ACO=0, ADC3 > 1.23V
        { PORTD = 0x10;                         //   LED1 on
          LCD_command(0xC6);                    //   display ">"
          LCD_data('>');
        }

      Delay_ms(100);                            // delay 100 ms
    }
}

