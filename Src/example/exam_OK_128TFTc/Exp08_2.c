/* ========================================================================== */
/*		Exp08_2.c : TFT-LCD Module Large Character Display            */
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
  LCD_string(0xC0,"   Exp08_2.c    ");
  Beep();

  while(1)
    { TFT_clear_screen(); 			// clear screen
      TFT_portrait();				// portrait mode
      Beep();

      TFT_string( 0, 0,Green,  Black,"******************************");
      TFT_string( 0, 2,Magenta,Black," OK-128TFT 트레이닝 키트 V1.0 ");
      TFT_string( 0, 4,Cyan,   Black,"     (TFT-32HX 보드 V2.0)     ");
      TFT_string( 0, 6,Green,  Black,"******************************");
      TFT_string( 0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");

      TFT_string( 0,16,Cyan,   Black,"      8x16 English Normal     ");
      TFT_string( 0,19,Yellow, Black,"     16x16 한글 보통 글씨     ");

      TFT_string_large(1,24, Cyan,  Black, "12x24 English Large");
      TFT_string_large(1,28, Yellow,Black, "24x24 한글 큰 글씨");

      TFT_string( 0,36,White,  Blue, "윤교수의 마이크로프로세서 월드");
      TFT_string( 0,38,Green,  Black,"  (http://cpu.kongju.ac.kr/)  ");
      Delay_ms(5000);

      TFT_clear_screen(); 			// clear screen
      TFT_landscape();				// landscape mode
      Beep();

      TFT_string( 0, 0,Green,  Black,"****************************************");
      TFT_string( 0, 2,Magenta,Black,"      OK-128TFT 트레이닝 키트 V1.0      ");
      TFT_string( 0, 4,Cyan,   Black,"          (TFT-32HX 보드 V2.0)          ");
      TFT_string( 0, 6,Green,  Black,"****************************************");
      TFT_string( 0, 9,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");

      TFT_string( 0,13,Cyan,   Black,"        8x16 English ASCII Normal       ");
      TFT_string( 0,15,Yellow, Black,"       16x16 한글 보통 글씨 명조체      ");

      TFT_string_large(1,18, Cyan,  Black, "12x24 English ASCII Large");
      TFT_string_large(1,21, Yellow,Black, "24x24 한글 큰 글씨 명조체");

      TFT_string( 0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
      TFT_string( 0,28,Green,  Black,"       (http://cpu.kongju.ac.kr/)       ");
      Delay_ms(5000);
    }
}

