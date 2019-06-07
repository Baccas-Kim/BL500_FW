/* ============================================================================ */
/*	OK-128LCD.h : OK-128TFT 키트 기본 헤더파일(16x2 텍스트 LCD 모듈용)	*/
/* ============================================================================ */
/*                          Designed and programmed by Duck-Yong Yoon in 2010.  */

typedef unsigned char	U08;			/* data type definition */
typedef   signed char	S08;
typedef unsigned int	U16;
typedef   signed int	S16;
typedef unsigned long	U32;
typedef   signed long	S32;

#define BV(bit)		(1<<(bit))		/* bit processing */
#define cbi(reg,bit)	reg &= ~(BV(bit))
#define sbi(reg,bit)	reg |= (BV(bit))

/* ---------------------------------------------------------------------------- */
/*		사용자 정의 함수 선언						*/
/* ---------------------------------------------------------------------------- */

void MCU_initialize(void);			// initialize ATmega128A MCU 
void Delay_us(U08 time_us);			// time delay for us (1~255 us)
void Delay_ms(U16 time_ms);			// time delay for ms (1~65535 ms)
void Beep(void);				// beep for 50 ms
void Beep_3times(void);				// beep 3 times
unsigned char Key_input(void);			// input key KEY1 - KEY4
unsigned char Key_input_silent(void);		// input key KEY1 - KEY4 without sound

void LCD_initialize(void);			// initialize text LCD module
void LCD_command(U08 command);			// write a command(instruction) to text LCD
void LCD_data(U08 data);			// display a character on text LCD
void LCD_string(U08 command, char *string);	// display a string on LCD

void LCD_binary(U16 number, U08 digit);		// display 8 or 16 bit binary number
void LCD_unsigned_decimal(U32 number, U08 zerofill, U08 digit); // display unsigned decimal number
void LCD_signed_decimal(S32 number, U08 zerofill, U08 digit);	// display signed decimal number
void LCD_hexadecimal(U32 number, U08 digit);	// display hexadecimal number
void LCD_0x_hexadecimal(U32 number, U08 digit);	// display hexadecimal number with 0x
void LCD_unsigned_float(float number, U08 integral, U08 fractional); // display unsigned floating-point number
void LCD_signed_float(float number, U08 integral, U08 fractional);   // display signed floating-point number

/* ---------------------------------------------------------------------------- */
/*		OK-128TFT 키트의 기본 함수					*/
/* ---------------------------------------------------------------------------- */

void MCU_initialize(void)			/* initialize ATmega128A MCU */
{
  DDRA  = 0xFF;					// PORTA output
  PORTA = 0x00;

  DDRB  = 0xF7;					// PORTB output(PB3 = input)
  PORTB = 0xC0;					// (-TS_CS = -SD_CS = 1)

  DDRC  = 0xFF;					// PORTC output
  PORTC = 0x58;					// (-TFT_CS = -TFT_WR = -TFT_HB_WR = 1)

  DDRD  = 0xF9;					// PORTD output(PD2,1 = input)
  PORTD = 0x00;

  DDRE  = 0x39;					// PORTE output(PE7,6,2,1 = input)
  PORTE = 0x00;

  DDRF  = 0x00;					// PORTF input
  PORTF = 0x00;

  DDRG  = 0x00;					// PORTG input
  PORTG = 0x00;
}

void Delay_us(U08 time_us)			/* time delay for us (1~255 us) */
{
  register unsigned char i;

  for(i = 0; i < time_us; i++)			// 4 cycles
    { asm volatile("PUSH R0");			// 2 cycles
      asm volatile("POP  R0");			// 2 cycles
      asm volatile("PUSH R0");			// 2 cycles
      asm volatile("POP  R0");			// 2 cycles
      asm volatile("PUSH R0");			// 2 cycles
      asm volatile("POP  R0");			// 2 cycles = 16 cycles for 16MHz
    }
}

void Delay_ms(U16 time_ms)			/* time delay for ms (1~65535 ms) */
{
  register unsigned int i;

  for(i = 0; i < time_ms; i++)
    { Delay_us(250);
      Delay_us(250);
      Delay_us(250);
      Delay_us(250);
    }
}

