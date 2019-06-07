/* ============================================================================ */
/*     kit_test.c : OK-128TFT 키트(3.2" 240x320 픽셀) 하드웨어 진단 프로그램	*/
/* ============================================================================ */
/*                          Designed and programmed by Duck-Yong Yoon in 2010.  */
/*                          Modified for I3202-7HMT2432A V1.1 in 2011/10/31.    */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

/* ---------------------------------------------------------------------------- */

void Display_screen(void);			// display test screen
void Display_test(U08 current);			// display test number
void Test1(void);				// Test 1
void TFT_8color_horizontal(void);		// display 8-color horizontal
void TFT_8color_vertical(void);			// display 8-color vertical
void TFT_color_gray(void);			// display 4-color gray
void Test2(void);				// Test 2
void Test3(void);				// Test 3
void Test4(void);				// Test 4
void Test5(void);				// Test 5
void Test6(void);				// Test 6
void Set_tempo(unsigned char number);		// set tempo
void Play_note(unsigned int sound, unsigned int note); // play note
void Play_rest(unsigned int rest);		// play rest
void Test7(void);				// Test 7
void Test8(void);				// Test 8
void Waveform_start(void);			// start waveform by TCF0 OVF interrupt
void Test9(void);				// Test 9
void Test10(void);				// Test 10
void DS1307_initialize(void);			// check DS1307 RTC
unsigned char DS1307_read(U08 address);		// read DS1307
void DS1307_write(U08 address, U08 TxD);	// write DS1307 
void Display_time(void);			// display DS1307 time
unsigned char BCD_increment(U08 number);	// BCD increment
unsigned char BCD_decrement(U08 number);	// BCD decrement
void Cursor_left(void);			        // go cursor left
void Cursor_right(void);             		// go cursor right
void Increment(void);	               		// increment time
void Decrement(void);	               		// decrement time
void Test11(void);				// Test 11
void Analog_increment(void);			// increment time
void Analog_decrement(void);			// decrement time
void Analog_cursor_left(void);			// go cursor left
void Analog_cursor_right(void);			// go cursor right
void Analog_display_time(void);			// display analog and digital time
void Draw_analog_clock(void);			// draw analog clock
void Draw_pin(void);				// draw hour, minute, second pin
void Line_soft(S16 x1,S16 y1, S16 x2,S16 y2, U16 color); // draw a soft line
void TFT_English_pixel(U16 Xpixel,U16 Ypixel, U08 code); // write a English ASCII character

/* ---------------------------------------------------------------------------- */

int main(void)
{
  unsigned char key = no_key, test = 1;

  MCU_initialize();          			// initialize ATmega128A MCU 
  Delay_ms(200);
  LCD_initialize();				// initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"  by  D.Y.Yoon  ");

  TFT_string(2,10,White,Magenta,"OK-128TFT 키트 진단 프로그램");
  TFT_string(2,20,Cyan,   Black,"      2011년 10월 31일      ");
  TFT_string(2,25,Green,  Black,"        윤덕용 작성        ");
  TFT_string(2,30,Yellow, Black,"  (지원 : 도서출판 Ohm사)  ");

  sbi(PORTD,4);					// booting LED and beep
  Beep();
  Delay_ms(100);
  cbi(PORTD,4);
  sbi(PORTD,5);
  Beep();
  Delay_ms(100);
  cbi(PORTD,5);
  sbi(PORTD,6);
  Beep();
  Delay_ms(100);
  cbi(PORTD,6);
  sbi(PORTD,7);
  Beep();
  Delay_ms(100);
  cbi(PORTD,7);
  Delay_ms(2000);

  LCD_string(0xC0,"    Test  01    ");		// display test number

  Display_screen();				// display test screen
  Display_test(test);				// display test number

  while(1)
    { while(key == no_key)			// wait key input
        key = Key_input();

      if(key == KEY3)				// if KEY3, decrenment test number
       { key = no_key;
         test--;
         if(test == 0) test = 11;
         Display_test(test);
       }
      else if(key == KEY4)			// if KEY4, increnment test number
       { key = no_key;
         test++;
         if(test > 11) test = 1;
         Display_test(test);
       }
      else if(key == KEY2)			// if KEY2, ignore and skip
       key = no_key;
      else if(key == KEY1)			// if KEY1, run test number
       { key = no_key;
         if(test == 1) Test1();
         else if(test == 2) Test2();
         else if(test == 3) Test3();
         else if(test == 4) Test4();
         else if(test == 5) Test5();
         else if(test == 6) Test6();
         else if(test == 7) Test7();
         else if(test == 8) Test8();
         else if(test == 9) Test9();
         else if(test == 10) Test10();
         else if(test == 11) Test11();

         Display_screen();			// display test screen
         Display_test(test);			// display test number
       }
    }
}

/* ---------------------------------------------------------------------------- */

void Display_screen(void)			/* display test screen */
{
  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(1, 1,White,Magenta,"OK-128TFT 키트 진단 프로그램");
  TFT_string(1, 4,Cyan,   Black,"  (1) TFT-LCD 컬러 테스트");
  TFT_string(1, 7,Green,  Black,"  (2) TFT-LCD 한글 테스트");
  TFT_string(1,10,Cyan,   Black,"  (3) TFT-LCD 그래픽 테스트");
  TFT_string(1,13,Green,  Black,"  (4) 터치스크린 테스트");
  TFT_string(1,16,Cyan,   Black,"  (5) LED/버저 출력 테스트");
  TFT_string(1,19,Green,  Black,"  (6) 스피커 출력 테스트");
  TFT_string(1,22,Cyan,   Black,"  (7) A/D 컨버터 입력 테스트");
  TFT_string(1,25,Green,  Black,"  (8) D/A 컨버터 출력 테스트");
  TFT_string(1,28,Cyan,   Black,"  (9) RS-232C 통신 테스트");
  TFT_string(1,31,Green,  Black,"  (10) DS1307 디지털 시계");
  TFT_string(1,34,Cyan,   Black,"  (11) DS1307 아날로그 시계");
  TFT_string(1,37,White,  Black," KEY1        KEY3    KEY4");
  TFT_string(7,37,Magenta,Black,"실행");

  Line(154,296,154,311, Magenta);		// display up arrow(decrement test number)
  Line(155,296,155,311, Magenta);
  Line(149,301,154,296, Magenta);
  Line(149,302,154,297, Magenta);
  Line(160,301,155,296, Magenta);
  Line(160,302,155,297, Magenta);

  Line(218,296,218,311, Magenta);		// display down arrow(increment test number)
  Line(219,296,219,311, Magenta);
  Line(213,306,218,311, Magenta);
  Line(213,305,218,310, Magenta);
  Line(224,306,219,311, Magenta);
  Line(224,305,219,310, Magenta);
}

void Display_test(U08 current)			/* display test number */
{
  LCD_command(0xCA);				// text LCD
  LCD_unsigned_decimal(current, 1, 2);

  TFT_string( 1, 4,Cyan,   Black,"  ");		// TFT-LCD
  TFT_string( 1, 7,Green,  Black,"  ");
  TFT_string( 1,10,Cyan,   Black,"  ");
  TFT_string( 1,13,Green,  Black,"  ");
  TFT_string( 1,16,Cyan,   Black,"  ");
  TFT_string( 1,19,Green,  Black,"  ");
  TFT_string( 1,22,Cyan,   Black,"  ");
  TFT_string( 1,25,Green,  Black,"  ");
  TFT_string( 1,28,Cyan,   Black,"  ");
  TFT_string( 1,31,Green,  Black,"  ");
  TFT_string( 1,34,Green,  Black,"  ");
  TFT_string( 1,current*3 + 1, Magenta,Black,">>");
}

