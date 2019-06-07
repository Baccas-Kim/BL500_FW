/* ========================================================================== */
/*		Exp14_1.c : Digital Clock on Text LCD Module		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

volatile unsigned char second, minute, hour, ampm;

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  second++;					// increment second
  if(second == 60)                              // if second = 60, second = 0
    { second = 0;
      minute++;                                 // increment minute
      if(minute == 60)                          // if minute = 60, minute = 0
        { minute = 0;
          hour++;                               // increment hour
          if(hour == 13)                        // if hour = 13, hour = 1
            hour = 1;
          if(hour == 12)                        // if hour = 12, adjust AM/PM
            { if(ampm == 'A') ampm = 'P';
              else            ampm = 'A';
            }
        }
     }

  LCD_command(0xC2);				// display hour
  LCD_unsigned_decimal(hour,1,2);
  LCD_command(0xC5);				// display minute
  LCD_unsigned_decimal(minute,1,2);
  LCD_command(0xC8);				// display second
  LCD_unsigned_decimal(second,1,2);
  LCD_command(0xCB);				// display AM/PM
  LCD_data(ampm);
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80," Digital Clock  ");          // display title
  LCD_string(0xC0,"  12:00:00 AM   ");

  hour = 12;					// initial time
  minute = 0;
  second = 0;
  ampm = 'A';

  TCCR1A = 0x00;                                // CTC mode(4), don't output OC1A
  TCCR1B = 0x0C;                                // 16MHz/256/(1+62499) = 1Hz
  TCCR1C = 0x00;
  OCR1A = 62499;

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
  sei();                                        // global interrupt enable

  while(1);
}
