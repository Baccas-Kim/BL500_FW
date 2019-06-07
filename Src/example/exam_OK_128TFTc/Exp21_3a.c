/* ========================================================================== */
/*	Exp21_3a.c : D/A Converter Sinusoidal Wave Output using Interrupt     */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

unsigned char i;
unsigned char sin_table[100];			// triangular wave data table

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  PORTA = sin_table[i];				// output data to D/A
  i++;
  if(i == 100)
    i = 0;
}

int main(void)
{
  float x;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80," DAC0800 by Int.");          // display title
  LCD_string(0xC0,"Sinusoidal Wave ");

  for(i = 0; i <= 99; i++)			// calculate sinusoidal wave data table
    { x = 360.*(float)i/100.;
      sin_table[i] = 0x80 + (127.*sin(M_PI*x/180.) + 0.5);
    }

  TCCR1A = 0x00;                                // CTC mode(4), don't output OC1A
  TCCR1B = 0x0A;                                // 16MHz/8/(1+19) = 100kHz
  TCCR1C = 0x00;
  OCR1A = 19;
  TCNT1 = 0x0000;                               // clear Timer/Counter1

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
  sei();                                        // global interrupt enable

  i = 0;

  while(1);                                     // wait interrupt
}

