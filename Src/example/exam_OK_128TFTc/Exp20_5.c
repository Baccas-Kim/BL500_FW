/* ========================================================================== */
/*	    Exp20_5.c : A/D Converter ADC1(LM35DZ) Input		      */
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

  LCD_string(0x80,"  ADC1(LM35DZ)  ");          // display title
  LCD_string(0xC0," 0000H = 00.0");
  LCD_data(0xDF);
  LCD_data('C');
  LCD_data(' ');

  ADCSRA = 0x87;                                // ADC enable, 125kHz
  ADMUX = 0xF0;                                 // diffrential ADC1 vs ADC0(GND), gain 1
  Delay_us(150);				// (use left adjust and Vref=2.56V)

  while(1)
    { sum = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD7;                        // start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          sum += (ADCL + ADCH*256)/64;		// convert 10 bit to 16 bit 2's complement
          Delay_ms(1);				// and add A/D result 16 times
        }

      sum = sum/16;                             // divide sum by 16
      sum = fabs(sum);				// calculate absolute value

      LCD_command(0xC1);                        // display in HEX
      LCD_hexadecimal(sum,4);
      LCD_command(0xC9);                        // display in temperature(XX.X)
      LCD_unsigned_float(sum*256./512.,2,1);	// Temp = sum*256/512

      Delay_ms(500);				// delay 0.5 sec
    }
}

