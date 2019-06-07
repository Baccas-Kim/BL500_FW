/* ========================================================================== */
/*		Exp08_1.c : Basic TFT-LCD Modlue Display	              */
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
  LCD_string(0xC0,"   Exp08_1.c    ");

  while(1)
    { TFT_clear_screen(); 			// clear screen
      TFT_portrait();				// portrait mode
      Beep();

      TFT_string( 0, 0,Green,  Black,"******************************");
      TFT_string( 0, 2,Magenta,Black," OK-128TFT 트레이닝 키트 V1.0 ");
      TFT_string( 0, 4,Cyan,   Black,"     (TFT-32HX 보드 V2.0)     ");
      TFT_string( 0, 6,Green,  Black,"******************************");
      TFT_string( 0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");
      TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀     ");
      TFT_string( 0,16,Yellow, Black,"          65536 컬러          ");
      TFT_string( 0,20,Red,    Black,"     8비트 CPU 인터페이스     ");
      TFT_string( 0,24,White,  Black,"   컨트롤러/드라이버 HX8347   ");
      TFT_string( 0,26,White,  Black,"         (대만 Himax)         ");
      TFT_string( 0,30,Red,    Black," 빨강 Red ");
      TFT_string(10,30,Green,  Black," 녹색 Gre ");
      TFT_string(20,30,Blue,   Black," 파랑 Blu ");
      TFT_string( 0,32,Yellow, Black," 노랑 Yel ");
      TFT_string(10,32,Cyan,   Black," 녹청 Cya ");
      TFT_string(20,32,Magenta,Black," 자홍 Mag ");
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
      TFT_string( 0,10,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");
      TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀, 65536 컬러   ");
      TFT_string( 0,16,White,  Black,"  컨트롤러/드라이버 HX8347(대만 Himax)  ");
      TFT_string( 0,20,Red,    Black," 빨강 Red    ");
      TFT_string(13,20,Green,  Black," 녹색 Green  ");
      TFT_string(26,20,Blue,   Black," 파랑 Blue    ");
      TFT_string( 0,22,Yellow, Black," 노랑 Yellow ");
      TFT_string(13,22,Cyan,   Black," 녹청 Cyan   ");
      TFT_string(26,22,Magenta,Black," 자홍 Magenta ");
      TFT_string( 0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
      TFT_string( 0,28,Green,  Black,"       (http://cpu.kongju.ac.kr/)       ");
      Delay_ms(5000);
    }
}

