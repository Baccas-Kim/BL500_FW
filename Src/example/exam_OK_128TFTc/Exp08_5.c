/* ========================================================================== */
/*		Exp08_5.c : TFT-LCD ��⿡ �� ���ڿ��� ���		      */
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
  LCD_string(0xC0,"   Exp08_5.c    ");

  while(1)
    { TFT_clear_screen();			// display screen 1
      Beep();

      TFT_string( 9, 1,White, Magenta," ���޷� �� ");
      Delay_ms(1000);
      TFT_string(18, 5,Green, Black,"��ҿ�");
      Delay_ms(1000);
      TFT_string( 0, 9,Cyan,  Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string( 0,11,Cyan,  Black,"���� ������");
      Delay_ms(1000);
      TFT_string( 0,13,Cyan,  Black,"������ ���� �����帮������.");
      Delay_ms(1000);
      TFT_string( 0,17,Yellow,Black,"������ ���");
      Delay_ms(1000);
      TFT_string( 0,19,Yellow,Black,"���޷���");
      Delay_ms(1000);
      TFT_string( 0,21,Yellow,Black,"�Ƹ����� ���� �濡 �Ѹ�������.");
      Delay_ms(1000);
      TFT_string( 0,25,Cyan,  Black,"���ô� ���� ����");
      Delay_ms(1000);
      TFT_string( 0,27,Cyan,  Black,"���� �� ����");
      Delay_ms(1000);
      TFT_string( 0,29,Cyan,  Black,"����� ������ ���ÿɼҼ�.");
      Delay_ms(1000);
      TFT_string( 0,33,Red,   Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string( 0,35,Red,   Black,"���� ������");
      Delay_ms(1000);
      TFT_string( 0,37,Red,   Black,"�׾ �ƴ� ���� �긮������.");
      Delay_ms(3000);

      TFT_clear_screen();			// display screen 2
      Beep();

      TFT_string( 6, 5,White, Magenta," ���⿡ ���� �ͼ� ");
      Delay_ms(1000);
      TFT_string(12,11,Red,   Black,"(����)");
      Delay_ms(1000);
      TFT_string( 0,13,Green, Black,"  ���� �ڶ������� �±ر� �տ� �����Ӱ� ���Ƿο� ���ѹα��� ������ ������ ���Ͽ� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(1000);
      TFT_string(10,25,Red,   Black,"(�� ����)");
      Delay_ms(1000);
      TFT_string( 0,27,Yellow,Black,"  ���� �ڶ����� �±ر� �տ� ������ ������ ������ ������ ���Ͽ� ���� ������ ���� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(5000);
    }
}
