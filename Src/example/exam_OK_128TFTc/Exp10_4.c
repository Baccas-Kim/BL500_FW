/* ========================================================================== */
/*	    Exp10_4.c : 터치 스크린을 이용한 그림판(아이콘 입력 개선)         */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

unsigned char line_width = 2;			// line width (default = middle)
unsigned int  user_color = White;		// user color (default = White)

void Draw_select(void)				/* draw selected color and line width */
{
  Block(10,267, 45,287, Black, Black);		// clear select box

  Rectangle(10,267, 45,287, user_color);	// line_width = 1 ?
  if(line_width >= 2)				// line_width = 2 ?
    { Rectangle(11,268, 44,286, user_color);
      Rectangle(12,269, 43,285, user_color);
    }
  if(line_width >= 3)				// line_width = 3 ?
    { Rectangle(13,270, 42,284, user_color);
      Rectangle(14,271, 41,283, user_color);
    }
}

void Title_screen(void)				/* draw title screen */
{
  Rectangle(0,0, 239,319, Green);		// draw outline of sheet
  Rectangle(1,1, 238,318, Green);
  Rectangle(2,2, 237,317, Green);

  TFT_string(11,1, White,Magenta, " 그림판 ");	// display title

  Draw_select();				// select box

  TFT_string(2,37, Green,Black, " C ");		// clear selection button
  Rectangle(10,292, 45,312, Green);
  Rectangle(11,293, 44,311, Green);

  Block(160,267, 180,287, White, White);	// line_width selection button
  Line(165,277, 175,277, Black);
  Block(185,267, 205,287, White, White);
  Line(190,276, 200,276, Black);
  Line(190,277, 200,277, Black);
  Line(190,278, 200,278, Black);
  Block(210,267, 230,287, White, White);
  Line(215,275, 225,275, Black);
  Line(215,276, 225,276, Black);
  Line(215,277, 225,277, Black);
  Line(215,278, 225,278, Black);
  Line(215,279, 225,279, Black);

  Block( 60,292,  80,312, White, White);	// color selection button
  Block( 85,292, 105,312, White, Red);
  Block(110,292, 130,312, White, Yellow);
  Block(135,292, 155,312, White, Green);
  Block(160,292, 180,312, White, Cyan);
  Block(185,292, 205,312, White, Blue);
  Block(210,292, 230,312, White, Magenta);
}

#define icon_width1	0x01			// line width 1
#define icon_width2	0x02			// line width 2
#define icon_width3	0x03			// line width 3
#define icon_clear	0x04			// clear
#define icon_white	0x05			// white
#define icon_red	0x06			// red
#define icon_yellow	0x07			// yellow
#define icon_green	0x08			// green
#define icon_cyan	0x09			// cyan
#define icon_blue	0x0A			// blue
#define icon_magenta	0x0B			// magenta
#define no_icon		0x00			// no icon

unsigned char icon_flag = 0;

unsigned char Icon_input(void)			/* input touch screen icon */
{
  unsigned char icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 160) && (x_touch <= 180) && (y_touch >= 267) && (y_touch <= 287))
    { icon = icon_width1;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 185) && (x_touch <= 205) && (y_touch >= 267) && (y_touch <= 287))
    { icon = icon_width2;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 210) && (x_touch <= 230) && (y_touch >= 267) && (y_touch <= 287))
    { icon = icon_width3;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 10) && (x_touch <= 45) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_clear;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 60) && (x_touch <= 80) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_white;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 85) && (x_touch <= 105) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_red;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 110) && (x_touch <= 130) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_yellow;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 135) && (x_touch <= 155) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_green;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 160) && (x_touch <= 180) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_cyan;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 185) && (x_touch <= 205) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_blue;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 210) && (x_touch <= 230) && (y_touch >= 292) && (y_touch <= 312))
    { icon = icon_magenta;
      icon_flag = 1;
      Beep();
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_icon;
      icon_flag = 0;
      Delay_ms(50);
    }
  else
    icon = no_icon;

  return icon;
}

/* ---------------------------------------------------------------------------- */

int main(void)
{
  unsigned char icon;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp10_4.c    ");

  Initialize_touch_screen();			// initialize touch screen
  Title_screen();				// draw title screen

  while(1)
    { icon = Icon_input();			// input touch screen icon
      if(icon == icon_width1)
        { line_width = 1;	Draw_select(); }
      else if(icon == icon_width2)
        { line_width = 2;	Draw_select(); }
      else if(icon == icon_width3)
        { line_width = 3;	Draw_select(); }
      else if(icon == icon_clear)
        { TFT_clear_screen();	Title_screen(); }
      else if(icon == icon_white)
        { user_color = White;	Draw_select(); }
      else if(icon == icon_red)
        { user_color = Red;	Draw_select(); }
      else if(icon == icon_yellow)
        { user_color = Yellow;	Draw_select(); }
      else if(icon == icon_green)
        { user_color = Green;	Draw_select(); }
      else if(icon == icon_cyan)
        { user_color = Cyan;	Draw_select(); }
      else if(icon == icon_blue)
        { user_color = Blue;	Draw_select(); }
      else if(icon == icon_magenta)
        { user_color = Magenta;	Draw_select(); }

      Touch_screen_input();			// input touch screen
      if((x_touch != 0) || (y_touch != 0))
        { if((line_width == 1) && (y_touch < 265))
            TFT_pixel(x_touch, y_touch, user_color);
          else if((line_width == 2) && (x_touch >= 1) && (y_touch >= 1) && (y_touch < 265))
            { TFT_pixel(x_touch, y_touch, user_color);
	      Rectangle(x_touch-1, y_touch-1, x_touch+1, y_touch+1, user_color);
            }
          else if((line_width == 3) && (x_touch >= 2) && (y_touch >= 2) && (y_touch < 265))
            { TFT_pixel(x_touch, y_touch, user_color);
	      Rectangle(x_touch-1, y_touch-1, x_touch+1, y_touch+1, user_color);
	      Rectangle(x_touch-2, y_touch-2, x_touch+2, y_touch+2, user_color);
            }
        }
    }
}
