/* ========================================================================== */
/*	Exp19_4.c : DS1307을 이용한 아날로그/디지털 시계 프로그램	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"
#include "OK-128TFT_large.h"

void DS1307_initialize(void);			/* initialize DS1307 RTC */
unsigned char DS1307_read(U08 address);		/* read DS1307 */
void DS1307_write(U08 address, U08 TxD);	/* write DS1307 */
void Increment(void);	               		/* increment time */
void Decrement(void);	               		/* decrement time */
unsigned char BCD_increment(U08 number);	/* BCD increment */
unsigned char BCD_decrement(U08 number);	/* BCD decrement */
void Cursor_left(void);				/* go cursor left */
void Cursor_right(void);             		/* go cursor right */

void Display_time(void);			/* display analog and digital time */
void Draw_analog_clock(void);			/* draw analog clock */
void Draw_pin(void);				/* draw hour, minute, second pin */
void Line_soft(S16 x1,S16 y1, S16 x2,S16 y2, U16 color); /* draw a soft line */
void TFT_English_pixel(U16 Xpixel,U16 Ypixel, U08 code); /* write a English ASCII character */

unsigned char year, month, date, weekday, hour, minute, second, second_old, start_flag;

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp19_4.c    ");

  TFT_cursor(Magenta);				// display cursor by Magenta color
  Xcursor = 18;
  Ycursor = 36;

  start_flag = 0;

  TFT_string(3,1, White,Magenta," 아날로그 / 디지털 시계 "); // display title
  Draw_analog_clock();				// draw analog clock
  TFT_string_large(3,36, Cyan, Black, "오전");	// display digital clock
  TFT_string_large(8,36, Green,Black, "00:00:00");

  DS1307_initialize();				// initialize DS1307 RTC

  while(1)
    { switch(Key_input())			// key input
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

void Increment(void)	               		/* increment time */
{
  if((Xcursor == 5) && (Ycursor == 36))		// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 9) && (Ycursor == 36))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 12) && (Ycursor == 36))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 15) && (Ycursor == 36))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS1307_write(0x00,second);
    }
  else if((Xcursor ==  11) && (Ycursor == 25))	// in case of year
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

void Decrement(void)	               		/* decrement time */
{
  if ((Xcursor == 5) && (Ycursor == 36))	// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 9) && (Ycursor == 36))	// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS1307_write(0x02,hour);
    }
  else if((Xcursor == 12) && (Ycursor == 36))	// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS1307_write(0x01,minute);
    }
  else if((Xcursor == 15) && (Ycursor == 36))	// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS1307_write(0x00,second);
    }
  else if((Xcursor ==  11) && (Ycursor == 25))	// in case of year
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
  if     ((Xcursor == 18) && (Ycursor == 36)) { Xcursor = 15; Ycursor = 36; }
  else if((Xcursor == 15) && (Ycursor == 36)) { Xcursor = 12; Ycursor = 36; }
  else if((Xcursor == 12) && (Ycursor == 36)) { Xcursor =  9; Ycursor = 36; }
  else if((Xcursor ==  9) && (Ycursor == 36)) { Xcursor =  5; Ycursor = 36; }
  else if((Xcursor ==  5) && (Ycursor == 36)) { Xcursor = 19; Ycursor = 25; }

  else if((Xcursor == 19) && (Ycursor == 25)) { Xcursor = 17; Ycursor = 25; }
  else if((Xcursor == 17) && (Ycursor == 25)) { Xcursor = 14; Ycursor = 25; }
  else if((Xcursor == 14) && (Ycursor == 25)) { Xcursor = 11; Ycursor = 25; }
  else if((Xcursor == 11) && (Ycursor == 25)) { Xcursor = 18; Ycursor = 36; }
}

void Cursor_right(void)             		/* go cursor right */
{
  if     ((Xcursor == 18) && (Ycursor == 36)) { Xcursor = 11; Ycursor = 25; }
  else if((Xcursor ==  5) && (Ycursor == 36)) { Xcursor =  9; Ycursor = 36; }
  else if((Xcursor ==  9) && (Ycursor == 36)) { Xcursor = 12; Ycursor = 36; }
  else if((Xcursor == 12) && (Ycursor == 36)) { Xcursor = 15; Ycursor = 36; }
  else if((Xcursor == 15) && (Ycursor == 36)) { Xcursor = 18; Ycursor = 36; }

  else if((Xcursor == 11) && (Ycursor == 25)) { Xcursor = 14; Ycursor = 25; }
  else if((Xcursor == 14) && (Ycursor == 25)) { Xcursor = 17; Ycursor = 25; }
  else if((Xcursor == 17) && (Ycursor == 25)) { Xcursor = 19; Ycursor = 25; }
  else if((Xcursor == 19) && (Ycursor == 25)) { Xcursor =  5; Ycursor = 36; }
}

/* ---------------------------------------------------------------------------- */
/*		아날로그 시계 관련 함수						*/
/* ---------------------------------------------------------------------------- */

unsigned int Xcenter = 120;			// center x,y of analog clock
unsigned int Ycenter = 160;

