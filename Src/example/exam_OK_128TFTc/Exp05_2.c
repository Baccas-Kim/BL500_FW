/* ========================================================================== */
/*		Exp05_2.c : Key Input - Debouncing		              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned char key;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"   KEY INPUT    ");          // display title
  LCD_string(0xC0,"Press KEY1-KEY4!");

  while(1)
    { key = Key_input();                       // key input
      switch(key)
        { case KEY1 : PORTD = (key << 4) ^ 0xF0;
                      LCD_string(0xC0,"  KEY1 is OK !  ");
                      break;
          case KEY2 : PORTD = (key << 4) ^ 0xF0;
                      LCD_string(0xC0,"  KEY2 is OK !  ");
                      break;
          case KEY3 : PORTD = (key << 4) ^ 0xF0;
                      LCD_string(0xC0,"  KEY3 is OK !  ");
                      break;
          case KEY4 : PORTD = (key << 4) ^ 0xF0;
                      LCD_string(0xC0,"  KEY4 is OK !  ");
                      break;
          default:    break;
        }
    }
}
