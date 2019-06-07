/* ========================================================================== */
/*		Exp06_1.c : Read/Write ATmega128 EEPROM (1)	              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"

unsigned char EEPROM_read(unsigned int address) /* read from EEPROM */
{
  while(EECR & (1 << EEWE));			// if EEWE = 1, wait
  EEAR = address;                               // if EEWE = 0, read
  EECR |= (1 << EERE);				// EERE = 1
  return EEDR;                                  // return data
}

void EEPROM_write(unsigned int address, unsigned char byte) /* write to EEPROM */
{
  while(EECR & (1 << EEWE));			// if EEWE = 1, wait
  EEAR = address;                               // if EEWE = 0, write
  EEDR = byte;
  EECR |= (1 << EEMWE);				// EEMWE = 1
  EECR |= (1 << EEWE);				// EEWE = 1
}

int main(void)
{
  unsigned char byte;
  unsigned int address;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  Beep();

  LCD_string(0x80,"ATmega128 EEPROM");          // display title
  LCD_string(0xC0,"0x0000=0x00/0x00");

  for(address = 0x0000; address <= 0x0FFF; address++)
    { LCD_command(0xC2);                        // display address
      LCD_hexadecimal(address,4);

      LCD_command(0xC9);                        // display write data
      LCD_hexadecimal(0x55,2);
      EEPROM_write(address,0x55);               // if EEPROM is ready, write 0x55
      byte = EEPROM_read(address);              // if EEPROM is ready, read
      LCD_command(0xCE);                        // display read data
      LCD_hexadecimal(byte,2);
      Delay_ms(500);
      if(byte != 0x55)                          // if error, beep
        { Beep_3times();
          Delay_ms(2000);
        }

      LCD_command(0xC9);                        // display write data
      LCD_hexadecimal(0xAA,2);
      EEPROM_write(address,0xAA);               // if EEPROM is ready, write 0xAA
      byte = EEPROM_read(address);              // if EEPROM is ready, read
      LCD_command(0xCE);                        // display read data
      LCD_hexadecimal(byte,2);
      Delay_ms(500);
      if(byte != 0xAA)                          // if error, beep
        { Beep_3times();
          Delay_ms(2000);
        }
    }

  Beep();
  while(1);
}
