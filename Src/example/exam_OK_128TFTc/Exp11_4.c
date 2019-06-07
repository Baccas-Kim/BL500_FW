/* ========================================================================== */
/*		Exp11_4.c : Stop Watch by Timer/Counter1		      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

volatile unsigned int second;
volatile unsigned char minute, hour;
volatile unsigned char run_flag;

void Clear_time(void)                           /* clear time */
{
  second = 0;                                   // clear time
  minute = 0;
  hour = 0;
  LCD_string(0xC0,"  00:00:00.00   ");          // clear display
}

void Set_timer1(void)                           /* initialize Timer1 and OC1A */
{
  TCCR1A = 0x00;                                // CTC mode(4), don't output OC1A
  TCCR1B = 0x0C;                                // 16MHz/256/(1+624) = 100Hz
  TCCR1C = 0x00;
  OCR1A = 624;
  TCNT1 = 0x0000;                               // clear Timer/Counter1

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
}

void Display_time(void)                         /* display time */
{
  LCD_command(0xC2);                            // display hour
  LCD_unsigned_decimal(hour,1,2);
  LCD_command(0xC5);                            // display minute
  LCD_unsigned_decimal(minute,1,2);
  LCD_command(0xC8);                            // display second
  LCD_unsigned_decimal(second/100,1,2);
  LCD_command(0xCB);                            // display 1/100 second
  LCD_unsigned_decimal(second%100,1,2);
}

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  second++;                                     // increment 1/100 second
  if(second == 6000)                            // second = 60 ?
    { second = 0;                               // if yes, second = 0 and minute++
      minute++;
      if(minute == 60)                          // minute = 60 ?
        { minute = 0;                           // if yes, minute = 0 and hour++
          hour++;
          if(hour == 100)                       // hour = 100 ?
            hour = 0;                           // if yes, hour = 0
        }
    }
  Display_time();                               // display time
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode
  Beep();

  TFT_string(6,10, White,Magenta, " 타이머1을 이용한 스톱 워치 ");
  TFT_string( 4,20, White,Blue, " KEY1 ");
  TFT_string(13,20, White,Blue, " KEY2 ");
  TFT_string(22,20, White,Blue, " KEY3 ");
  TFT_string(31,20, White,Blue, " KEY4 ");
  TFT_string(4,23, Cyan,Black, " 시작              삭제     정지");

  LCD_string(0x80," AVR Stop Watch ");          // display title
  Clear_time();                                 // clear time and display
  run_flag = 0;
  Set_timer1();                                 // initialize Timer1 and OC1A

  while(1)
    { switch(Key_input())                       // key input
        { case KEY1 : if(run_flag == 1) break;	// if run_flag=1, ignore KEY1
	              PORTD = 0x10;             // if KEY1, start
                      TCNT1H = 0x00;
                      TCNT1L = 0x00;
		      run_flag =  1;
                      sei();
                      break;
          case KEY3 : if(run_flag == 1) break;	// if run_flag=1, ignore KEY3
	              cli();                    // if KEY3, reset
                      PORTD = 0x40;
                      Clear_time();
                      break;
          case KEY4 : if(run_flag == 0) break;	// if run_flag=0, ignore KEY4
	              cli();                    // if KEY4, stop
                      PORTD = 0x80;
                      run_flag = 0;
                      break;
          default:    break;
        }
    }
}
