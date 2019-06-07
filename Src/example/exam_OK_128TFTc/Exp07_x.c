/* ========================================================================== */
/*		Exp07_x.c : Baisc Color Display on TFT-LCD Module	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

unsigned int Color[8] =				// 8-color data
   { White, Black, Red, Green, Blue, Yellow, Cyan, Magenta };

void TFT_full_color(void)			/* display full color screen */
{
  TFT_portrait();				// portrait mode

  TFT_color_screen(White);  			// display white screen
  TFT_string(10,1,Black,Transparent,"��� ȭ��");
  Delay_ms(1000);
  TFT_color_screen(Red);  			// display red screen
  TFT_string(10,1,Green,Transparent,"���� ȭ��");
  Delay_ms(1000);
  TFT_color_screen(Green);  			// display green screen
  TFT_string(10,1,Red,  Transparent,"��� ȭ��");
  Delay_ms(1000);
  TFT_color_screen(Blue);  			// display blue screen  
  TFT_string(10,1,Cyan, Transparent,"û�� ȭ��");
  Delay_ms(1000);
}

void TFT_8color_portrait(void)			/* display portrait 8-color bar */
{
  unsigned int i, j, k;

  TFT_portrait();				// portrait mode
  TFT_GRAM_address(0,0);

  for(i = 0; i < 8; i++)			// display color bar
    for(j = 0; j < 40; j++)
      for(k = 0; k < 240; k++)
        TFT_data(Color[i]);

  TFT_string(13, 1,Black,Transparent,"���");	// display color name
  TFT_string(13, 7,White,Transparent,"���");
  TFT_string(13,11,Green,Transparent,"����");
  TFT_string(13,16,Red,  Transparent,"���");
  TFT_string(13,21,Cyan, Transparent,"û��");
  TFT_string(13,26,Black,Transparent,"���");
  TFT_string(13,31,Red,  Transparent,"��û");
  TFT_string(13,36,White,Transparent,"��ȫ");
  Delay_ms(3000);
}

void TFT_8color_landscape(void)			/* display landscape 8-color bar */
{
  unsigned int i, j, k;

  TFT_landscape();				// landscape mode
  TFT_GRAM_address(0,0);

  for(i = 0; i < 8; i++)			// display color bar
    for(j = 0; j < 30; j++)
      for(k = 0; k < 320; k++)
        TFT_data(Color[i]);

  TFT_portrait();				// portrait mode
  TFT_string(28, 1,Black,Transparent,"��");	// display color name
  TFT_string(28, 3,Black,Transparent,"��");
  TFT_string(24, 1,White,Transparent,"��");
  TFT_string(24, 3,White,Transparent,"��");
  TFT_string(20, 1,Green,Transparent,"��");
  TFT_string(20, 3,Green,Transparent,"��");
  TFT_string(16, 1,Red,  Transparent,"��");
  TFT_string(16, 3,Red,  Transparent,"��");
  TFT_string(13, 1,Cyan, Transparent,"û");
  TFT_string(13, 3,Cyan, Transparent,"��");
  TFT_string( 9, 1,Black,Transparent,"��");
  TFT_string( 9, 3,Black,Transparent,"��");
  TFT_string( 5, 1,Red,  Transparent,"��");
  TFT_string( 5, 3,Red,  Transparent,"û");
  TFT_string( 1, 1,White,Transparent,"��");
  TFT_string( 1, 3,White,Transparent,"ȫ");
  Delay_ms(3000);
}

void TFT_8color_gray(void)			/* display landscape 8-color gray */
{
  unsigned int i, j, k, color;

  TFT_landscape();				// landscape mode
  TFT_GRAM_address(0,0);

  for(j = 0; j < 30; j++)			// white gray
    { for(k = 0; k < 320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6) + i;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// red gray
    { for(k = 0; k < 320; k++)
        { color = (0x1F - k/13) << 11;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// green gray
    { for(k = 0; k < 320; k++)
        { color = (0x1F - k/13) << 6;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// blue gray
    { for(k = 0; k < 320; k++)
        { color = 0x1F - k/13;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// yellow gray
    { for(k = 0; k < 320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6);
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// cyan gray
    { for(k = 0; k < 320; k++)
        { i = 0x1F - k/13;
	  color = (i << 6) + i;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// magenta gray
    { for(k = 0; k < 320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + i;
          TFT_data(color);
        }
    }

  for(j = 0; j < 30; j++)			// white gray
    { for(k = 0; k < 320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6) + i;
          TFT_data(color);
        }
    }

  TFT_portrait();				// portrait mode
  TFT_string(13, 0,Magenta,Transparent,"����");
  TFT_string(12,38,Magenta,Transparent,"��ο�");
  Delay_ms(3000);
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display title
  LCD_string(0xC0,"   Exp07_x.c    ");

  while(1)
    { Beep();
      TFT_full_color();				// display full color screen
      Beep();
      TFT_8color_portrait();			// display portrait 8-color bar
      Beep();
      TFT_8color_landscape();			// display landscape 8-color bar
      Beep();
      TFT_8color_gray();			// display landscape 8-color gray
    }
}
