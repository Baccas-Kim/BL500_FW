/* ========================================================================== */
/*	Exp19_3.c : DS1307을 이용한 TFT-LCD용 디지털 시계(터치 스크린 사용)   */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"
#include "OK-128TFT_large.h"

void DS1307_initialize(void);			// initialize DS1307 RTC
unsigned char DS1307_read(U08 address);		// read DS1307
void DS1307_write(U08 address, U08 TxD);	// write DS1307
void Display_time(void);			// display DS1307 time
unsigned char BCD_increment(U08 number);	// BCD increment
unsigned char BCD_decrement(U08 number);	// BCD decrement
void Cursor_left(void);				// go cursor left
void Cursor_right(void);             		// go cursor right
void Increment(void);	               		// increment time
void Decrement(void);	               		// decrement time

unsigned char Icon_input(void);			// input touch screen icon
void Icon_draw(U16 color1, U16 color2);		// draw from icon1 to icon4
void Icon1_draw(U16 color1, U16 color2);	// display icon1
void Icon2_draw(U16 color1, U16 color2);	// display icon2
void Icon3_draw(U16 color1, U16 color2);	// display icon3
void Icon4_draw(U16 color1, U16 color2);	// display icon4

unsigned char year, month, date, weekday, hour, minute, second;

int main(void)
{
  unsigned char key;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp19_3.c    ");

  TFT_cursor(Magenta);				// display cursor by Magenta color
  Xcursor = 17;
  Ycursor = 21;

  TFT_string( 4, 6,White, Magenta," 디지털 시계(DS1307) ");	// display title
  TFT_string_large( 2,16,Cyan,  Black,  "2010/06/30 ");
  TFT_string_large(13,16,Yellow,Black,  "(수)");
  TFT_string_large( 3,21,White, Black,  "오전 ");
  TFT_string_large( 8,21,Green, Black,  "00:00:00");

  Icon_draw(Cyan,Magenta);			// display touch screen icon

  Initialize_touch_screen();			// initialize touch screen
  DS1307_initialize();				// initialize DS1307 RTC

  while(1)
    { key = Key_input();			// key input
      if(key == no_key)				// if no key, input touch screen icon
        key = Icon_input();

      switch(key)
        { case KEY1 : Cursor_left();		// if KEY1, go cursor left
                      break;
          case KEY2 : Cursor_right();		// if KEY2, go cursor right
                      break;
          case KEY3 : Increment();		// if KEY3, increment time
                      break;
	  case KEY4 : Decrement();		// if KEY4, decrement time
                      break;
          default   : Display_time();		// display time
	              break;
        }
    }
}

/* ---------------------------------------------------------------------------- */
/*		디지털 시계 관련 함수						*/
/* ---------------------------------------------------------------------------- */

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

void Display_time(void)				/* display DS1307 time */
{
  TFT_xy(2,16); TFT_color(Cyan,Black);		// display year
  TFT_hexadecimal_large(0x20, 2);
  year = DS1307_read(0x06);
  TFT_hexadecimal_large(year,2);
  TFT_xy(7,16); TFT_color(Cyan,Black);		// display month
  month = DS1307_read(0x05);
  TFT_hexadecimal_large(month,2);
  TFT_xy(10,16); TFT_color(Cyan,Black);		// display date
  date = DS1307_read(0x04);
  TFT_hexadecimal_large(date,2);
  weekday = DS1307_read(0x03);			// display weekday
  if(weekday == 0x01)      TFT_string_large(14,16, Yellow,Black, "일");
  else if(weekday == 0x02) TFT_string_large(14,16, Yellow,Black, "월");
  else if(weekday == 0x03) TFT_string_large(14,16, Yellow,Black, "화");
  else if(weekday == 0x04) TFT_string_large(14,16, Yellow,Black, "수");
  else if(weekday == 0x05) TFT_string_large(14,16, Yellow,Black, "목");
  else if(weekday == 0x06) TFT_string_large(14,16, Yellow,Black, "금");
  else if(weekday == 0x07) TFT_string_large(14,16, Yellow,Black, "토");
  TFT_xy(8,21); TFT_color(Green,Black);		// display hour
  hour = DS1307_read(0x02);
  TFT_hexadecimal_large(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string_large(5,21, White,Black, "전");
  else                      TFT_string_large(5,21, White,Black, "후");
  TFT_xy(11,21); TFT_color(Green,Black);	// display minute
  minute = DS1307_read(0x01);
  TFT_hexadecimal_large(minute,2);
  TFT_xy(14,21); TFT_color(Green,Black);	// display second
  second = DS1307_read(0x00);
  TFT_hexadecimal_large(second & 0x7F,2);
  TFT_xy(17,21); TFT_color(Green,Black);	// display home
  TFT_English_large(' ');
}

unsigned char BCD_increment(U08 number)		/* BCD increment */
{
  unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i++;                                          // incrment
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

unsigned char BCD_decrement(U08 number)		/* BCD decrement */
{
  unsigned char i;

  i = (number >> 4)*10 + (number & 0x0F);       // convert BCD to binary
  i--;                                          // decrement
  i = ((i/10) << 4) + (i % 10);                 // convert binary to BCD
  return i;
}

void Cursor_left(void)				/* go cursor left */
{
  if     ((Xcursor == 17) && (Ycursor == 21)) { Xcursor = 14; Ycursor = 21; }
  else if((Xcursor == 14) && (Ycursor == 21)) { Xcursor = 11; Ycursor = 21; }
  else if((Xcursor == 11) && (Ycursor == 21)) { Xcursor =  8; Ycursor = 21; }
  else if((Xcursor ==  8) && (Ycursor == 21)) { Xcursor =  5; Ycursor = 21; }
  else if((Xcursor ==  5) && (Ycursor == 21)) { Xcursor = 14; Ycursor = 16; }
  else if((Xcursor == 14) && (Ycursor == 16)) { Xcursor = 11; Ycursor = 16; }
  else if((Xcursor == 11) && (Ycursor == 16)) { Xcursor =  8; Ycursor = 16; }
  else if((Xcursor ==  8) && (Ycursor == 16)) { Xcursor =  5; Ycursor = 16; }
  else if((Xcursor ==  5) && (Ycursor == 16)) { Xcursor = 17; Ycursor = 21; }
}

void Cursor_right(void)             		/* go cursor right */
{
  if     ((Xcursor == 17) && (Ycursor == 21)) { Xcursor =  5; Ycursor = 16; }
  else if((Xcursor ==  5) && (Ycursor == 16)) { Xcursor =  8; Ycursor = 16; }
  else if((Xcursor ==  8) && (Ycursor == 16)) { Xcursor = 11; Ycursor = 16; }
  else if((Xcursor == 11) && (Ycursor == 16)) { Xcursor = 14; Ycursor = 16; }
  else if((Xcursor == 14) && (Ycursor == 16)) { Xcursor =  5; Ycursor = 21; }
  else if((Xcursor ==  5) && (Ycursor == 21)) { Xcursor =  8; Ycursor = 21; }
  else if((Xcursor ==  8) && (Ycursor == 21)) { Xcursor = 11; Ycursor = 21; }
  else if((Xcursor == 11) && (Ycursor == 21)) { Xcursor = 14; Ycursor = 21; }
  else if((Xcursor == 14) && (Ycursor == 21)) { Xcursor = 17; Ycursor = 21; }
}

void Increment(void)	               		/* increment time */
{
  if((Xcursor ==  5) && (Ycursor == 16))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 8) && (Ycursor == 16))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 11) && (Ycursor == 16))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 14) && (Ycursor == 16))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS1307_write(0x03,weekday);
    }
  else if((Xcursor == 5) && (Ycursor == 21))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 8) && (Ycursor == 21))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 11) && (Ycursor == 21))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 14) && (Ycursor == 21))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS1307_write(0x00,second);
    }
}

