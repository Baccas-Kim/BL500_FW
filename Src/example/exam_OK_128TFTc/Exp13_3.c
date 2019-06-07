/* ========================================================================== */
/*		Exp13_3.c : Variable-Frequency, Variable-Duty PWM	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

int main(void)
{
  unsigned int period, duty;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"Period =  1250*2");          // display title
  LCD_string(0xC0,"Width  =   125*2");

  TCCR3A = 0x21;                                // Phase and Frequency Correct PWM mode(9)
  TCCR3B = 0x13;                                // use OC3B
  TCCR3C = 0x00;
  OCR3A = 1250;                                 // default PWM frequency(period)
  period = 1250;                                // f(PWM) = 16MHz/64/2/1250 = 100Hz
  OCR3B = 125;                                  // default PWM duty ratio
  duty = 125;                                   // D(PWM) = 125/1250

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : if(period != 1250)		// if KEY1, increment period
                        { period += 125;
                          OCR3A = period;
                          LCD_command(0x8A);
                          LCD_unsigned_decimal(period,0,4);
			}
                      break;
	  case KEY2 : if((period != 125) && (period != duty))	// if KEY2, decrement period
                        { period -= 125;
                          OCR3A = period;
                          LCD_command(0x8A);
                          LCD_unsigned_decimal(period,0,4);
                        }
                      break;
          case KEY3 : if(duty != period)		// if KEY3, increment duty ratio
                        { duty += 125;
                          OCR3B = duty;
                          LCD_command(0xCA);
                          LCD_unsigned_decimal(duty,0,4);
                        }
                      break;
	  case KEY4 : if(duty != 0)             // if KEY4, decrement duty ratio
                        { duty -= 125;
                          OCR3B = duty;
                          LCD_command(0xCA);
                          LCD_unsigned_decimal(duty,0,4);
                        }
                      break;
          default:    break;
        }
    }
}

