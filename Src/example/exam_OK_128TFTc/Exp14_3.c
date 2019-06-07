/* ========================================================================== */
/*		Exp14_3.c : Digital Clock on TFT-LCD Module		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"
#include "OK-128TFT_large.h"

volatile unsigned char second, minute, hour, ampm;

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  second++;					// increment second
  if(second == 60)                              // if second = 60, second = 0
    { second = 0;
      minute++;                                 // increment minute
      if(minute == 60)                          // if minute = 60, minute = 0
        { minute = 0;
          hour++;                               // increment hour
          if(hour == 13)                        // if hour = 13, hour = 1
            hour = 1;
          if(hour == 12)                        // if hour = 12, adjust AM/PM
            { if(ampm == 'A') ampm = 'P';
              else            ampm = 'A';
            }
        }
    }
}

void Display_time(void)			        /* display time */
{
  if(ampm == 'A')				// display AM or PM
    TFT_string_large(5,16, Yellow,Black, "전");
  else
    TFT_string_large(5,16, Yellow,Black, "후");

  TFT_color(Green,Black);
  TFT_xy( 8,16); TFT_unsigned_decimal_large(hour,1,2);	 // display hour
  TFT_xy(11,16); TFT_unsigned_decimal_large(minute,1,2); // display minute
  TFT_xy(14,16); TFT_unsigned_decimal_large(second,1,2); // display second

  TFT_string_large(17,16,Magenta,Black, " ");		// display home position
}

void Cursor_left(void)			        /* go cursor left */
{
  if(Xcursor != 5)  Xcursor -= 3;
  else              Xcursor = 17;

  Display_time();
}

void Cursor_right(void)             		/* go cursor right */
{
  if(Xcursor != 17) Xcursor += 3;
  else              Xcursor = 5;

  Display_time();
}

void Increment(void)	               		/* increment time */
{
  switch(Xcursor)
    { case  5 : if(ampm == 'A')			// in case of AM/PM
                  { ampm = 'P';
                    TFT_string_large(5,16, Yellow,Black, "후");
		  }
                else
		  { ampm = 'A';
                    TFT_string_large(5,16, Yellow,Black, "전");
		  }
                break;
      case  8 : if(hour != 12) hour++;		// in case of hour
                else           hour = 1;
                TFT_color(Green,Black);
		TFT_xy(8,16);
		TFT_unsigned_decimal_large(hour,1,2);
                break;
      case 11 : if(minute != 59) minute++;	// in case of minute
                else             minute = 0;
                TFT_color(Green,Black);
		TFT_xy(11,16);
		TFT_unsigned_decimal_large(minute,1,2);
                break;
      case 14 : if(second != 59) second++;	// in case of second
                else             second = 0;
                TFT_color(Green,Black);
		TFT_xy(14,16);
		TFT_unsigned_decimal_large(second,1,2);
                break;
      default : break;
    }
}

void Decrement(void)	               		/* decrement time */
{
  switch(Xcursor)
    { case  5 : if(ampm == 'A')			// in case of AM/PM
                  { ampm = 'P';
                    TFT_string_large(5,16, Yellow,Black, "후");
		  }
                else
		  { ampm = 'A';
                    TFT_string_large(5,16, Yellow,Black, "전");
		  }
                break;
      case  8 : if(hour != 1) hour--;		// in case of hour
                else          hour = 12;
                TFT_color(Green,Black);
		TFT_xy(8,16);
		TFT_unsigned_decimal_large(hour,1,2);
                break;
      case 11 : if(minute != 0) minute--;	// in case of minute
                else            minute = 59;
                TFT_color(Green,Black);
		TFT_xy(11,16);
		TFT_unsigned_decimal_large(minute,1,2);
                break;
      case 14 : if(second != 0) second--;	// in case of second
                else            second = 59;
                TFT_color(Green,Black);
		TFT_xy(14,16);
		TFT_unsigned_decimal_large(second,1,2);
                break;
      default : break;
    }
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp14_3.c    ");

  TFT_cursor(Magenta);				// cursor on by Magenta color
  Xcursor = 17;
  Ycursor = 16;

  TFT_string(1,8, White,Magenta," 디지털 시계 (타이머1 사용) "); // display title
  TFT_string_large(3,16, Yellow,Black, "오전 ");
  TFT_string_large(8,16, Green, Black, "00:00:00");
  TFT_string(4,28, White,Black, "KEY1 - 커서 좌로 이동");
  TFT_string(4,30, White,Black, "KEY2 - 커서 우로 이동");
  TFT_string(4,32, White,Black, "KEY3 - 시/분/초 증가");
  TFT_string(4,34, White,Black, "KEY4 - 시/분/초 감소");

  hour = 12;					// initial time
  minute = 0;
  second = 0;
  ampm = 'A';

  TCCR1A = 0x00;                                // CTC mode(4), don't output OC1A
  TCCR1B = 0x0C;                                // 16MHz/256/(1+62499) = 1Hz
  TCCR1C = 0x00;
  OCR1A = 62499;

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
  sei();                                        // global interrupt enable

  while(1)
    { switch(Key_input())			// key input
        { case KEY1 : Cursor_left();		// if KEY1, go cursor left
                      break;
          case KEY2 : Cursor_right();		// if KEY2, go cursor right
                      break;
          case KEY3 : Increment();		// if KEY3, increment time
                      break;
          case KEY4 : Decrement();		// if KEY4, decrement time
                      break;
          default   : Display_time();		// display time
	              break;
        }
    }
}
