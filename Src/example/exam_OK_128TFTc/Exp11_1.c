/* ========================================================================== */
/*		Exp11_1.c : Timer/Counter3 Interrupt(1Hz)	              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

ISR(TIMER3_COMPA_vect)				/* OC3A interrupt function */
{
  PORTD = PORTD ^ 0x10;                         // toggle LED1
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"     Timer3     ");          // display title
  LCD_string(0xC0," 1 Hz Interrupt ");

  PORTD = 0x10;                                 // LED1 on

  TCCR3A = 0x00;                                // CTC mode(4), don't output OC3A
  TCCR3B = 0x0C;                                // 16MHz/256/(1+62499) = 1Hz
  TCCR3C = 0x00;
  OCR3A = 62499;
  TCNT3 = 0x0000;                               // clear Timer/Counter3

  ETIFR = 0x10;                                 // clear OC3A interrupt flag
  ETIMSK = 0x10;                                // enable OC3A interrupt
  sei();                                        // global interrupt enable

  while(1);                                     // wait interrupt
}