/* ---------------------------------------------------------------------------- */

void Test1(void)				/* Test 1 */
{
  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(3,10,Green,Magenta,"(1) TFT-LCD 컬러 테스트");
  Delay_ms(500);

  TFT_color_screen(White);  			// display full color(white) 
  TFT_string(10,1,Black,Transparent,"백색 화면");
  Delay_ms(1000);
  TFT_color_screen(Red);  			// display full color(red)
  TFT_string(10,1,Green,Transparent,"적색 화면");
  Delay_ms(1000);
  TFT_color_screen(Green);  			// display full color(green)
  TFT_string(10,1,Red,  Transparent,"녹색 화면");
  Delay_ms(1000);
  TFT_color_screen(Blue);  			// display full color(blue)  
  TFT_string(10,1,Cyan, Transparent,"청색 화면");
  Delay_ms(1000);

  TFT_8color_horizontal();			// display 8-Color(horizontal)
  TFT_string(13, 1,Black,Transparent,"백색");
  TFT_string(13, 7,White,Transparent,"흑색");
  TFT_string(13,11,Green,Transparent,"적색");
  TFT_string(13,16,Red,  Transparent,"녹색");
  TFT_string(13,21,Cyan, Transparent,"청색");
  TFT_string(13,26,Black,Transparent,"노랑");
  TFT_string(13,31,Red,  Transparent,"녹청");
  TFT_string(13,36,White,Transparent,"자홍");
  Delay_ms(3000);				
  TFT_8color_vertical();			// display 8-Color(vertical)
  TFT_string( 1, 1,White,Transparent,"자");
  TFT_string( 1, 3,White,Transparent,"홍");
  TFT_string( 5, 1,Red,  Transparent,"녹");
  TFT_string( 5, 3,Red,  Transparent,"청");
  TFT_string( 9, 1,Black,Transparent,"노");
  TFT_string( 9, 3,Black,Transparent,"랑");
  TFT_string(13, 1,Cyan, Transparent,"청");
  TFT_string(13, 3,Cyan, Transparent,"색");
  TFT_string(16, 1,Red,  Transparent,"녹");
  TFT_string(16, 3,Red,  Transparent,"색");
  TFT_string(20, 1,Green,Transparent,"적");
  TFT_string(20, 3,Green,Transparent,"색");
  TFT_string(24, 1,White,Transparent,"흑");
  TFT_string(24, 3,White,Transparent,"색");
  TFT_string(28, 1,Black,Transparent,"백");
  TFT_string(28, 3,Black,Transparent,"색");
  Delay_ms(3000);

  TFT_color_gray();				// display 8-Gray(horizontal)
  TFT_string(13, 0,Magenta,Transparent,"밝음");
  TFT_string(12,38,Magenta,Transparent,"어두움");
  Delay_ms(3000);
}

unsigned int Color[8] =				// 8-color data
   { White, Black, Red, Green, Blue, Yellow, Cyan, Magenta };

void TFT_8color_horizontal(void)		/* display 8-color horizontal */
{
  unsigned int i, j, k;

  TFT_portrait();
  TFT_GRAM_address(0,0);

  for(i=0; i<8; i++)
    for(j=0; j<40; j++)
      for(k=0; k<240; k++)
        TFT_data(Color[i]);
}

void TFT_8color_vertical(void)			/* display 8-color vertical */
{
  unsigned int i, j, k;

  TFT_landscape();
  TFT_GRAM_address(0,0);

  for(i = 0; i < 8;i++)
    for(j = 0; j < 30; j++)
      for(k = 0; k < 320; k++)
        TFT_data(Color[i]);

  TFT_portrait();
}