void Decrement(void)	               		/* decrement time */
{
  if((Xcursor ==  5) && (Ycursor == 16))	// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS1307_write(0x06,year);
    }
  else if((Xcursor == 8) && (Ycursor == 16))	// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS1307_write(0x05,month);
    }
  else if((Xcursor == 11) && (Ycursor == 16))	// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS1307_write(0x04,date);
    }
  else if((Xcursor == 14) && (Ycursor == 16))	// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS1307_write(0x03,weekday);
    }
  else if((Xcursor == 5) && (Ycursor == 21))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 8) && (Ycursor == 21))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 11) && (Ycursor == 21))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 14) && (Ycursor == 21))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS1307_write(0x00,second);
    }
}

/* ---------------------------------------------------------------------------- */
/*		터치 스크린 관련 함수						*/
/* ---------------------------------------------------------------------------- */

unsigned char icon_flag = 0;

unsigned char Icon_input(void)			/* input touch screen icon */
{
  unsigned char icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 13) && (x_touch <= 57) && (y_touch >= 277) && (y_touch <= 315))
    { icon = KEY1;
      icon_flag = 1;
      Icon1_draw(Magenta,Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 69) && (x_touch <= 114) && (y_touch >= 277) && (y_touch <= 315))
    { icon = KEY2;
      icon_flag = 1;
      Icon2_draw(Magenta,Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 125) && (x_touch <= 170) && (y_touch >= 277) && (y_touch <= 315))
    { icon = KEY3;
      icon_flag = 1;
      Icon3_draw(Magenta,Magenta);
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 181) && (x_touch <= 226) && (y_touch >= 277) && (y_touch <= 315))
    { icon = KEY4;
      icon_flag = 1;
      Icon4_draw(Magenta,Magenta);
      Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_key;
      icon_flag = 0;
      Icon_draw(Cyan,Magenta);
    }
  else
    icon = no_key;

  return icon;
}

void Icon_draw(U16 color1, U16 color2)		/* draw from icon1 to icon4 */
{
  Icon1_draw(color1, color2);
  Icon2_draw(color1, color2);
  Icon3_draw(color1, color2);
  Icon4_draw(color1, color2);
}

void Icon1_draw(U16 color1, U16 color2)		/* display icon1 */
{
  Rectangle( 13,277,  58,315, color1);
  TFT_string( 3,35, color1,Black, "KEY1");
  TFT_string( 3,37, color2,Black, "( )");
  TFT_xy( 4,37);
  TFT_English(0x1B);
}

void Icon2_draw(U16 color1, U16 color2)		/* display icon2 */
{
  Rectangle( 69,277, 114,315, color1);
  TFT_string(10,35, color1,Black, "KEY2");
  TFT_string(10,37, color2,Black, "( )");
  TFT_xy(11,37);
  TFT_English(0x1A);
}

void Icon3_draw(U16 color1, U16 color2)		/* display icon3 */
{
  Rectangle(125,277, 170,315, color1);
  TFT_string(17,35, color1,Black, "KEY3");
  TFT_string(17,37, color2,Black, "( )");
  TFT_xy(18,37);
  TFT_English(0x18);
}

void Icon4_draw(U16 color1, U16 color2)		/* display icon4 */
{
  Rectangle(181,277, 226,315, color1);
  TFT_string(24,35, color1,Black, "KEY4");
  TFT_string(24,37, color2,Black, "( )");
  TFT_xy(25,37);
  TFT_English(0x19);
}
