/* ========================================================================== */
/*		Exp08_6.c : TFT-LCD ��⿡ ���ڿ��� �������� ���	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

void TFT_string_slow(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char *str)	/* write TFT-LCD string */
{
  unsigned char ch1;
  unsigned int ch2;

  Xcharacter = xChar;
  Ycharacter = yChar;

  foreground = colorfore;
  background = colorback;

  while(*str)
    { ch1 = *str;
      str++;

      if(ch1 < 0x80)				// English ASCII character
	TFT_English(ch1);
      else					// Korean
	{ ch2 = (ch1 << 8) + (*str);
          str++;
	  ch2 = KS_code_conversion(ch2);	// convert KSSM(�ϼ���) to KS(������)
	  TFT_Korean(ch2);
	}

      if(ch1 != ' ')
        { sbi(PORTC,2);				// buzzer on
          Delay_ms(5);				// delay 5 ms
          cbi(PORTC,2);				// buzzer off
        }

      Delay_ms(200);				// delay for slow display
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
  LCD_string(0xC0,"   Exp08_6.c    ");

  while(1)
    { TFT_clear_screen();			// display screen 1

      TFT_string_slow( 9, 1,White, Magenta," ���޷� �� ");
      Delay_ms(1000);
      TFT_string_slow(18, 5,Green, Black,"��ҿ�");
      Delay_ms(1000);
      TFT_string_slow( 0, 9,Cyan,  Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string_slow( 0,11,Cyan,  Black,"���� ������");
      Delay_ms(1000);
      TFT_string_slow( 0,13,Cyan,  Black,"������ ���� �����帮������.");
      Delay_ms(1000);
      TFT_string_slow( 0,17,Yellow,Black,"������ ���");
      Delay_ms(1000);
      TFT_string_slow( 0,19,Yellow,Black,"���޷���");
      Delay_ms(1000);
      TFT_string_slow( 0,21,Yellow,Black,"�Ƹ����� ���� �濡 �Ѹ�������.");
      Delay_ms(1000);
      TFT_string_slow( 0,25,Cyan,  Black,"���ô� ���� ����");
      Delay_ms(1000);
      TFT_string_slow( 0,27,Cyan,  Black,"���� �� ����");
      Delay_ms(1000);
      TFT_string_slow( 0,29,Cyan,  Black,"����� ������ ���ÿɼҼ�.");
      Delay_ms(1000);
      TFT_string_slow( 0,33,Red,   Black,"�����Ⱑ ���ܿ�");
      Delay_ms(1000);
      TFT_string_slow( 0,35,Red,   Black,"���� ������");
      Delay_ms(1000);
      TFT_string_slow( 0,37,Red,   Black,"�׾ �ƴ� ���� �긮������.");
      Delay_ms(3000);

      TFT_clear_screen();			// display screen 2

      TFT_string_slow( 6, 5,White, Magenta," ���⿡ ���� �ͼ� ");
      Delay_ms(1000);
      TFT_string_slow(12,11,Red,   Black,"(����)");
      Delay_ms(1000);
      TFT_string_slow( 0,13,Green, Black,"  ���� �ڶ������� �±ر� �տ� �����Ӱ� ���Ƿο� ���ѹα��� ������ ������ ���Ͽ� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(1000);
      TFT_string_slow(10,25,Red,   Black,"(�� ����)");
      Delay_ms(1000);
      TFT_string_slow( 0,27,Yellow,Black,"  ���� �ڶ����� �±ر� �տ� ������ ������ ������ ������ ���Ͽ� ���� ������ ���� �漺�� ���� ���� ���� �����մϴ�.");
      Delay_ms(3000);
    }
}