void TFT_color_gray(void)			/* display 4-color gray */
{
  unsigned int i, j, k, color;

  TFT_landscape();
  TFT_GRAM_address(0,0);

  for(j=0; j<30; j++)				// white gray
    { for(k=0; k<320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6) + i;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// red gray
    { for(k=0; k<320; k++)
        { color = (0x1F - k/13) << 11;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// green gray
    { for(k=0; k<320; k++)
        { color = (0x1F - k/13) << 6;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// blue gray
    { for(k=0; k<320; k++)
        { color = 0x1F - k/13;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// yellow gray
    { for(k=0; k<320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6);
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// cyan gray
    { for(k=0; k<320; k++)
        { i = 0x1F - k/13;
	  color = (i << 6) + i;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// magenta gray
    { for(k=0; k<320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + i;
          TFT_data(color);
        }
    }

  for(j=0; j<30; j++)				// white gray
    { for(k=0; k<320; k++)
        { i = 0x1F - k/13;
	  color = (i << 11) + (i << 6) + i;
          TFT_data(color);
        }
    }

  TFT_portrait();
}

/* ---------------------------------------------------------------------------- */

void Test2(void)				/* Test 2 */
{
  TFT_clear_screen(); 				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(3,10,Green,Magenta,"(2) TFT-LCD 한글 테스트");
  Delay_ms(500);
  TFT_clear_screen();

  TFT_string( 0, 0,Green,  Black,"******************************");
  TFT_string( 0, 2,Magenta,Black," OK-128TFT 트레이닝 키트 V1.0 ");
  TFT_string( 0, 4,Cyan,   Black,"     (TFT-32HX 보드 V2.0)     ");
  TFT_string( 0, 6,Green,  Black,"******************************");
  TFT_string( 0, 8,Black,  Black,"                              ");
  TFT_string( 0,10,Magenta,Black," (디스플레이 모드 = Portrait) ");
  TFT_string( 0,12,Black,  Black,"                              ");
  TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀     ");
  TFT_string( 0,16,Yellow, Black,"          65536 컬러          ");
  TFT_string( 0,18,Black,  Black,"                              ");
  TFT_string( 0,20,Red,    Black,"    16비트 CPU 인터페이스     ");
  TFT_string( 0,22,Black,  Black,"                              ");
  TFT_string( 0,24,White,  Black,"   컨트롤러/드라이버 HX8347   ");
  TFT_string( 0,26,White,  Black,"         (대만 Himax)         ");
  TFT_string( 0,28,Black,  Black,"                              ");
  TFT_string( 0,30,Red,    Black," 빨강 Red ");
  TFT_string(10,30,Green,  Black," 녹색 Gre ");
  TFT_string(20,30,Blue,   Black," 파랑 Blu ");
  TFT_string( 0,32,Yellow, Black," 노랑 Yel ");
  TFT_string(10,32,Cyan,   Black," 녹청 Cya ");
  TFT_string(20,32,Magenta,Black," 자홍 Mag ");
  TFT_string( 0,34,Black,  Black,"                              ");
  TFT_string( 0,36,White,  Blue, "윤교수의 마이크로프로세서 월드");
  TFT_string( 0,38,Green,  Black,"http://cafe.naver.com/mcu32bit");
  Delay_ms(5000);

  TFT_clear_screen(); 				// clear screen
  TFT_landscape();				// landscape mode

  TFT_string( 0, 0,Green,  Black,"****************************************");
  TFT_string( 0, 2,Magenta,Black,"      OK-128TFT 트레이닝 키트 V1.0      ");
  TFT_string( 0, 4,Cyan,   Black,"          (TFT-32HX 보드 V2.0)          ");
  TFT_string( 0, 6,Green,  Black,"****************************************");
  TFT_string( 0, 8,Black,  Black,"                                        ");
  TFT_string( 0,10,Magenta,Black,"      (디스플레이 모드 = Landscape)     ");
  TFT_string( 0,12,Black,  Black,"                                        ");
  TFT_string( 0,14,Cyan,   Black,"    3.2인치, 240x320 픽셀, 65536 컬러   ");
  TFT_string( 0,16,White,  Black,"  컨트롤러/드라이버 HX8347(대만 Himax)  ");
  TFT_string( 0,18,Black,  Black,"                                        ");
  TFT_string( 0,20,Red,    Black," 빨강 Red    ");
  TFT_string(13,20,Green,  Black," 녹색 Green  ");
  TFT_string(26,20,Blue,   Black," 파랑 Blue    ");
  TFT_string( 0,22,Yellow, Black," 노랑 Yellow ");
  TFT_string(13,22,Cyan,   Black," 녹청 Cyan   ");
  TFT_string(26,22,Magenta,Black," 자홍 Magenta ");
  TFT_string( 0,24,Black,  Black,"                                        ");
  TFT_string( 0,26,White,  Blue, "     윤교수의 마이크로프로세서 월드     ");
  TFT_string( 0,28,Green,  Black,"    (http://cafe.naver.com/mcu32bit)    ");
  Delay_ms(5000);
}

/* ---------------------------------------------------------------------------- */

void Test3(void)				/* Test 3 */
{
  unsigned int i, x, y;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(2,10,Green,Magenta,"(3) TFT-LCD 그래픽 테스트");
  Delay_ms(500);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// graphic show 1(line)
  Rectangle(1,1, 238,318, Magenta);
  Delay_ms(500);
  for(x = 10; x <= 230; x += 10)
    { Line(x,0, x,319, Green);
      Delay_ms(50);
    }
  for(y = 10; y <= 310; y += 10)
    { Line(0,y, 239,y, Green);
      Delay_ms(50);
    }
  Delay_ms(3000);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// (ramp)
  Rectangle(1,1, 238,318, Magenta);
  for(i = 0,x = 12,y = 20; i <= 34; i++,x += 12,y += 20)
    { Line(x,0, 0,y, Green);
      Line(0,319-y, x,319, Green);
      Delay_ms(50);
    }
  for(i = 0,x = 420,y = 700; i <= 34; i++,x -= 12,y -= 20)
    { Line(x,0, 0,y, Red);
      Line(0,319-y, x,319, Red);
      Delay_ms(50);
    }
  Delay_ms(3000);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// graphic show 2(rectangle)
  Rectangle(1,1, 238,318, Magenta);
  for(i = 0,x = 114,y = 152; i <= 18; i++,x -= 6,y -= 8)
    { Rectangle(x,y, 239-x,319-y, Cyan);
      Delay_ms(50);
    }
  Delay_ms(1000);
  for(i = 0,x = 6,y = 8; i <= 18; i++,x += 6,y += 8)
    { Rectangle(x,y, 239-x,319-y, White);
      Delay_ms(50);
    }
  for(i = 0,x = 114,y = 152; i <= 18; i++,x -= 6,y -= 8)
    { Rectangle(x,y, 239-x,319-y, Green);
      Delay_ms(50);
    }
  Delay_ms(3000);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// graphic show 3(circle)
  Rectangle(1,1, 238,318, Magenta);
  for(i = 10; i <= 160; i += 10)
    { Circle(120,160,i, Green);
      Delay_ms(50);
    }
  Delay_ms(1000);
  for(i = 160; i >= 10; i -= 10)
    { Circle(120,160,i, Red);
      Delay_ms(50);
    }
  for(i = 10; i <= 160; i += 10)
    { Circle(120,160,i, White);
      Delay_ms(50);
    }
  Delay_ms(3000);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// (two)
  Rectangle(1,1, 238,318, Magenta);
  for(i = 10; i <= 120; i += 10)
    { Circle(120,80,i, Green);
      Circle(120,240,i, Green);
      Delay_ms(50);
    }
  Delay_ms(1000);
  for(i = 120; i >= 10; i -= 10)
    { Circle(120,80,i, White);
      Circle(120,240,i, White);
      Delay_ms(50);
    }
  for(i = 10; i <= 120; i += 10)
    { Circle(120,80,i, Cyan);
      Circle(120,240,i, Cyan);
      Delay_ms(50);
    }
  Delay_ms(3000);
  TFT_clear_screen();

  Rectangle(0,0, 239,319, Magenta);		// graphic show 4(sine)
  Rectangle(1,1, 238,318, Magenta);
  Line(120,0, 120,319, Magenta);
  for(i = 10; i <= 120; i += 10)
    { Sine(i, 0, Green);
      Delay_ms(20);
    }
  Delay_ms(1000);
  for(i = 10; i <= 120; i += 10)
    { Sine(i, 1, Blue);
      Delay_ms(20);
    }
  Delay_ms(3000);
}

/* ---------------------------------------------------------------------------- */

void Test4(void)				/* Test 4 */
{
  unsigned char i;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);

  TFT_string(4,7,Green,Magenta,"(4) 터치스크린 테스트 ");

  Initialize_touch_screen();			// initialize touch screen

  if(touch_flag == 'A')
    TFT_string(11,10,Green,Black,"ADS7846");
  else
    TFT_string(11,10,Green,Black,"TSC2003");

  TFT_string( 1, 1,Cyan,   Black,"(0,0)");
  TFT_string(22, 1,Cyan,   Black,"(239,0)");
  TFT_string( 1,37,Cyan,   Black,"(0,319)");
  TFT_string(20,37,Cyan,   Black,"(239,319)");

  TFT_string( 2,16,Cyan,   Black,"   원시 데이터");
  TFT_string(16,16,Magenta,Black,              " x = 0000   ");
  TFT_string( 2,18,Magenta,Black,"               y = 0000   ");
  TFT_string( 2,22,Cyan,   Black,"   픽셀 데이터");
  TFT_string(16,22,Magenta,Black,              " x = 000    ");
  TFT_string( 2,24,Magenta,Black,"               y = 000    ");
  TFT_string(12,30,White,  Blue, "비프음");

  TFT_string( 6,35,White,  Black,  "스크린을 누르시오! ");
  Beep();

  for(i = 0; i < 100; i++)			// 10 sec
    { Touch_screen_input();			// input touch screen

      TFT_color(Cyan,Black);
      TFT_xy(21,16);				// display 12-bit original X data
      TFT_unsigned_decimal(x_12bit,0,4);
      TFT_xy(21,18);				// display 12-bit original Y data
      TFT_unsigned_decimal(y_12bit,0,4);
      TFT_xy(21,22);				// display pixel X data
      TFT_unsigned_decimal(x_touch,0,3);
      TFT_xy(21,24);				// display pixel Y data
      TFT_unsigned_decimal(y_touch,0,3);

      if((x_touch >= 96) && (x_touch <= 143) && (y_touch >= 235) && (y_touch <= 255))
        { Beep();
          Delay_ms(200);
        }
   
      Delay_ms(50);
    }
}

/* ---------------------------------------------------------------------------- */

void Test5(void)				/* Test 5 */
{
  unsigned char i;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(3,10,Green,Magenta,"(5) LED/버저 출력 테스트");
  Delay_ms(500);

  for(i = 0; i < 2; i++)			// repeat 2 times
    { TFT_string(7,20,Magenta,Black,"                ");
      TFT_string(7,23,Green,  Black,"                ");
      TFT_string(7,26,Red,    Black,"                ");
      TFT_string(7,29,Cyan,   Black,"                ");

      TFT_string(7,20,Magenta,Black,"LED1 점등 / 비프");
      sbi(PORTD,4);
      Beep();
      Delay_ms(500);
      cbi(PORTD,4);
      TFT_string(7,23,Green,  Black,"LED2 점등 / 비프");
      sbi(PORTD,5);
      Beep();
      Delay_ms(500);
      cbi(PORTD,5);
      TFT_string(7,26,Red,    Black,"LED3 점등 / 비프");
      sbi(PORTD,6);
      Beep();
      Delay_ms(500);
      cbi(PORTD,6);
      TFT_string(7,29,Cyan,   Black,"LED4 점등 / 비프");
      sbi(PORTD,7);
      Beep();
      Delay_ms(500);
      cbi(PORTD,7);
    }
}

/* ---------------------------------------------------------------------------- */

#define	VLOA	9008			// define note, octave 2
#define	VLOAX	8580
#define	VLOB	8098
#define	LOC	7644			// define note, octave 3
#define	LOCX	7214
#define	LOD	6810
#define	LODX	6427
#define	LOE	6066
#define	LOF	5726
#define	LOFX	5404
#define	LOG	5101
#define	LOGX	4815
#define	LOA	4544
#define	LOAX	4289
#define	LOB	4049
#define	MIC	3821			// define note, octave 4
#define	MICX	3607
#define	MID	3404
#define	MIDX	3213
#define	MIE	3033
#define	MIF	2862
#define	MIFX	2702
#define	MIG	2550
#define	MIGX	2407
#define	MIA	2272
#define	MIAX	2144
#define	MIB	2024
#define	HIC	1910			// define note, octave 5
#define	HICX	1803
#define	HID	1702
#define	HIDX	1606
#define	HIE	1516
#define	HIF	1431
#define	HIFX	1350
#define	HIG	1275
#define	HIGX	1203
#define	HIA	1135
#define	HIAX	1072
#define	HIB	1011
#define	VHIC	955			// define note, octave 6
#define	VHICX	901
#define	VHID	850
#define	VHIDX	803
#define	VHIE	757

#define	NOTE32	1*3			// define note length
#define	NOTE16	2*3
#define	NOTE16D	3*3
#define	NOTE16T	2*2
#define	NOTE8	4*3
#define	NOTE8D	6*3
#define	NOTE8T	4*2
#define	NOTE4	8*3
#define	NOTE4D	12*3
#define	NOTE4T	8*2
#define	NOTE2	16*3
#define	NOTE2D	24*3
#define	NOTE1	32*3

#define	REST32	1*3			// define rest length
#define	REST16	2*3
#define	REST16D	3*3
#define	REST8	4*3
#define	REST8D	6*3
#define	REST4	8*3
#define	REST4D	12*3
#define	REST2	16*3
#define	REST2D	24*3
#define	REST1	32*3

void Test6(void)				/* Test 6 */
{
  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(4,10,Green,Magenta,"(6) 스피커 출력 테스트");
  Delay_ms(500);

  Set_tempo(4);					// set tempo

  TFT_string(11,20,Green,Black,"옥타브 4");
  TFT_string(3,24,White,Black,"도 레 미 파 솔 라 시 도~");

  Play_note(MIC,NOTE8);	             		// play
  Play_note(MID,NOTE8);
  Play_note(MIE,NOTE8);
  Play_note(MIF,NOTE8);
  Play_note(MIG,NOTE8);
  Play_note(MIA,NOTE8);
  Play_note(MIB,NOTE8);
  Play_note(HIC,NOTE4);
  Play_rest(REST4);

  TFT_string(3,27,White,Black,"도 시 라 솔 파 미 레 도~");

  Play_note(HIC,NOTE8);	             		// play
  Play_note(MIB,NOTE8);
  Play_note(MIA,NOTE8);
  Play_note(MIG,NOTE8);
  Play_note(MIF,NOTE8);
  Play_note(MIE,NOTE8);
  Play_note(MID,NOTE8);
  Play_note(MIC,NOTE4);
  Play_rest(REST4);
}

unsigned char tempo;

void Set_tempo(unsigned char number)            /* set tempo */
{
  tempo = number;                               // set tempo
  TCCR1A = 0x40;                                // CTC mode(12), use OC1A
  TCCR1B = 0x18;                                // speaker off
  TCCR1C = 0x00;
}

void Play_note(unsigned int sound, unsigned int note) /* play note */
{
  ICR1 = sound;					// set ICR1
  TCNT1 = 0x0000;                               // clear Timer/Counter 1
  TCCR1B = 0x1A;                                // start CTC mode(prescaler = 8)

  Delay_ms(note*tempo*7);                       // note play time

  TCCR1B = 0x18;                                // speaker off
}

void Play_rest(unsigned int rest)               /* play rest */
{
  Delay_ms(rest*tempo*7);                       // rest play time
}

/* ---------------------------------------------------------------------------- */

void Test7(void)				/* Test 7 */
{
  unsigned char i, time;
  signed int result;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(2,10,Green,Magenta,"(7) A/D 컨버터 입력 테스트");

  TFT_string(5,20,White,Black,"ADC1(온도) = 00.0 C");
  TFT_xy(22,20);
  TFT_English(0x7F);
  TFT_string(5,24,White,Black,"ADC2(온도) = 00.0 C");
  TFT_xy(22,24);
  TFT_English(0x7F);
  TFT_string(5,28,White,Black,"ADC3(전압) = 0.00V");


  for(time = 0; time < 40; time++)		// 10 sec
    { ADCSRA = 0x84;				// ADC enable, 1MHz
      ADMUX = 0xC1;				// select ADC1 with 2.56V
      Delay_us(150);

      result = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD4;			// start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          result += ADCL + ADCH*256;		// add A/D result 16 times
          Delay_ms(1);
        }

      result >>= 4;				// divide result by 16

      TFT_xy(18,20);				// display temperature
      TFT_color(Cyan,Black);
      TFT_unsigned_float(result*256./1024., 2, 1); // Temp = result*256/1024

      ADCSRA = 0x84;				// ADC enable, 1MHz
      ADMUX = 0xC2;				// select ADC2 with 2.56V
      Delay_us(150);

      result = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD4;			// start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          result += ADCL + ADCH*256;		// add A/D result 16 times
          Delay_ms(1);
        }

      result >>= 4;				// divide result by 16

      TFT_xy(18,24);				// display temperature
      TFT_color(Cyan,Black);
      TFT_unsigned_float(result*51.2/1024., 2, 1); // Temp = result*51.2/1024

      ADCSRA = 0x84;				// ADC enable, 1MHz
      ADMUX = 0x43;				// select ADC3 with AVCC
      Delay_us(150);

      result = 0;
      for(i = 0; i < 16; i++)			// read ADC by 16 times
        { ADCSRA = 0xD4;			// start conversion and clear ADIF
          while((ADCSRA & 0x10) != 0x10);
          result += ADCL + ADCH*256;		// add A/D result 16 times
          Delay_ms(1);
        }

      result >>= 4;				// divide result by 16

      TFT_xy(18,28);				// display voltage
      TFT_color(Cyan,Black);
      TFT_unsigned_float(result*5./1024., 1, 2); // Volt = result*5/1024

      Delay_ms(200);
    }
}

/* ---------------------------------------------------------------------------- */

volatile unsigned char data_count, DAC_data[200];
volatile unsigned int period_count;

void Test8(void)				/* Test 8 */
{
  unsigned char i, x;
  float a;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(2,10,Green,Magenta,"(8) D/A 컨버터 출력 테스트");

  TFT_string(9,20,Cyan,  Black,"1 kHz 삼각파");
  TFT_string(3,24,Yellow,Black,"(J2 = OFF : -10 ~ +10V)");
  TFT_string(3,27,Yellow,Black,"(J2 = ON  :   0 ~ +10V)");

  a = 255./100.;				// calculate data of triangular wave
  for(x = 0; x <= 100; x++)
    DAC_data[x] = a * x;

  i = 1;
  for(x = 101; x < 200; x++)
    { DAC_data[x] = DAC_data[100-i];
      i++;
    }

  Waveform_start();				// start waveform
  while(period_count < 5000);			// wait 5 sec
  cli();

  TFT_string(9,20,Cyan,  Black,"1 kHz 정현파");
  TFT_string(3,24,Yellow,Black,"(J2 = OFF : -10 ~ +10V)");
  TFT_string(3,27,Yellow,Black,"(J2 = ON  :   0 ~ +10V)");
  Beep();

  for(x = 0; x < 200; x++)			// calculate data of sinusoidal wave
    DAC_data[x] = 127. * sin(x * 1.8 * 3.141592 / 180.) + 128;

  Waveform_start();				// start waveform
  while(period_count < 5000);			// wait 5 sec
  cli();
}

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function */
{
  PORTA = DAC_data[data_count];

  data_count++;
  if(data_count == 200)
    { data_count = 0;
      period_count++;
    }
}

void Waveform_start(void)			/* start waveform by OC1A interrupt */
{
  data_count = 0;
  period_count = 0;

  TCCR1A = 0x00;				// CTC mode(4), don't output OC1A
  TCCR1B = 0x0A;				// 16MHz/8/(1+9) = 200kHz
  TCCR1C = 0x00;
  OCR1A = 9;
  TCNT1 = 0x0000;

  TIMSK = 0x10;					// enable OC1A interrupt
  ETIMSK = 0x00;
  TIFR = 0x3C;					// clear all interrupt flags
  ETIFR = 0x01;
  sei();					// global interrupt enable
}

/* ---------------------------------------------------------------------------- */

void Test9(void)				/* Test 9 */
{
  unsigned char i, test_flag, RxD;
  unsigned char character[9] = { "OK-128TFT" };

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(3,10,Green,Magenta,"(9) RS-232C 통신 테스트");

  TFT_string(9,13,White,Black,"CN5 컨넥터에");
  TFT_string(2,15,White,Black,"RS-232C 케이블을 접속하고");
  TFT_string(1,17,White,Black,"2번핀과 3번핀을 연결하시오!");

  TFT_string( 4,22,Cyan, Black,"송신 문자 : ");
  TFT_string( 4,25,Cyan, Black,"수신 문자 : ");
  Delay_ms(500);

  UBRR1H = 0;					// 19200 bps
  UBRR1L = 51;
  UCSR1A = 0x00;				// asynchronous normal mode
  UCSR1B = 0x18;				// Rx/Tx enable, 8 data
  UCSR1C = 0x06;				// no parity, 1 stop, 8 data
  Delay_ms(1);
  RxD = UDR1;					// dummy read

  test_flag = 0;
  for(i = 0; i < 9; i++)
    { UDR1 = character[i];			// transmit test data
      TFT_xy(16+i,22);				// display transmitted test data
      TFT_color(Green,Black);
      TFT_English(character[i]);
      Delay_ms(10);

      RxD = UDR1;				// receive test data
      TFT_xy(16+i,25);				// display transmitted test data
      TFT_color(Magenta,Black);
      TFT_English(RxD);

      if(RxD == character[i])			// data OK ?
        test_flag++;

      Delay_ms(500);
    }

  if(test_flag == 9)
    { TFT_string(10,30,Green,Black,"통신 OK !");
      Beep();
    }
  else
    { TFT_string(9,30,Magenta,Black,"통신 에러 !");
      Beep_3times();
    }

  Delay_ms(2000);
}

/* ---------------------------------------------------------------------------- */

void Test10(void)				/* Test 10 */
{
  unsigned char key;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  Rectangle(0,0, 239,319, Green);		// display title
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);
  TFT_string(3,10,Green,Magenta,"(10) 디지털 시계 테스트");

  TFT_string(5,20,White,Black,"2010년 12월 05일(일)");
  TFT_string(5,23,White,Black,"   오전 12:00:00    ");

  DS1307_initialize();				// initialize DS1307 RTC

  TFT_cursor(Red);				// display cursor
  Xcursor = 24;
  Ycursor = 23;

  while(1)
    { Display_time();				// display clock

      key = Key_input();			// key input
      if((key == KEY4) && (Xcursor == 24) && (Ycursor == 23))
        return;

      switch(key)
        { case KEY1 : Cursor_left();		// if KEY1, go cursor left
                      break;
          case KEY2 : Cursor_right();		// if KEY2, go cursor right
                      break;
          case KEY3 : Increment();		// if KEY3, increment time
                      break;
	  case KEY4 : Decrement();		// if KEY4, decrement time
                      break;
          default   : break;
        }
    }
}

unsigned char year, month, date, weekday, hour, minute, second;

void DS1307_initialize(void)			/* initialize DS1307 RTC */
{
  TWBR = 18;					// 100kHz(TWBR=18, TWPS=1)
  TWSR = 0x01;

  second =  DS1307_read(0x00);			// if CH = 1, initialize
  hour =  DS1307_read(0x02);			// if 24 hour system, initialize

  if(((second & 0x80) == 0x80) || ((hour & 0x40) == 0x00))
    { DS1307_write(0x00, 0x00);			// second = 0x00
      DS1307_write(0x01, 0x00);			// minute = 0x00
      DS1307_write(0x02, 0x12 | 0x40);		// hour = 0x12 AM (12 hour system)
      DS1307_write(0x03, 0x01);			// weekday = 0x01(Sunday)
      DS1307_write(0x04, 0x05);			// date = 0x05
      DS1307_write(0x05, 0x12);			// month = 0x12
      DS1307_write(0x06, 0x10);			// year = 0x2010
    }
}

unsigned char DS1307_read(U08 address)		/* read DS1307 */
{
  unsigned char RxD;

  TWCR = 0xA4;					// START condition
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)); // START complete ?

  TWDR = 0xD0;					// SLA+W
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x18)); // SLA+W complete ?

  TWDR = address;				// DS1307 address
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x28)); // address complete ?

  TWCR = 0xA4;					// REPEATED START condition
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x10)); // START complete ?

  TWDR = 0xD1;					// SLA+R
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x40)); // SLA+R complete ?

  TWCR = 0x84;					// read time data with no acknowledge
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x58)); // data complete ?
  RxD = TWDR;					// read data

  TWCR = 0x94;   		                // STOP condition
  Delay_us(10);

  return RxD;
}

