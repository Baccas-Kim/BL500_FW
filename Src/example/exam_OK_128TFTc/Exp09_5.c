/* ========================================================================== */
/*	Exp09_5.c : VR1 전압값을 실시간으로 A/D 변환하여 측정한 파형          */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

void TFT_English_pixel(U16 Xpixel,U16 Ypixel, U08 code)	/* write a English ASCII character */
{
  unsigned char data, x, y;
  unsigned int pixel[8][16];

  for(x = 0; x < 8; x++)			// read English ASCII font
    { data = pgm_read_byte(&E_font[code][x]);
      for(y = 0; y < 8; y++)
        { if(data & 0x01) pixel[x][y] = foreground;
	  else            pixel[x][y] = background;
	  data = data >> 1;
	}
    }

  for(x = 0; x < 8; x++)
    { data = pgm_read_byte(&E_font[code][x+8]);
      for(y = 0; y < 8; y++)
	{ if(data & 0x01) pixel[x][y+8] = foreground;
	  else            pixel[x][y+8] = background;
	  data = data >> 1;
	}
    }

  for(y = 0; y < 16; y++)			// write character
    for(x = 0; x < 8; x++)
      TFT_pixel(Xpixel + x, Ypixel + y, pixel[x][y]);
}

void Draw_axis(void)				/* draw x-axis and y-axis */
{
  TFT_string(15,0, White,Blue, " VR1 전압 ");	// display title

  Line(15,215, 310,215, White);			// draw x-axis
  Line(305,210, 310,215, White);
  Line(305,220, 310,215, White);
  Line(70,212, 70,218, White);			// draw x-axis scale
  Line(120,212, 120,218, White);
  Line(170,212, 170,218, White);
  Line(220,212, 220,218, White);
  Line(270,212, 270,218, White);
  TFT_color(Cyan,Black);			// write time scale
  TFT_English_pixel(16,222, '0');
  TFT_English_pixel(66,222, '1');
  TFT_English_pixel(116,222, '2');
  TFT_English_pixel(166,222, '3');
  TFT_English_pixel(216,222, '4');
  TFT_English_pixel(266,222, '5');
  TFT_color(Magenta,Black);
  TFT_English_pixel(288,222, 's');
  TFT_English_pixel(296,222, 'e');
  TFT_English_pixel(304,222, 'c');

  Line(20,220, 20,5, White);			// draw y-axis
  Line(15,10, 20,5, White);
  Line(25,10, 20,5, White);
  Line(17,175, 23,175, White);			// draw y-axis scale
  Line(17,135, 23,135, White);
  Line(17,95, 23,95, White);
  Line(17,55, 23,55, White);
  Line(17,15, 23,15, White);
  TFT_color(Cyan,Black);			// write voltage scale
  TFT_English_pixel(5,208, '0');
  TFT_English_pixel(5,168, '1');
  TFT_English_pixel(5,128, '2');
  TFT_English_pixel(5,88, '3');
  TFT_English_pixel(5,48, '4');
  TFT_color(Magenta,Black);
  TFT_English_pixel(5,8, 'V');
}

volatile unsigned char ADC_buffer[291], end_flag;
volatile unsigned int ADC_count, ADC_result;

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function (20 ms) */
{
  ADCSRA = 0xD5;				// start conversion and clear ADIF
  while((ADCSRA & 0x10) != 0x10);
  ADC_result = ADCL + ADCH*256;			// read A/D result

  ADC_buffer[ADC_count] = (float)ADC_result*200./1024.;

  if(ADC_count != 0)				// draw graph
    Line(20+ADC_count-1,215-ADC_buffer[ADC_count-1], 20+ADC_count,215-ADC_buffer[ADC_count], Magenta);

  if(ADC_count != 290)
    ADC_count++;
  else
    end_flag = 1;
}

int main(void)
{
  unsigned int i;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp09_5.c    ");

  ADCSRA = 0x85;                                // ADC enable, 500kHz
  ADMUX = 0x43;                                 // select ADC3 with AVCC, right adjust
  Delay_us(150);

  while(1)
    { end_flag = 0;
      ADC_count = 0;

      Draw_axis();				// draw x-axis and y-axis

      TCCR1A = 0x00;				// CTC mode(4), don't output OC1A
      TCCR1B = 0x0C;				// 16MHz/256/(1+1249) = 50Hz
      TCCR1C = 0x00;
      OCR1A = 1249;
      TCNT1 = 0x0000;				// clear Timer/Counter1

      TIFR = 0x10;				// clear OC1A interrupt flag
      TIMSK = 0x10;				// enable OC1A interrupt
      sei();					// global interrupt enable

      while(end_flag == 0);			// wait screen graph end

      cli();					// global interrupt disable
      for(i = 1; i <= 290; i++)			// clear old graph
        Line(20+i-1,215-ADC_buffer[i-1], 20+i,215-ADC_buffer[i], Black);
    }
}