void Beep(void)					/* beep for 50 ms */
{ 
  sbi(PORTC,2);					// buzzer on
  Delay_ms(50);					// delay 50 ms
  cbi(PORTC,2);					// buzzer off
}

void Beep_3times(void)				/* beep 3 times */
{ 
  Beep();					// beep for 50 ms
  Delay_ms(100);
  Beep();					// beep for 50 ms
  Delay_ms(100);
  Beep();					// beep for 50 ms
}

/* ---------------------------------------------------------------------------- */
/*		키입력 함수							*/
/* ---------------------------------------------------------------------------- */

#define KEY1		0x0E			// KEY1 value
#define KEY2		0x0D			// KEY2 value
#define KEY3		0x0B			// KEY3 value
#define KEY4		0x07			// KEY4 value
#define no_key		0x0F			// no key input

unsigned char key_flag = 0;

unsigned char Key_input(void)			/* input key KEY1 - KEY4 */
{
  unsigned char key;

  key = PING & 0x0F;				// any key pressed ?
  if(key == 0x0F)				// if no key, check key off
    { if(key_flag == 0)
        return key;
      else
        { Delay_ms(20);
          key_flag = 0;
          return key;
        }
    }
  else						// if key input, check continuous key
    { if(key_flag != 0)				// if continuous key, treat as no key input
        return 0x0F;
      else					// if new key, beep and delay for debounce
        { Beep();
          key_flag = 1;
          return key;
        }
    }
}

unsigned char Key_input_silent(void)		/* input key KEY1 - KEY4 without sound */
{
  unsigned char key;

  key = PING & 0x0F;				// any key pressed ?
  if(key == 0x0F)				// if no key, check key off
    { if(key_flag == 0)
        return key;
      else
        { Delay_ms(20);
          key_flag = 0;
          return key;
        }
    }
  else						// if key input, check continuous key
    { if(key_flag != 0)				// if continuous key, treat as no key input
        return 0x0F;
      else					// if new key, delay for debounce
        { Delay_ms(20);
          key_flag = 1;
          return key;
        }
    }
}

/* ---------------------------------------------------------------------------- */
/*		텍스트 LCD 모듈 관련 함수					*/
/* ---------------------------------------------------------------------------- */

void LCD_initialize(void)			/* initialize text LCD module */
{
  LCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
  LCD_command(0x0C);				// display control(display ON, cursor OFF)
  LCD_command(0x06);				// entry mode set(increment, not shift)
  LCD_command(0x01);				// clear display
  Delay_ms(2);
}

void LCD_command(U08 command)			/* write a command(instruction) to text LCD */
{
  cbi(PORTC,0);					// E = 0, Rs = 0
  cbi(PORTC,1);
  PORTA = command;				// output command
  sbi(PORTC,0);					// E = 1
  asm volatile(" PUSH  R0 ");			// delay for about 250 ns
  asm volatile(" POP   R0 ");
  cbi(PORTC,0);					// E = 0
  Delay_us(50);
}

void LCD_data(U08 data)				/* display a character on text LCD */
{
  cbi(PORTC,0);					// E = 0, Rs = 1
  sbi(PORTC,1);
  PORTA = data;					// output data
  sbi(PORTC,0);					// E = 1
  asm volatile(" PUSH  R0 ");			// delay for about 250 ns
  asm volatile(" POP   R0 ");
  cbi(PORTC,0);					// E = 0
  Delay_us(50);
}

void LCD_string(U08 command, char *string)	/* display a string on LCD */
{
  LCD_command(command);				// start position of string
  while(*string != '\0')			// display string
    { LCD_data(*string);
       string++;
    }
}

/* ---------------------------------------------------------------------------- */
/*		텍스트 LCD 모듈의 수치 데이터 출력 함수				*/
/* ---------------------------------------------------------------------------- */

void LCD_binary(U16 number, U08 digit)		/* display 8 or 16 bit binary number */
{
  unsigned char i, shift;

  if((digit == 0) || (digit > 16)) return;

  shift = digit - 1;
  for(i = 0; i < digit; i++ )
    { LCD_data(((number >> shift) & 0x01) + '0');
      shift--;
    }
}

