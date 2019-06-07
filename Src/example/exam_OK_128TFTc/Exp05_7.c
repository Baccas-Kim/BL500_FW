/* ========================================================================== */
/*	Exp05_7.c : INT7 Interrupt Count Program - No Debouncing	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"

unsigned char int_count = 0;			// interrupt count

ISR(INT7_vect)					/* INT7 interrupt function */
{
  int_count++;					// increment interrupt count
  LCD_command(0xC6);				// display interrupt count
  LCD_unsigned_decimal(int_count,0,3);
}

int main(void)
{
  unsigned char i,LED;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"   INT7 Count   ");		// display title
  LCD_string(0xC0,"      000       ");

  EICRB = 0x80;                                 // INT7 = falling edge trigger
  EIMSK = 0x80;					// enable INT7
  EIFR = 0xFF;                                  // clear interrupt flag
  sei();                                        // global interrupt enable

  while(1)
    { for(i = 1, LED = 0x10; i <= 4; i++)	// shift from LED1 to LED4
        { PORTD = LED;
          Delay_ms(200);
          LED <<= 1;
        }
    }
}
