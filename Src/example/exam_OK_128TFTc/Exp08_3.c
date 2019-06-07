/* ========================================================================== */
/*		Exp08_3.c : TFT-LCD 모듈의 투명 글씨 및 외곽선체 사용	      */
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
      TFT_string(1, 2, White,Magenta," 투명 글씨/외곽선체  사용예 ");
      TFT_string(1,10, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string(2,15,Yellow,Transparent,"한글, English, 0123456789");
      TFT_string(2,18,Yellow,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(1,24, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string(2,30,Yellow,Transparent,"한글, English, 0123456789");
      TFT_string(2,33,Yellow,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Delay_ms(5000);

      TFT_color_screen(Yellow);
      TFT_string(1, 2, White,Magenta," 투명 글씨/외곽선체 사용예 ");
      TFT_string(1,10, White,Blue,   " (1) 외곽선이 없을 경우 ");
      TFT_string(2,15, White,Transparent,"한글, English, 0123456789");
      TFT_string(2,18, White,Transparent,"글씨가 잘 보이지 않습니다.");

      TFT_string(1,24, White,Blue,   " (2) 외곽선이 있을 경우 ");
      TFT_outline(Blue);
      TFT_string(2,30, White,Transparent,"한글, English, 0123456789");
      TFT_string(2,33, White,Transparent,"이제 글씨가 잘 보입니다.");
      TFT_outline(Transparent);
      Delay_ms(5000);
    }
}