void DS1307_write(U08 address, U08 TxD)		/* write DS1307 */
{
  TWCR = 0xA4;					// START condition
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)); // START complete ?

  TWDR = 0xD0;					// SLA+W
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x18)); // SLA+W complete ?

  TWDR = address;				// DS1307 address
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x28)); // address complete ?

  TWDR = TxD;					// write time data
  TWCR = 0x84;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x28)); // data complete ?

  TWCR = 0x94;					// STOP condition
  Delay_us(10);
}

unsigned char time_flag = 0;

void Display_time(void)				/* display DS1307 time */
{
  TFT_xy(5,20); TFT_color(Cyan,Black);		// display year
  TFT_hexadecimal(0x20, 2);
  year = DS1307_read(0x06);
  TFT_hexadecimal(year,2);
  TFT_xy(12,20); TFT_color(Cyan,Black);		// display month
  month = DS1307_read(0x05);
  TFT_hexadecimal(month,2);
  TFT_xy(17,20); TFT_color(Cyan,Black);		// display date
  date = DS1307_read(0x04);
  TFT_hexadecimal(date,2);
  weekday = DS1307_read(0x03);			// display weekday
  if(weekday == 0x01)      TFT_string(22,20,White,Black,"일");
  else if(weekday == 0x02) TFT_string(22,20,White,Black,"월");
  else if(weekday == 0x03) TFT_string(22,20,White,Black,"화");
  else if(weekday == 0x04) TFT_string(22,20,White,Black,"수");
  else if(weekday == 0x05) TFT_string(22,20,White,Black,"목");
  else if(weekday == 0x06) TFT_string(22,20,White,Black,"금");
  else if(weekday == 0x07) TFT_string(22,20,White,Black,"토");
  TFT_xy(13,23); TFT_color(Green,Black);	// display hour
  hour = DS1307_read(0x02);
  TFT_hexadecimal(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string(10,23,White,Black,"전");
  else                      TFT_string(10,23,White,Black,"후");
  TFT_xy(16,23); TFT_color(Green,Black);	// display minute
  minute = DS1307_read(0x01);
  TFT_hexadecimal(minute,2);
  TFT_xy(19,23); TFT_color(Green,Black);	// display second
  second = DS1307_read(0x00);
  TFT_hexadecimal(second & 0x7F,2);
  TFT_xy(24,23); TFT_color(Green,Black);	// display home
  TFT_English(' ');

  if((Xcursor == 24) && (Ycursor == 23))
    { TFT_string(3,35,White,Black,"KEY4 : 시계 프로그램 종료");
      TFT_string(3,37,White,Black,"                         ");
    }
  else
    { TFT_string(3,35,White,Black,"KEY1 : 좌측, KEY3 : 증가 ");
      TFT_string(3,37,White,Black,"KEY2 : 우측, KEY4 : 감소 ");
    }

  if((minute == 0x00) && (second == 0x00) && (time_flag == 0))
    { time_flag = 1;
      Beep();
    }
  else if((minute != 0x00) || (second != 0x00))
    time_flag = 0;
}

unsigned char BCD_increment(U08 number)		/* BCD increment */
{ unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i++;                                          // incrment
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

unsigned char BCD_decrement(U08 number)		/* BCD decrement */
{ unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i--;                                          // decrement
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

void Cursor_left(void)			        /* go cursor left */
{
  if     ((Xcursor == 24) && (Ycursor == 23)) { Xcursor = 20; Ycursor = 23; }
  else if((Xcursor == 20) && (Ycursor == 23)) { Xcursor = 17; Ycursor = 23; }
  else if((Xcursor == 17) && (Ycursor == 23)) { Xcursor = 14; Ycursor = 23; }
  else if((Xcursor == 14) && (Ycursor == 23)) { Xcursor = 10; Ycursor = 23; }
  else if((Xcursor == 10) && (Ycursor == 23)) { Xcursor = 22; Ycursor = 20; }
  else if((Xcursor == 22) && (Ycursor == 20)) { Xcursor = 18; Ycursor = 20; }
  else if((Xcursor == 18) && (Ycursor == 20)) { Xcursor = 13; Ycursor = 20; }
  else if((Xcursor == 13) && (Ycursor == 20)) { Xcursor =  8; Ycursor = 20; }
  else if((Xcursor ==  8) && (Ycursor == 20)) { Xcursor = 24; Ycursor = 23; }
}

void Cursor_right(void)             		/* go cursor right */
{
  if     ((Xcursor ==  8) && (Ycursor == 20)) { Xcursor = 13; Ycursor = 20; }
  else if((Xcursor == 13) && (Ycursor == 20)) { Xcursor = 18; Ycursor = 20; }
  else if((Xcursor == 18) && (Ycursor == 20)) { Xcursor = 22; Ycursor = 20; }
  else if((Xcursor == 22) && (Ycursor == 20)) { Xcursor = 10; Ycursor = 23; }
  else if((Xcursor == 10) && (Ycursor == 23)) { Xcursor = 14; Ycursor = 23; }
  else if((Xcursor == 14) && (Ycursor == 23)) { Xcursor = 17; Ycursor = 23; }
  else if((Xcursor == 17) && (Ycursor == 23)) { Xcursor = 20; Ycursor = 23; }
  else if((Xcursor == 20) && (Ycursor == 23)) { Xcursor = 24; Ycursor = 23; }
  else if((Xcursor == 24) && (Ycursor == 23)) { Xcursor =  8; Ycursor = 20; }
}

void Increment(void)	               		/* increment time */
{
  if((Xcursor ==  8) && (Ycursor == 20))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 13) && (Ycursor == 20))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 18) && (Ycursor == 20))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 22) && (Ycursor == 20))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS1307_write(0x03,weekday);
    }
  else if((Xcursor == 10) && (Ycursor == 23))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 23))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 23))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 23))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS1307_write(0x00,second);
    }
}