void LCD_unsigned_decimal(U32 number, U08 zerofill, U08 digit) /* display unsigned decimal number */
{
  unsigned char zero_flag, character;
  unsigned long div;

  if((digit == 0) || (digit > 9)) return;

  div = 1;
  while(--digit) div *= 10;

  zero_flag = zerofill;
  while(div > 0)				// display number
    { character = number / div;
      if((character == 0) && (zero_flag == 0) && (div != 1))
        LCD_data(character + ' ');
      else
        { zero_flag = 1;
          LCD_data(character + '0');
        }
      number %= div;
      div /= 10;
    }
}

void LCD_signed_decimal(S32 number, U08 zerofill, U08 digit)	/* display signed decimal number */
{
  unsigned char zero_flag, character;
  unsigned long div;

  if((digit == 0) || (digit > 9)) return;

  if(number >= 0)				// display sign
    LCD_data('+');
  else
    { LCD_data('-');
      number = -number;
    }

  div = 1;
  while(--digit) div *= 10;

  zero_flag = zerofill;
  while(div > 0)				// display number
    { character = number / div;
      if((character == 0) && (zero_flag == 0) && (div != 1))
        LCD_data(character + ' ');
      else
        { zero_flag = 1;
          LCD_data(character + '0');
        }
      number %= div;
      div /= 10;
    }
}

void LCD_hexadecimal(U32 number, U08 digit)	/* display hexadecimal number */
{
  unsigned char i, character;

  if((digit == 0) || (digit > 8)) return;

  for(i = digit; i > 0; i--)
    { character = (number >> 4*(i-1)) & 0x0F;
      if(character < 10) LCD_data(character + '0');
      else               LCD_data(character - 10 + 'A');
    }
}

void LCD_0x_hexadecimal(U32 number, U08 digit)	/* display hexadecimal number with 0x */
{
  unsigned char i, character;

  if((digit == 0) || (digit > 8)) return;

  LCD_data('0');
  LCD_data('x');

  for(i = digit; i > 0; i--)
    { character = (number >> 4*(i-1)) & 0x0F;
      if(character < 10) LCD_data(character + '0');
      else               LCD_data(character - 10 + 'A');
    }
}

void LCD_unsigned_float(float number, U08 integral, U08 fractional) /* display unsigned floating-point number */
{
  unsigned char zero_flag, digit, character;	// integral = digits of integral part
  unsigned long div, integer;			// fractional = digits of fractional part

  digit = integral + fractional;
  if((integral == 0) || (fractional == 0) || (digit > 9)) return;

  div = 1;
  while(--digit) div *= 10;

  while(fractional--) number *= 10.;
  integer = (U32)(number + 0.5);

  zero_flag = 0;
  digit = 1;
  while(div > 0)				// display number
    { character = integer / div;
      if((character == 0) && (zero_flag == 0) && (digit != integral))
        LCD_data(character + ' ');
      else
        { zero_flag = 1;
          LCD_data(character + '0');
        }
      integer %= div;
      div /= 10;

      if(digit == integral)
        LCD_data('.');
      digit++;
    }
}

void LCD_signed_float(float number, U08 integral, U08 fractional) /* display signed floating-point number */
{
  unsigned char zero_flag, digit, character;
  unsigned long div, integer;

  digit = integral + fractional;
  if((integral == 0) || (fractional == 0) || (digit > 9)) return;

  if(number >= 0)				// display sign
    LCD_data('+');
  else
    { LCD_data('-');
      number = -number;
    }

  div = 1;
  while(--digit) div *= 10;

  while(fractional--) number *= 10.;
  integer = (U32)(number + 0.5);

  zero_flag = 0;
  digit = 1;
  while(div > 0)				// display number
    { character = integer / div;
      if((character == 0) && (zero_flag == 0) && (digit != integral))
        LCD_data(character + ' ');
      else
        { zero_flag = 1;
          LCD_data(character + '0');
        }
      integer %= div;
      div /= 10;

      if(digit == integral)
        LCD_data('.');
      digit++;
    }
}
