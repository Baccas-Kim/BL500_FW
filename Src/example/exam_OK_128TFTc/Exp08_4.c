/* ========================================================================== */
/*		Exp08_4.c : TFT-LCD ����� ���� �� �ܰ���ü ���	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"
#include "OK-128TFT_large.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp08_4.c    ");

  while(1)
    { TFT_clear_screen();
      Beep();

      TFT_portrait();				// portrait mode for normal font
      TFT_string(2,11, Green,Black,"�ѱ�/English 12345 ����ü");
      TFT_underscore(Yellow);
      TFT_string(2,14, Cyan, Black,"�ѱ�/English 12345 ����");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string(2,17, White,Black,"�ѱ�/English 12345 �ܰ���ü");
      TFT_outline(Transparent);

      TFT_portrait_large();			// portrait mode for large font
      TFT_string_large(0, 3, White,Magenta," ����/�ܰ���ü ��� ");
      TFT_string_large(1,24, Green,Black,"�ѱ�/English 12345");
      TFT_underscore(Yellow);
      TFT_string_large(1,29, Cyan, Black,"�ѱ�/English 12345");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string_large(1,34, White,Black,"�ѱ�/English 12345");
      TFT_outline(Transparent);
      Delay_ms(5000);

      TFT_clear_screen();
      Beep();

      TFT_landscape();				// landscape mode for normal font
      TFT_string(6, 7, Green,Black,"�ѱ�/English 12345 ����ü");
      TFT_underscore(Yellow);
      TFT_string(6,10, Cyan, Black,"�ѱ�/English 12345 ����");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string(6,13, White,Black,"�ѱ�/English 12345 �ܰ���ü");
      TFT_outline(Transparent);

      TFT_landscape_large();			// landscape mode for large font
      TFT_string_large(2, 1, White,Magenta," ����/�ܰ���ü ��� �� ");
      TFT_string_large(1,18, Green,Black,"�ѱ�/English 12345 ����ü");
      TFT_underscore(Yellow);
      TFT_string_large(1,22, Cyan, Black,"�ѱ�/English 12345 ����");
      TFT_underscore(Transparent);
      TFT_outline(Red);
      TFT_string_large(1,26, White,Black,"�ѱ�/English 12345 �ܰ���");
      TFT_outline(Transparent);
      Delay_ms(5000);
    }
}