void Decrement(void)	               		/* decrement time */
{
  if((Xcursor ==  8) && (Ycursor == 20))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 13) && (Ycursor == 20))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 18) && (Ycursor == 20))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 22) && (Ycursor == 20))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS1307_write(0x03,weekday);
    }
  else if((Xcursor == 10) && (Ycursor == 23))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 23))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 23))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 23))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS1307_write(0x00,second);
    }
}

/* ---------------------------------------------------------------------------- */

unsigned char second_old, start_flag;

void Test11(void)				/* Test 11 */
{
  unsigned char key = no_key;

  TFT_clear_screen();				// clear screen
  TFT_portrait();				// portrait mode

  DS1307_initialize();				// initialize DS1307 RTC

  TFT_cursor(Magenta);				// display cursor by Magenta color
  Xcursor = 28;
  Ycursor = 37;

  start_flag = 0;

  TFT_string(4,1, White,Magenta," DS1307 아날로그 시계 "); // display title
  Draw_analog_clock();				// draw analog clock
  TFT_string(8,37, Cyan, Black, "오전");	// display digital clock
  TFT_string(13,37, Green,Black, "00:00:00");

  while(1)
    { key = Key_input();			// key input
      if((key == KEY4) && (Xcursor == 25) && (Ycursor == 37))
        return;

      switch(key)
        { case KEY1 : Analog_cursor_left();	// if KEY1, go cursor left
                      break;
          case KEY2 : Analog_cursor_right();	// if KEY2, go cursor right
                      break;
          case KEY3 : Analog_increment();	// if KEY3, increment time
                      break;
	  case KEY4 : Analog_decrement();	// if KEY4, decrement time
                      break;
	  default   : Analog_display_time();	// display time
	              break;
        }
    }
}