void Display_time(void)				/* display analog and digital time */
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

  TFT_xy(8,36);					// display hour
  TFT_color(Green,Black);
  TFT_hexadecimal_large(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) TFT_string_large(5,36, Cyan,Black, "전");
  else                      TFT_string_large(5,36, Cyan,Black, "후");
  TFT_xy(11,36);				// display minute
  TFT_color(Green,Black);
  TFT_hexadecimal_large(minute,2);
  TFT_xy(14,36);				// display second
  TFT_color(Green,Black);
  TFT_hexadecimal_large(second & 0x7F,2);

  TFT_xy(18,36); TFT_color(Green,Black);	// display home
  TFT_English_large(' ');

  Draw_pin();					// draw hour, minute, second pin
  Circle(Xcenter,Ycenter, 5, Green);
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
     { Xstart = Xcenter + scale_in1*sin(3.141592/180*(i*6));
       Ystart = Ycenter - scale_in1*cos(3.141592/180*(i*6));
       Xend   = Xcenter + scale_out*sin(3.141592/180*(i*6));
       Yend   = Ycenter - scale_out*cos(3.141592/180*(i*6));

       Line_soft(Xstart,Ystart, Xend,Yend, LightGrey);
     }
 for(i = 0; i < 12 ; i++)
   { Xstart = Xcenter + scale_in2*sin(3.141592/180*(i*30));
     Ystart = Ycenter - scale_in2*cos(3.141592/180*(i*30));
     Xend   = Xcenter + scale_out*sin(3.141592/180*(i*30));
     Yend   = Ycenter - scale_out*cos(3.141592/180*(i*30));

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

  Xstart1_hour = Xcenter + 6*sin((pin_hour*30 + pin_minute/2 - 30)*3.141592/180); // calculate hour variables
  Ystart1_hour = Ycenter - 6*cos((pin_hour*30 + pin_minute/2 - 30)*3.141592/180);

  Xstart2_hour = Xcenter + 6*sin((pin_hour*30 + pin_minute/2 + 30)*3.141592/180);
  Ystart2_hour = Ycenter - 6*cos((pin_hour*30 + pin_minute/2 + 30)*3.141592/180);

  Xend_hour = Xcenter + length_hour*sin((pin_hour*30 + pin_minute/2)*3.141592/180);
  Yend_hour = Ycenter - length_hour*cos((pin_hour*30 + pin_minute/2)*3.141592/180);

  Xstart1_minute = Xcenter + 6*sin((pin_minute*6 + pin_second/10 - 15)*3.141592/180); // calculate minute variables
  Ystart1_minute = Ycenter - 6*cos((pin_minute*6 + pin_second/10 - 15)*3.141592/180);

  Xstart2_minute = Xcenter + 6*sin((pin_minute*6 + pin_second/10 + 15)*3.141592/180);
  Ystart2_minute = Ycenter - 6*cos((pin_minute*6 + pin_second/10 + 15)*3.141592/180);

  Xend_minute = Xcenter + length_minute*sin((pin_minute*6 + pin_second/10)*3.141592/180);
  Yend_minute = Ycenter - length_minute*cos((pin_minute*6 + pin_second/10)*3.141592/180);

  Xstart_second = Xcenter + 6*sin(3.141592/180*(pin_second*6)); // calculate second variables
  Ystart_second = Ycenter - 6*cos(3.141592/180*(pin_second*6));

  Xend_second = Xcenter + length_second*sin(3.141592/180*(pin_second*6));
  Yend_second = Ycenter - length_second*cos(3.141592/180*(pin_second*6));
	
  Xcircle_second = Xcenter + center_second*sin(3.141592/180*(pin_second*6));
  Ycircle_second = Ycenter - center_second*cos(3.141592/180*(pin_second*6));

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

      Line_soft (Xstart1_minute_old,Ystart1_minute_old, Xend_minute_old,Yend_minute_old, Black);
      Line_soft (Xstart2_minute_old,Ystart2_minute_old, Xend_minute_old,Yend_minute_old, Black);
    }

  for(i = 3; i > 0; i--)			// delete old second pin
    Circle(Xcircle_second_old,Ycircle_second_old, i, Black);
  Line_soft (Xstart_second_old,Ystart_second_old, Xend_second_old,Yend_second_old, Black);

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
          { x = x1 + (long)(y - y1)*(long)(x2 - x1)/(y2 - y1);
            TFT_pixel(x, y, color);
          }
      else
        for(y = y1; y >= y2; y--)
          { x = x1 + (long)(y - y1)*(long)(x2 - x1)/(y2 - y1);
            TFT_pixel(x, y, color);
          }
    }
  if(x1 != x2)					// if x1 != x2, x is variable
    { if(x1 < x2)				//              y is function
        for(x = x1; x <= x2; x++)
          { y = y1 + (long)(x - x1)*(long)(y2 - y1)/(x2 - x1);
            TFT_pixel(x, y, color);
          }
      else
        for(x = x1; x >= x2; x--)
          { y = y1 + (long)(x - x1)*(long)(y2 - y1)/(x2 - x1);
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
