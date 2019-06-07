/* ========================================================================== */
/*	    Exp21_4.c : Variable D/A Voltage by Key Input		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

void LCD_voltage(unsigned char number)          /* display D/A voltage */
{
  unsigned char i;
  unsigned int j;

  LCD_command(0xC2);                            // display in HEX
  LCD_hexadecimal(number,2);

  LCD_command(0xC7);                            // display sign of voltage
  if(number >= 0x80)                            //   and calculate absolute value
    { LCD_data('+');
      i = number - 0x80;
    }
  else
    { LCD_data('-');
      i = 0x80 - number;
    }

  j = i*100/128;                                // voltage = (absolute/128)*10
  i = j/100;
  if(i == 0) LCD_data(' ');                     // 10^1
  else       LCD_data(i + '0');
  i = j%100;
  LCD_data(i/10 + '0');                         // 10^0
  LCD_data('.');
  LCD_data(i%10 + '0');                         // 10^-1
}

int main(void)
{
  unsigned char voltage;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"   D/A Output   ");          // display title
  LCD_string(0xC0,"0x80 = + 0.0 [V]");

  PORTA = 0x80;                                 // initial D/A output value
  voltage = 0x80;

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : if(voltage != 0xFF)       // if KEY1, increment by 0x01
                        { voltage++;
                          LCD_voltage(voltage);
                          PORTA = voltage;
			}
                      break;
	  case KEY2 : if(voltage != 0x00)       // if KEY2, decrement by 0x01
                        { voltage--;
                          LCD_voltage(voltage);
                          PORTA = voltage;
			}
                      break;
          case KEY3 : if(voltage <= 0xEF)       // if KEY3, increment by 0x10
                        { voltage += 0x10;
                          LCD_voltage(voltage);
                          PORTA = voltage;
			}
                      break;
          case KEY4 : if(voltage >= 0x10)       // if KEY4, decrement by 0x10
                        { voltage -= 0x10;
                          LCD_voltage(voltage);
                          PORTA = voltage;
			}
                      break;
          default:    break;
        }
    }
}
