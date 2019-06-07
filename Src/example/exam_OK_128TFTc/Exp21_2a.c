/* ========================================================================== */
/*	Exp21_2a.c : D/A Converter Triangular Wave Output using Interrupt     */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

unsigned char i;
unsigned char tri_table[100];			// triangular wave data table

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  PORTA = tri_table[i];				// output data to D/A
  i++;
  if(i == 100)
    i = 0;
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80," DAC0800 by Int.");          // display title
  LCD_string(0xC0,"Triangular Wave ");

  for(i = 0; i <= 49; i++)			// calculate triangular wave data table
    tri_table[i] = (i*255 + 25)/50;
  for(i = 50; i <= 99; i++)
    tri_table[i] = 2*255 - (i*255 + 25)/50;

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

