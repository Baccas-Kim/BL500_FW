/* ============================================================================ */
/*	             Exp02_2.c : Basic C Program (2)				*/
/* ============================================================================ */
/*			    Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>

#define BV(bit)		(1<<(bit))		/* bit processing */
#define cbi(reg,bit)	reg &= ~(BV(bit))
#define sbi(reg,bit)	reg |= (BV(bit))

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

void Delay_us(unsigned char time_us)		/* time delay for us */
{
  register unsigned char i;

  for(i = 0; i < time_us; i++)			// 4 cycle +
    { asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle +
      asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle +
      asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle = 16 cycle = 1 us for 16MHz
    }
}

void Delay_ms(unsigned int time_ms)		/* time delay for ms */
{
  register unsigned int i;

  for(i = 0; i < time_ms; i++)
    { Delay_us(250);
      Delay_us(250);
      Delay_us(250);
      Delay_us(250);
    }
}

void LCD_command(unsigned char command)		/* write a command(instruction) to text LCD */
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

void LCD_data(unsigned char data)		/* display a character on text LCD */
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

void LCD_initialize(void)			/* initialize text LCD module */
{
  LCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
  LCD_command(0x0C);				// display control(display ON, cursor OFF)
  LCD_command(0x06);				// entry mode set(increment, not shift)
  LCD_command(0x01);				// clear display
  Delay_ms(2);
}

void LCD_string(unsigned char command, char *string)	/* display a string on text LCD */
{
  LCD_command(command);				// start position of string
  while(*string != '\0')			// display string
    { LCD_data(*string);
       string++;
    }
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module

  LCD_string(0x80," OK-128TFT V1.0 ");          // display logo

  while(1)
    { LCD_string(0xC0,"   ATmega128A   ");      // display message 1
      Delay_ms(1000);
      LCD_string(0xC0,"   2010/12/05   ");      // display message 2
      Delay_ms(1000);
    }
}
