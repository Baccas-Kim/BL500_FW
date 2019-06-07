/* ========================================================================== */
/*		Exp07_3.c : Visiting Count Program(1)			      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <stdlib.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  unsigned int random, total = 0;
  unsigned char key, i, j, count, visit_flag, x, y, table[30][20];

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp07_3.c    ");

  TCCR1A = 0x00;				// CTC mode(4), don't output OC1A
  TCCR1B = 0x0A;				// 16MHz/8/(1+199) = 10kHz
  TCCR1C = 0x00;
  OCR1A = 199;
  TCNT1 = 0;

START:
  TFT_string(5,10, White,Magenta, " Visiting Count (1) "); // wait KEY1 to start
  TFT_string(3,20, Cyan,   Black, " Press KEY1 to start ! ");
  TFT_string(3,30, White,  Black, " (Total Counts = ");
  TFT_unsigned_decimal(total,0,5);
  TFT_string(25,30, White,Black, ") ");
  Beep();
  total = 0;
  key = 0;
  while(key != KEY1) key = Key_input();

  srand(TCNT1);					// initialize random number

  TFT_color(White,Black);
  for(j = 0; j <= 19; j++)			// clear visiting room
    for(i = 0; i <= 29; i++)
      { table[i][j] = 0;
        TFT_xy(i, 2*j);
        TFT_English('0');
      }
  visit_flag = 0;

  x = rand() & 0x001F;				// starting position
  if(x > 29) x = 29;
  y = rand() & 0x000F;

  TFT_color(Blue,Black);
  while(visit_flag == 0)
    { random = rand();				// get random number

      if(random <= 0x1FFF)			// 0x0000 - 0x1FFF
        { if(x != 29)
            { x++; table[x][y] += 1; } }
      else if(random <= 0x3FFF)			// 0x2000 - 0x3FFF
        { if(x != 0)
            { x--; table[x][y] += 1; } }
      else if(random <= 0x5FFF)			// 0x4000 - 0x5FFF
        { if(y != 19)
            { y++; table[x][y] += 1; } }
      else					// 0x6000 - 0x7FFF
        { if(y != 0)
            { y--; table[x][y] += 1; } }

      count = table[x][y];			// display visiting count
      if(count >= 62) count = '*';
      else if(count >= 36) count = count - 36 + 'a';
      else if(count >= 10) count = count - 10 + 'A';
      else count = count + '0';
      TFT_xy(x, 2*y);
      TFT_English(count);

      visit_flag = 1;				// check end
      for(j = 0; j <= 19; j++)
        for(i = 0; i <= 29; i++)
          if(table[i][j] == 0)
            visit_flag = 0;

      total++;					// increment total count
      Delay_ms(10);
    }

  goto START;					// if complete, go next stage
}

