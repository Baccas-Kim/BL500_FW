/* ========================================================================== */
/*		Exp12_2.c : LG전자 TV 리모컨(NEC 포맷) 수신 프로그램	      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

// Leader_Code	= 211				// 13.5ms / 64us = 210.938
// Low_Bit	= 18				// 1.125ms / 64us = 17.578
// High_Bit	= 35				// 2.25ms / 64us = 35.156

#define Leader_Max	232			// 110% of Leader_Code
#define Leader_Min	189			//  90% of Leader_Code
#define Low_Bit_Max	20			// 110% of Low_Bit_Code
#define Low_Bit_Min	15			//  90% of Low_Bit_Code
#define High_Bit_Max	39			// 110% of High_Bit_Code
#define High_Bit_Min	31			//  90% of High_Bit_Code

volatile unsigned char Remocon_state;		// step to recognize remocon signal
volatile unsigned char Remocon_count;		// 64us base time counter
volatile unsigned char Remocon_data;		// temporary byte data of remocon signal
volatile unsigned char Remocon_bit_count;	// bit count of data to recognize remocon signal
volatile unsigned char Remocon_OK_flag;		// remocon command receive OK
volatile unsigned char Remocon_command[4];	// remocon command string

ISR(TIMER1_OVF_vect)				/* Timer1 overflow interrupt with 64us * 256 period */
{
  Remocon_state = 0;				// if overflow, the signal is noise
}

ISR(INT6_vect)					/* INT6 interrupt by remocon signal */
{
  Remocon_count = TCNT1;			// read Remocon_count
  TCNT1 = 0;					// start new Remocon_count

  sei();                                       	// global interrupt enable

  switch(Remocon_state)
    { case 0 :					// if ready state, go to step 1(leader state)
               Remocon_state = 1;
               Remocon_count = 0;           
               break;

      case 1 : 					// if leader state, check leader signal
               if((Remocon_count >= Leader_Min) && (Remocon_count <= Leader_Max))
                 { Remocon_state = 2;		// if lead signal, go to step 2(data state)
	           Remocon_count = 0;
                   Remocon_data = 0;
	           Remocon_bit_count = 0;
                 }
               else
	         Remocon_state = 0;		// if not lead signal, go to step 0(ready state)
               break;

      case 2: 					// if data state, check data signal 0 or 1
               Remocon_data >>= 1;
               if((Remocon_count >= Low_Bit_Min) && (Remocon_count <= Low_Bit_Max))
                 Remocon_data &= 0x7F;		// if data 0, add data bit 0
               else if((Remocon_count >= High_Bit_Min) && (Remocon_count <= High_Bit_Max))
                 Remocon_data |= 0x80;
               else
	         Remocon_state = 0;		// if not 0 and not 1, go to step 0(ready state)
               Remocon_count = 0;
               Remocon_bit_count++;
	       if((Remocon_bit_count % 8) == 0) // if a character complete, store it
	         { Remocon_command[(Remocon_bit_count/8)-1] = Remocon_data;
	           Remocon_data = 0;
                 }
               if(Remocon_bit_count == 32)	// if remocon OK, check custom code and checksum
                 { if((~Remocon_command[2] & 0xFF) == Remocon_command[3])
		     { Remocon_state = 0;
		       Remocon_OK_flag = 1;
		     }
                   else
		     Remocon_state = 0;
	 	 }
               break;

      default : break;
    }
}

void Remocon_initialize()			/* Timer1 overflow interrupt and INT6 interrupt */
{
  TCCR1A = 0x01;				// Timer 1, mode 5(fast PWM with 0x00FF period)
  TCCR1B = 0x0D;				// 16MHz/1024 = 15625Hz (64us base time)
  TCCR1C = 0x00;

  TIFR = 0x04;					// clear Timer1 overflow interrupt flag
  TIMSK = 0x04;					// enable Timer1 overflow interrupt

  Remocon_state = 0;				// remocon ready state
  Remocon_OK_flag = 0;				// remocon not character

  cbi(DDRE,6);					// PE6(INT6) = input

  EICRB = 0x20;                                	// INT6 = falling edge trigger for remocon signal
  EIFR = 0x40;                                 	// clear INT6 interrupt flag
  EIMSK = 0x40;					// enable INT6

  sei();					// global interrupt enable 
}

int main(void)
{
  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  TFT_landscape();				// landscape mode

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp12_2.c    ");
  Beep();

  TFT_string(10,10,White,Magenta,"  LG전자 TV 리모컨  ");
  TFT_string(11,16,Green,Black,  "코드 : 00 00 00 00");
  TFT_string(11,19,Green,Black,  "기능 :             ");
  TFT_color(White,Black);

  Remocon_initialize();				// initialize remocon

  while(1)
    { if(Remocon_OK_flag == 1)
        { Remocon_OK_flag = 0;
          TFT_xy(18,16); TFT_hexadecimal(Remocon_command[0],2);		   // code
          TFT_xy(21,16); TFT_hexadecimal(Remocon_command[1],2);
          TFT_xy(24,16); TFT_hexadecimal(Remocon_command[2],2);
          TFT_xy(27,16); TFT_hexadecimal(Remocon_command[3],2);
          Beep();

          if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x08)) // function
	    TFT_string(18,19,White,Black,"전원     ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x0B))
	    TFT_string(18,19,White,Black,"TV/비디오");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x0F))
	    TFT_string(18,19,White,Black,"TV/유선  ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x10))
	    TFT_string(18,19,White,Black,"0        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x11))
	    TFT_string(18,19,White,Black,"1        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x12))
	    TFT_string(18,19,White,Black,"2        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x13))
	    TFT_string(18,19,White,Black,"3        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x14))
	    TFT_string(18,19,White,Black,"4        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x15))
	    TFT_string(18,19,White,Black,"5        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x16))
	    TFT_string(18,19,White,Black,"6        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x17))
	    TFT_string(18,19,White,Black,"7        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x18))
	    TFT_string(18,19,White,Black,"8        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x19))
	    TFT_string(18,19,White,Black,"9        ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x43))
	    TFT_string(18,19,White,Black,"메뉴     ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x00))
	    TFT_string(18,19,White,Black,"채널+    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x01))
	    TFT_string(18,19,White,Black,"채널-    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x02))
	    TFT_string(18,19,White,Black,"음량+    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x03))
	    TFT_string(18,19,White,Black,"음량-    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x06))
	    TFT_string(18,19,White,Black,"조정+    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x07))
	    TFT_string(18,19,White,Black,"조정-    ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x09))
	    TFT_string(18,19,White,Black,"조용히   ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x54))
	    TFT_string(18,19,White,Black,"자동채널 ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x1A))
	    TFT_string(18,19,White,Black,"앞채널   ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x44))
	    TFT_string(18,19,White,Black,"기억     ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x55))
	    TFT_string(18,19,White,Black,"지움     ");
          else if((Remocon_command[0] == 0x04) && (Remocon_command[2] == 0x0E))
	    TFT_string(18,19,White,Black,"취침예약 ");
          else
	    TFT_string(18,19,White,Black,"         ");
        }
    }
}
