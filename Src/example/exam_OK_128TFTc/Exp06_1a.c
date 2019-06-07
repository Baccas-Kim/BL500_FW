/* ========================================================================== */
/*		Exp06_1a.c : Read/Write ATmega128 EEPROM (2)	              */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "OK-128LCD.h"

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
      while(!eeprom_is_ready());                // if EEPROM is ready,
      eeprom_write_byte((uint8_t *)address,0x55);  //  write 0x55
      while(!eeprom_is_ready());                // if EEPROM is ready,
      byte = eeprom_read_byte((uint8_t *)address); //  read
      LCD_command(0xCE);                        // display read data
      LCD_hexadecimal(byte,2);
      Delay_ms(500);
      if(byte != 0x55)                          // if error, beep
        { Beep_3times();
          Delay_ms(2000);
        }

      LCD_command(0xC9);                        // display write data
      LCD_hexadecimal(0xAA,2);
      while(!eeprom_is_ready());                // if EEPROM is ready,
      eeprom_write_byte((uint8_t *)address,0xAA);  //  write 0xAA
      while(!eeprom_is_ready());                // if EEPROM is ready,
      byte = eeprom_read_byte((uint8_t *)address); //  read
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

