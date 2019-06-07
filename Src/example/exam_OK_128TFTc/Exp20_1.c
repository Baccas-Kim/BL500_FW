/* ========================================================================== */
/*	    Exp20_1.c : A/D Converter ADC3(VR1) Input			      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i;
  unsigned int sum;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"   ADC3 (VR1)   ");          // display title
  LCD_string(0xC0," 0000H = 0.0[V] ");

  ADCSRA = 0x85;                                // ADC enable, 500kHz
  ADMUX = 0x43;                                 // select ADC3 with AVCC, right adjust
  Delay_us(150);

  while(1)
    { sum = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD5;                        // start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          sum += ADCL + ADCH*256;               // add A/D result 16 times
          Delay_ms(1);
        }

      sum = sum >> 4;                           // divide sum by 16

      LCD_command(0xC1);                        // display in HEX
      LCD_hexadecimal(sum,4);
      LCD_command(0xC9);                        // display in voltage(X.X)
      LCD_unsigned_float(sum*5./1024.,1,1);	// Volt = sum*5/1024

      Delay_ms(200);                            // delay 200 ms
    }
}

