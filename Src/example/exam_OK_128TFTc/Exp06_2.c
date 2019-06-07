/* ========================================================================== */
/*		Exp06_2.c : Watchdog Timer (1)				      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/wdt.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char i;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0," Watchdog Timer ");
  sbi(PORTC,2);                                 // buzzer on
  PORTD = 0x10;                                 // LED1 on
  Delay_ms(200);
  PORTD = 0x20;                                 // LED2 on
  Delay_ms(200);
  PORTD = 0x40;                                 // LED3 on
  Delay_ms(200);
  PORTD = 0x80;                                 // LED4 on
  Delay_ms(200);
  PORTD = 0x00;                                 // all LED off
  cbi(PORTC,2);                                 // buzzer off
  Delay_ms(1000);

  wdt_enable(WDTO_1S);                          // enable Watchdog Timer(0.9 sec)

  while(1)
    { LCD_string(0x80,"Press KEY1/KEY4!");	// display command
      LCD_string(0xC0,"                ");

      switch(Key_input())                       // key input
        { case KEY1 : LCD_string(0xC0,"   Blink LED1   ");      // KEY1
                      for(i = 0; i < 20; i++)
                        { wdt_reset();
                          PORTD ^= 0x10;
                          Delay_ms(100);
                        }
                      LCD_string(0xC0,"                ");
                      break;
          case KEY4 : LCD_string(0xC0,"   Blink LED4   ");      // KEY4
                      for(i = 0; i < 10; i++)
                        { wdt_reset();
                          PORTD ^= 0x80;
                          Delay_ms(500);
                        }
                      LCD_string(0xC0,"                ");
                      break;
          default:    wdt_reset();
                      break;
        }
    }
}
