/* ========================================================================== */
/*	    Exp20_6.c : A/D Converter ADC3 vs ADC2 Differential Input	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i;
  int sum;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"  ADC3 vs ADC2  ");          // display title
  LCD_string(0xC0,"    +0.00[V]    ");

  ADCSRA = 0x87;                                // ADC enable, 125kHz
  ADMUX = 0x7B;                                 // diffrential ADC3 vs ADC2, gain 1
  Delay_us(150);                                // (use left adjust and Vref=AVCC)

  while(1)
    { sum = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD7;                        // start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          sum += (ADCL + ADCH*256)/64;		// convert 10 bit to 16 bit 2's complement
          Delay_ms(1);				// and add A/D result 16 times
        }

      sum = sum/16;                             // divide sum by 16

      LCD_command(0xC4);                        // display in voltage(+/-X.XX)
      LCD_signed_float(sum*5./512.,1,2);	// Volt = sum*50/512

      Delay_ms(500);                            // delay 0.5 sec
    }
}
