/* ========================================================================== */
/*		Exp06_3.c : Watchdog Timer (2)				      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
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

  WDTCR = 0x18;                                 // enable Watchdog Timer(0.22 sec)
  WDTCR = 0x0C;

  PORTD = 0x40;                                 // LED3 on

  while(1)
    { LCD_string(0x80,"Press KEY1/KEY4!");	// display command
      LCD_string(0xC0,"(KEY3/2=ON/OFF) ");

      switch(Key_input())                       // key input
        { case KEY1 : LCD_string(0xC0,"   Blink LED1   ");      // KEY1
                      for(i = 0; i < 20; i++)
                        { asm volatile(" WDR ");
                          PORTD ^= 0x10;
                          Delay_ms(100);
                        }
                      break;
          case KEY4 : LCD_string(0xC0,"   Blink LED4   ");      // KEY4
                      for(i = 0; i < 10; i++)
                        { asm volatile(" WDR ");
                          PORTD ^= 0x80;
                          Delay_ms(500);
                        }
                      break;
          case KEY2 : WDTCR = 0x18;                             // KEY2
                      WDTCR = 0x00;
                      PORTD = 0x20;
                      break;
          case KEY3 : WDTCR = 0x18;                             // KEY3
                      WDTCR = 0x0C;
                      PORTD = 0x40;
                      break;
          default:    asm volatile(" WDR ");
                      break;
        }
    }
}