void Analog_increment(void)			/* increment time */
{
  if((Xcursor == 10) && (Ycursor == 37))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 37))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 37))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 37))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS1307_write(0x00,second);
    }
  else if((Xcursor == 11) && (Ycursor == 25))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 14) && (Ycursor == 25))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 17) && (Ycursor == 25))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 19) && (Ycursor == 25))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS1307_write(0x03,weekday);
    }
}

void Analog_decrement(void)			/* decrement time */
{
  if ((Xcursor == 10) && (Ycursor == 37))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 14) && (Ycursor == 37))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 17) && (Ycursor == 37))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 20) && (Ycursor == 37))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS1307_write(0x00,second);
    }
  else if((Xcursor == 11) && (Ycursor == 25))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 14) && (Ycursor == 25))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 17) && (Ycursor == 25))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 19) && (Ycursor == 25))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS1307_write(0x03,weekday);
    }
}

void Analog_cursor_left(void)			/* go cursor left */
{
  if     ((Xcursor == 28) && (Ycursor == 37)) { Xcursor = 20; Ycursor = 37; }
  else if((Xcursor == 20) && (Ycursor == 37)) { Xcursor = 17; Ycursor = 37; }
  else if((Xcursor == 17) && (Ycursor == 37)) { Xcursor = 14; Ycursor = 37; }
  else if((Xcursor == 14) && (Ycursor == 37)) { Xcursor = 10; Ycursor = 37; }
  else if((Xcursor == 10) && (Ycursor == 37)) { Xcursor = 19; Ycursor = 25; }

  else if((Xcursor == 19) && (Ycursor == 25)) { Xcursor = 17; Ycursor = 25; }
  else if((Xcursor == 17) && (Ycursor == 25)) { Xcursor = 14; Ycursor = 25; }
  else if((Xcursor == 14) && (Ycursor == 25)) { Xcursor = 11; Ycursor = 25; }
  else if((Xcursor == 11) && (Ycursor == 25)) { Xcursor = 28; Ycursor = 37; }
}

