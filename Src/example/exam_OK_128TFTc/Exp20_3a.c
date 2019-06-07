/* ========================================================================== */
/*	Exp20_3a.c : A/D Converter ADC2(LM35DZ) Input by Moving Average	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i, ADC_count = 0, ADC_start_flag = 0;
  unsigned int result, ADC_buffer[16];

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"  ADC2(LM35DZ)  ");          // display title
  LCD_string(0xC0," 0000H = 00.0");
  LCD_data(0xDF);
  LCD_data('C');
  LCD_data(' ');

  ADCSRA = 0x85;                                // ADC enable, 500kHz
  ADMUX = 0x42;                                 // select ADC2 with AVCC, right adjust
  Delay_us(150);

  while(1)
    { ADCSRA = 0xD5;				// start conversion and clear ADIF
      while((ADCSRA & 0x10) != 0x10);
      result = ADCL + ADCH*256;			// read A/D result

      if(ADC_start_flag == 1)			// if not start, store result
        { ADC_buffer[ADC_count] = result;
          ADC_count++;
          ADC_count = ADC_count % 16;
        }
      else
        { for(i = 0; i < 16; i++)		// if start new, initialize buffer 
            ADC_buffer[i] = result;
          ADC_start_flag = 1;
        }

      result = 0;
      for(i = 0; i < 16; i++)			// calculate moving average of buffer
        result += ADC_buffer[i];
      result = result >> 4;

      LCD_command(0xC1);                        // display in HEX
      LCD_hexadecimal(result,4);
      LCD_command(0xC9);                        // display in temperature(XX.X)
      LCD_unsigned_float(result*100./1024.,2,1);// Temp = result*100/1024

      Delay_ms(500);				// delay 0.5 sec
    }
}

