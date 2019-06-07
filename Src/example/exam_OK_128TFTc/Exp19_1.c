/* ========================================================================== */
/*	    Exp19_1.c : DS1307을 이용한 텍스트 LCD용 디지털 시계	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

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

unsigned char cursor, year, month, date, weekday, hour, minute, second;

int main(void)
{
  unsigned char key;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  cursor = 0xCF;				// initial cursor position
  LCD_command(0x0F);				// cursor ON

  LCD_string(0x80,"2010/12/05 (SUN)");		// display title
  LCD_string(0xC0,"  12:00:00 AM   ");

  DS1307_initialize();				// initialize DS1307 RTC

  while(1)
    { key = Key_input();			// key input

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
      Delay_ms(50);
    }
}

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
  LCD_command(0x80);				// display year
  LCD_hexadecimal(0x20, 2);
  year = DS1307_read(0x06);
  LCD_hexadecimal(year,2);
  LCD_command(0x85);				// display month
  month = DS1307_read(0x05);
  LCD_hexadecimal(month,2);
  LCD_command(0x88);				// display date
  date = DS1307_read(0x04);
  LCD_hexadecimal(date,2);
  weekday = DS1307_read(0x03);			// display weekday
  if(weekday == 0x01)      LCD_string(0x8C,"SUN");
  else if(weekday == 0x02) LCD_string(0x8C,"MON");
  else if(weekday == 0x03) LCD_string(0x8C,"TUE");
  else if(weekday == 0x04) LCD_string(0x8C,"WED");
  else if(weekday == 0x05) LCD_string(0x8C,"THU");
  else if(weekday == 0x06) LCD_string(0x8C,"FRI");
  else if(weekday == 0x07) LCD_string(0x8C,"SAT");
  LCD_command(0xC2);				// display hour
  hour = DS1307_read(0x02);
  LCD_hexadecimal(hour & 0x1F,2);
  if((hour & 0x20) == 0x00) LCD_string(0xCB,"AM");
  else                      LCD_string(0xCB,"PM");
  LCD_command(0xC5);				// display minute
  minute = DS1307_read(0x01);
  LCD_hexadecimal(minute,2);
  LCD_command(0xC8);				// display second
  second = DS1307_read(0x00);
  LCD_hexadecimal(second & 0x7F,2);
  LCD_command(cursor);				// go cursor position
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
  if     (cursor == 0xCF) cursor = 0xCC;
  else if(cursor == 0xCC) cursor = 0xC9;
  else if(cursor == 0xC9) cursor = 0xC6;
  else if(cursor == 0xC6) cursor = 0xC3;
  else if(cursor == 0xC3) cursor = 0x8E;
  else if(cursor == 0x8E) cursor = 0x89;
  else if(cursor == 0x89) cursor = 0x86;
  else if(cursor == 0x86) cursor = 0x83;
  else if(cursor == 0x83) cursor = 0xCF;
}

void Cursor_right(void)             		/* go cursor right */
{
  if     (cursor == 0xCF) cursor = 0x83;
  else if(cursor == 0x83) cursor = 0x86;
  else if(cursor == 0x86) cursor = 0x89;
  else if(cursor == 0x89) cursor = 0x8E;
  else if(cursor == 0x8E) cursor = 0xC3;
  else if(cursor == 0xC3) cursor = 0xC6;
  else if(cursor == 0xC6) cursor = 0xC9;
  else if(cursor == 0xC9) cursor = 0xCC;
  else if(cursor == 0xCC) cursor = 0xCF;
}

void Increment(void)	               		/* increment time */
{
  if(cursor == 0x83)				// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x99)      year = 0x00;
      else                  year = BCD_increment(year);
      DS1307_write(0x06,year);
    }
  else if(cursor == 0x86)			// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x12)     month = 0x01;
      else                  month = BCD_increment(month);
      DS1307_write(0x05,month);
    }
  else if(cursor == 0x89)			// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x31)      date = 0x01;
      else                  date = BCD_increment(date);
      DS1307_write(0x04,date);
    }
  else if(cursor == 0x8E)			// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x07)   weekday = 0x01;
      else                  weekday++;
      DS1307_write(0x03,weekday);
    }
  else if(cursor == 0xC3)			// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x12) hour = (hour & 0xE0) + 0x01;
      else                      hour = BCD_increment(hour);
      DS1307_write(0x02,hour);
    }
  else if(cursor == 0xC6)			// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x59)    minute = 0x00;
      else                  minute = BCD_increment(minute);
      DS1307_write(0x01,minute);
    }
  else if(cursor == 0xC9)			// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x59)    second = 0x00;
      else                  second = BCD_increment(second);
      DS1307_write(0x00,second);
    }
  else if(cursor == 0xCC)			// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
}

void Decrement(void)	               		/* decrement time */
{
  if(cursor == 0x83)				// in case of year
    { year = DS1307_read(0x06);
      if(year == 0x00)      year = 0x99;
      else                  year = BCD_decrement(year);
      DS1307_write(0x06,year);
    }
  else if(cursor == 0x86)			// in case of month
    { month = DS1307_read(0x05);
      if(month == 0x01)     month = 0x12;
      else                  month = BCD_decrement(month);
      DS1307_write(0x05,month);
    }
  else if(cursor == 0x89)			// in case of date
    { date = DS1307_read(0x04);
      if(date == 0x01)      date = 0x31;
      else                  date = BCD_decrement(date);
      DS1307_write(0x04,date);
    }
  else if(cursor == 0x8E)			// in case of weekday
    { weekday = DS1307_read(0x03);
      if(weekday == 0x01)   weekday = 0x07;
      else                  weekday--;
      DS1307_write(0x03,weekday);
    }
  else if(cursor == 0xC3)			// in case of hour
    { hour = DS1307_read(0x02);
      if((hour & 0x1F) == 0x01) hour = (hour & 0xE0) + 0x12;
      else                      hour = BCD_decrement(hour);
      DS1307_write(0x02,hour);
    }
  else if(cursor == 0xC6)			// in case of minute
    { minute = DS1307_read(0x01);
      if(minute == 0x00)    minute = 0x59;
      else                  minute = BCD_decrement(minute);
      DS1307_write(0x01,minute);
    }
  else if(cursor == 0xC9)			// in case of second
    { second = DS1307_read(0x00);
      if(second == 0x00)    second = 0x59;
      else                  second = BCD_decrement(second);
      DS1307_write(0x00,second);
    }
  else if(cursor == 0xCC)			// in case of AM/PM
    { hour = DS1307_read(0x02);
      if((hour & 0x20) == 0x00) hour |= 0x20;
      else                      hour &= 0xDF;
      DS1307_write(0x02,hour);
    }
}