void Analog_cursor_right(void)			/* go cursor right */
{
  if     ((Xcursor == 28) && (Ycursor == 37)) { Xcursor = 11; Ycursor = 25; }
  else if((Xcursor == 10) && (Ycursor == 37)) { Xcursor = 14; Ycursor = 37; }
  else if((Xcursor == 14) && (Ycursor == 37)) { Xcursor = 17; Ycursor = 37; }
  else if((Xcursor == 17) && (Ycursor == 37)) { Xcursor = 20; Ycursor = 37; }
  else if((Xcursor == 20) && (Ycursor == 37)) { Xcursor = 28; Ycursor = 37; }

  else if((Xcursor == 11) && (Ycursor == 25)) { Xcursor = 14; Ycursor = 25; }
  else if((Xcursor == 14) && (Ycursor == 25)) { Xcursor = 17; Ycursor = 25; }
  else if((Xcursor == 17) && (Ycursor == 25)) { Xcursor = 19; Ycursor = 25; }
  else if((Xcursor == 19) && (Ycursor == 25)) { Xcursor = 10; Ycursor = 37; }
}

unsigned int Xcenter = 120;			// center x,y of analog clock
unsigned int Ycenter = 160;

void Analog_display_time(void)			/* display analog and digital time */
{
  second = DS1307_read(0x00);			// new second ?
  if(second == second_old)
    return;
  else
    second_old = second;

  minute = DS1307_read(0x01);
  hour = DS1307_read(0x02);
  weekday = DS1307_read(0x03);
  date = DS1307_read(0x04);
  month = DS1307_read(0x05);
  year = DS1307_read(0x06);

  TFT_xy(13,37);				// display hour
  TFT_color(Green,Black);
  TFT_hexadecimal(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string(8,37, Cyan,Black, "오전");
  else                      TFT_string(8,37, Cyan,Black, "오후");
  TFT_xy(16,37);				// display minute
  TFT_color(Green,Black);
  TFT_hexadecimal(minute,2);
  TFT_xy(19,37);				// display second
  TFT_color(Green,Black);
  TFT_hexadecimal(second & 0x7F,2);

  TFT_xy(28,37); TFT_color(Green,Black);	// display home
  TFT_English(' ');

  Draw_pin();					// draw hour, minute, second pin
  Circle(Xcenter,Ycenter, 5, Green);

  if((minute == 0x00) && (second == 0x00) && (time_flag == 0))
    { time_flag = 1;
      Beep();
    }
  else if((minute != 0x00) || (second != 0x00))
    time_flag = 0;
}

void Draw_analog_clock(void)			/* draw analog clock */
{
 unsigned int i; 
 unsigned int scale_out = 118;
 unsigned int scale_in1 = 112;
 unsigned int scale_in2 = 106;
 signed int Xstart, Ystart, Xend, Yend;

 for(i = 0; i < 60 ; i++)			// draw clock scale
   if((i % 5) != 0)
     { Xstart = Xcenter + (signed int)(scale_in1*sin(3.141592/180*(i*6)));
       Ystart = Ycenter - (signed int)(scale_in1*cos(3.141592/180*(i*6)));
       Xend   = Xcenter + (signed int)(scale_out*sin(3.141592/180*(i*6)));
       Yend   = Ycenter - (signed int)(scale_out*cos(3.141592/180*(i*6)));

       Line_soft(Xstart,Ystart, Xend,Yend, LightGrey);
     }
 for(i = 0; i < 12 ; i++)
   { Xstart = Xcenter + (signed int)(scale_in2*sin(3.141592/180*(i*30)));
     Ystart = Ycenter - (signed int)(scale_in2*cos(3.141592/180*(i*30)));
     Xend   = Xcenter + (signed int)(scale_out*sin(3.141592/180*(i*30)));
     Yend   = Ycenter - (signed int)(scale_out*cos(3.141592/180*(i*30)));

     Line_soft(Xstart,Ystart, Xend,Yend, White);
   }

  Circle(Xcenter,Ycenter, 119, Cyan);		// draw outline
  Circle(Xcenter,Ycenter, 118, Cyan);

  for(i = 4; i > 0; i--)			// draw center
    Circle(Xcenter,Ycenter, i, Yellow);
}

signed int Xstart1_hour_old, Ystart1_hour_old, Xstart2_hour_old, Ystart2_hour_old;
signed int Xend_hour_old, Yend_hour_old;
signed int Xstart1_minute_old, Ystart1_minute_old, Xstart2_minute_old, Ystart2_minute_old;
signed int Xend_minute_old, Yend_minute_old;
signed int Xstart_second_old, Ystart_second_old, Xend_second_old, Yend_second_old;
signed int Xcircle_second_old, Ycircle_second_old;

void Draw_pin(void)				/* draw hour, minute, second pin */
{
  unsigned int i;
  unsigned int pin_hour, length_hour = 65;
  signed int Xstart1_hour, Ystart1_hour, Xstart2_hour, Ystart2_hour;
  signed int Xend_hour, Yend_hour;

  unsigned int pin_minute, length_minute = 80;
  signed int Xstart1_minute, Ystart1_minute, Xstart2_minute, Ystart2_minute;
  signed int Xend_minute, Yend_minute;

  unsigned int pin_second, length_second = 100, center_second = 80 ;
  signed int Xstart_second, Ystart_second, Xend_second, Yend_second;
  signed int Xcircle_second, Ycircle_second;

  pin_hour = ((hour & 0x10) >> 4)*10 + (hour & 0x0F);	// convert hour to binary
  pin_minute = (minute >> 4)*10 + (minute & 0x0F);	// convert minute to binary
  pin_second = (second >> 4)*10 + (second & 0x0F);	// convert second to binary

						// calculate hour variables
  Xstart1_hour = Xcenter + (signed int)(6*sin((pin_hour*30 + pin_minute/2. - 30)*3.141592/180.)+0.5);
  Ystart1_hour = Ycenter - (signed int)(6*cos((pin_hour*30 + pin_minute/2. - 30)*3.141592/180.)+0.5);

  Xstart2_hour = Xcenter + (signed int)(6*sin((pin_hour*30 + pin_minute/2. + 30)*3.141592/180.)+0.5);
  Ystart2_hour = Ycenter - (signed int)(6*cos((pin_hour*30 + pin_minute/2. + 30)*3.141592/180.)+0.5);

  Xend_hour = Xcenter + (signed int)(length_hour*sin((pin_hour*30 + pin_minute/2.)*3.141592/180.)+0.5);
  Yend_hour = Ycenter - (signed int)(length_hour*cos((pin_hour*30 + pin_minute/2.)*3.141592/180.)+0.5);

						 // calculate minute variables
  Xstart1_minute = Xcenter + (signed int)(6*sin((pin_minute*6 + pin_second/10. - 15)*3.141592/180.)+0.5);
  Ystart1_minute = Ycenter - (signed int)(6*cos((pin_minute*6 + pin_second/10. - 15)*3.141592/180.)+0.5);

  Xstart2_minute = Xcenter + (signed int)(6*sin((pin_minute*6 + pin_second/10. + 15)*3.141592/180.)+0.5);
  Ystart2_minute = Ycenter - (signed int)(6*cos((pin_minute*6 + pin_second/10. + 15)*3.141592/180.)+0.5);

  Xend_minute = Xcenter + (signed int)(length_minute*sin((pin_minute*6 + pin_second/10.)*3.141592/180.)+0.5);
  Yend_minute = Ycenter - (signed int)(length_minute*cos((pin_minute*6 + pin_second/10.)*3.141592/180.)+0.5);

						// calculate second variables
  Xstart_second = Xcenter + (signed int)(6*sin(3.141592/180.*(pin_second*6))+0.5);
  Ystart_second = Ycenter - (signed int)(6*cos(3.141592/180.*(pin_second*6))+0.5);

  Xend_second = Xcenter + (signed int)(length_second*sin(3.141592/180.*(pin_second*6))+0.5);
  Yend_second = Ycenter - (signed int)(length_second*cos(3.141592/180.*(pin_second*6))+0.5);
	
  Xcircle_second = Xcenter + (signed int)(center_second*sin(3.141592/180.*(pin_second*6))+0.5);
  Ycircle_second = Ycenter - (signed int)(center_second*cos(3.141592/180.*(pin_second*6))+0.5);

  if(start_flag == 0)				// initialize old value
    { start_flag = 1;

      Xstart1_hour_old = Xstart1_hour;
      Ystart1_hour_old = Ystart1_hour;
      Xstart2_hour_old = Xstart2_hour;
      Ystart2_hour_old = Ystart2_hour;
      Xend_hour_old = Xend_hour;
      Yend_hour_old = Yend_hour;

      Xstart1_minute_old = Xstart1_minute;
      Ystart1_minute_old = Ystart1_minute;
      Xstart2_minute_old = Xstart2_minute;
      Ystart2_minute_old = Ystart2_minute;
      Xend_minute_old = Xend_minute;
      Yend_minute_old = Yend_minute;

      Xstart_second_old = Xstart_second;
      Ystart_second_old = Ystart_second;
      Xend_second_old = Xend_second;
      Yend_second_old = Yend_second;
      Xcircle_second_old = Xcircle_second;
      Ycircle_second_old = Ycircle_second;
    }

						// delete old hour and minute pin
  if((Xend_hour != Xend_hour_old) || (Yend_hour != Yend_hour_old) || (Xend_minute != Xend_minute_old) || (Yend_minute != Yend_minute_old))
    { Line_soft(Xstart1_hour_old,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart2_hour_old,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);

      Line_soft(Xstart1_hour_old+1,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart1_hour_old-1,Ystart1_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart2_hour_old+1,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart2_hour_old-1,Ystart2_hour_old, Xend_hour_old,Yend_hour_old, Black);

      Line_soft(Xstart1_hour_old,Ystart1_hour_old+1, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart1_hour_old,Ystart1_hour_old-1, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart2_hour_old,Ystart2_hour_old+1, Xend_hour_old,Yend_hour_old, Black);
      Line_soft(Xstart2_hour_old,Ystart2_hour_old-1, Xend_hour_old,Yend_hour_old, Black);

      Line_soft(Xstart1_minute_old,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart2_minute_old,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);

      Line_soft(Xstart1_minute_old+1,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart1_minute_old-1,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart2_minute_old+1,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart2_minute_old-1,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);

      Line_soft(Xstart1_minute_old,Ystart1_minute_old+1, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart1_minute_old,Ystart1_minute_old-1, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart2_minute_old,Ystart2_minute_old+1, Xend_minute_old,Yend_minute_old, Black);
      Line_soft(Xstart2_minute_old,Ystart2_minute_old-1, Xend_minute_old,Yend_minute_old, Black);
    }

  for(i = 3; i > 0; i--)			// delete old second pin
    Circle(Xcircle_second_old,Ycircle_second_old, i, Black);
  Line_soft(Xstart_second_old,Ystart_second_old, Xend_second_old,Yend_second_old, Black);

  TFT_color(Green,Black);			// display hour number
  TFT_English_pixel(112,54, '1'); TFT_English_pixel(120,54, '2');
  TFT_English_pixel(62,68, '1'); TFT_English_pixel(70,68, '1');
  TFT_English_pixel(164,68, '1');
  TFT_English_pixel(28,104, '1'); TFT_English_pixel(36,104, '0');
  TFT_English_pixel(202,104, '2');
  TFT_English_pixel(18,152, '9');
  TFT_English_pixel(216,152, '3');
  TFT_English_pixel(32,202, '8');
  TFT_English_pixel(202,202, '4');
  TFT_English_pixel(68,238, '7');
  TFT_English_pixel(164,238, '5');
  TFT_English_pixel(117,252, '6');

  TFT_string(8,25, Silver,Black, "20");		// display calendar
  TFT_hexadecimal(year,2);
  TFT_English('/');
  TFT_hexadecimal(month,2);
  TFT_English('/');
  TFT_hexadecimal(date,2);
  if(weekday == 0x01)      TFT_string(18,25, Silver,Black, "(일)");
  else if(weekday == 0x02) TFT_string(18,25, Silver,Black, "(월)");
  else if(weekday == 0x03) TFT_string(18,25, Silver,Black, "(화)");
  else if(weekday == 0x04) TFT_string(18,25, Silver,Black, "(수)");
  else if(weekday == 0x05) TFT_string(18,25, Silver,Black, "(목)");
  else if(weekday == 0x06) TFT_string(18,25, Silver,Black, "(금)");
  else if(weekday == 0x07) TFT_string(18,25, Silver,Black, "(토)");

  Rectangle(60,196, 180,218, Green);

  Line_soft(Xstart1_hour,Ystart1_hour, Xend_hour, Yend_hour, Magenta); // draw new hour pin
  Line_soft(Xstart2_hour,Ystart2_hour, Xend_hour, Yend_hour, Magenta);

  Line_soft(Xstart1_minute,Ystart1_minute, Xend_minute,Yend_minute, Cyan); // draw new minute pin
  Line_soft(Xstart2_minute,Ystart2_minute, Xend_minute,Yend_minute, Cyan);

  for(i = 3; i > 0; i--)			// draw new second pin
    Circle(Xcircle_second,Ycircle_second, i, Yellow);
  Line_soft (Xstart_second,Ystart_second, Xend_second,Yend_second, Yellow);

  Xstart1_hour_old = Xstart1_hour;		// store old value
  Ystart1_hour_old = Ystart1_hour;
  Xstart2_hour_old = Xstart2_hour;
  Ystart2_hour_old = Ystart2_hour;
  Xend_hour_old = Xend_hour;
  Yend_hour_old = Yend_hour;

  Xstart1_minute_old = Xstart1_minute;
  Ystart1_minute_old = Ystart1_minute;
  Xstart2_minute_old = Xstart2_minute;
  Ystart2_minute_old = Ystart2_minute;
  Xend_minute_old = Xend_minute;
  Yend_minute_old = Yend_minute;

  Xstart_second_old = Xstart_second;
  Ystart_second_old = Ystart_second;
  Xend_second_old = Xend_second;
  Yend_second_old = Yend_second;
  Xcircle_second_old = Xcircle_second;
  Ycircle_second_old = Ycircle_second;
}

void Line_soft(S16 x1,S16 y1, S16 x2,S16 y2, U16 color)	/* draw a soft line */
{
  int x, y;

  if(y1 != y2)					// if y1 != y2, y is variable
    { if(y1 < y2)				//              x is function
        for(y = y1; y <= y2; y++)
          { x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
            TFT_pixel(x, y, color);
          }
      else
        for(y = y1; y >= y2; y--)
          { x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
            TFT_pixel(x, y, color);
          }
    }
  if(x1 != x2)					// if x1 != x2, x is variable
    { if(x1 < x2)				//              y is function
        for(x = x1; x <= x2; x++)
          { y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
            TFT_pixel(x, y, color);
          }
      else
        for(x = x1; x >= x2; x--)
          { y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
            TFT_pixel(x, y, color);
          }
    }
}

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
