/* ========================================================================== */
/*		Exp11_2.c : Timer/Counter0 Interrupt(250Hz)	              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

volatile unsigned char interrupt_count;         // interrupt counter

ISR(TIMER0_COMP_vect)				/* OC0 interrupt function */
{
  PORTD = PORTD ^ 0x20;                         // toggle LED2
  interrupt_count--;
  if(interrupt_count == 0)                      // 250th interrupt ?
    { interrupt_count = 250;                    // if yes, toggle LED1
      PORTD = PORTD ^ 0x10;
    }
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"     Timer0     ");          // display title
  LCD_string(0xC0,"250 Hz Interrupt");

  PORTB = 0x30;                                 // LED1 and 2 on
  interrupt_count = 250;                        // interrupt counter

  TCCR0 = 0x0E;                                 // CTC mode(2), don't output OC0
  ASSR = 0x00;                                  // use internal clock
  OCR0 = 249;                                   // 16MHz/256/(1+249) = 250Hz
  TCNT0 = 0x00;                                 // clear Timer/Counter0

  TIFR = 0x02;                                  // clear OC0 interrupt flag
  TIMSK = 0x02;                                 // enable OC0 interrupt
  sei();                                        // global interrupt enable

  while(1);                                     // wait interrupt
}

