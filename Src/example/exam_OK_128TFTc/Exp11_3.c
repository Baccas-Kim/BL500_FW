/* ========================================================================== */
/*		Exp11_3.c : Timer/Counter1 and 3 Multi-Interrupt	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

volatile unsigned int OC1A_count, OC3A_count;   // interrupt counter

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  PORTD = PORTD ^ 0x10;                         // toggle LED1
  OC1A_count++;                                 // increment OC1A interrupt count
}

ISR(TIMER3_COMPA_vect)				/* OC3A interrupt function */
{
  PORTD = PORTD ^ 0x20;                         // toggle LED2
  OC3A_count++;                                 // increment OC3A interrupt count
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80," Timer1 = 00000 ");          // display title
  LCD_string(0xC0," Timer3 = 00000 ");

  PORTD = 0x30;                                 // LED1 and 2 on
  OC1A_count = 0;                               // clear OC1A interrupt counter
  OC3A_count = 0;                               // clear OC3A interrupt counter

  TCCR1A = 0x40;                                // CTC mode(4), output OC1A
  TCCR1B = 0x0C;                                // 16MHz/256/(1+62499) = 1Hz
  TCCR1C = 0x00;
  OCR1A = 62499;
  TCNT1 = 0x0000;                               // clear Timer/Counter1

  TCCR3A = 0x40;                                // CTC mode(4), output OC3A
  TCCR3B = 0x0C;                                // 16MHz/256/(1+12499) = 5Hz
  TCCR3C = 0x00;
  OCR3A = 12499;
  TCNT3 = 0x0000;                               // clear Timer/Counter3

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  ETIFR = 0x10;                                 // clear OC3A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
  ETIMSK = 0x10;                                // enable OC3A interrupt
  sei();                                        // global interrupt enable

  while(1)                                      // wait interrupt
    { LCD_command(0x8A);                        // display OC1A interrupt count
      LCD_unsigned_decimal(OC1A_count,0,5);
      LCD_command(0xCA);                        // display OC3A interrupt count
      LCD_unsigned_decimal(OC3A_count,0,5);
    }
}
