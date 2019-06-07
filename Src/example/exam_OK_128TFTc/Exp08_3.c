/* ========================================================================== */
/*		Exp08_3.c : TFT-LCD ����� ���� �۾� �� �ܰ���ü ���	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp08_3.c    ");
  Beep();

  while(1)
    { TFT_color_screen(White);
      TFT_string(1, 2, White,Magenta," ���� �۾�/�ܰ���ü  ��뿹 ");
      TFT_string(1,10, White,Blue,   " (1) �ܰ����� ���� ��� ");
      TFT_string(2,15,Yellow,Transparent,"�ѱ�, English, 0123456789");
      TFT_string(2,18,Yellow,Transparent,"�۾��� �� ������ �ʽ��ϴ�.");

      TFT_string(1,24, White,Blue,   " (2) �ܰ����� ���� ��� ");
      TFT_outline(Blue);
      TFT_string(2,30,Yellow,Transparent,"�ѱ�, English, 0123456789");
      TFT_string(2,33,Yellow,Transparent,"���� �۾��� �� ���Դϴ�.");
      TFT_outline(Transparent);
      Delay_ms(5000);

      TFT_color_screen(Yellow);
      TFT_string(1, 2, White,Magenta," ���� �۾�/�ܰ���ü ��뿹 ");
      TFT_string(1,10, White,Blue,   " (1) �ܰ����� ���� ��� ");
      TFT_string(2,15, White,Transparent,"�ѱ�, English, 0123456789");
      TFT_string(2,18, White,Transparent,"�۾��� �� ������ �ʽ��ϴ�.");

      TFT_string(1,24, White,Blue,   " (2) �ܰ����� ���� ��� ");
      TFT_outline(Blue);
      TFT_string(2,30, White,Transparent,"�ѱ�, English, 0123456789");
      TFT_string(2,33, White,Transparent,"���� �۾��� �� ���Դϴ�.");
      TFT_outline(Transparent);
      Delay_ms(5000);
    }
}
