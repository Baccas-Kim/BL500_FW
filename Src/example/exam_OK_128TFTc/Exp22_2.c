/* ========================================================================== */
/*		Exp22_2.c : 오목 게임 프로그램				      */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2010.  */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "OK-128LCD.h"
#include "OK-128TFT.h"

unsigned char Icon_input(void);			// input touch screen icon
void Initialize_board(void);			// initialize board
void Draw_symbol(U08 xPos,U08 yPos, U16 color, U08 code); // draw cross of bopard
void Draw_board(void);				// draw board
void Draw_control(void);			// draw game control
void Draw_nextgame(U16 color1, U16 color2);	// display next game for touch screen
void Draw_left(U16 color1, U16 color2);		// display left arrow for touch screen
void Draw_right(U16 color1, U16 color2);	// display right arrow for touch screen
void Draw_up(U16 color1, U16 color2);		// display up arrow for touch screen
void Draw_down(U16 color1, U16 color2);		// display down arrow for touch screen
void Draw_enter(U16 color1, U16 color2);	// display stone enter for touch screen
void Draw_arrow(U16 xPos,U16 yPos,U16 color,U08 code);	  // draw arrow for touch screen

void Omok_game(void);				// omok game algorism
void Check_user(U08 countmax, U08 danger);	// check if user win
void Check_CPU(U08 countmax, U08 danger);	// check if CPU win

/* ---------------------------------------------------------------------------- */

#define	Xbase		15			// x base position of board
#define	Ybase		23			// y base position of board

#define White_stone	1 
#define Black_stone	2 

#define Left		0x01			// menu code of touch screen
#define Right		0x02
#define Down		0x03
#define Up		0x04
#define Enter		0x05
#define Next		0x06

unsigned char cursor_x = 9, cursor_y = 9;	// cursor position
unsigned char cursor_x_old, cursor_y_old;
unsigned char game_over;			// game over flag
unsigned char beep_flag;			// beep on/off flag

unsigned char board[19][19];			// board data
unsigned char board_old[19][19];

unsigned char danger_CPU[19][19];		// CPU danger count
unsigned char danger_user[19][19]; 		// user danger count

/* ---------------------------------------------------------------------------- */

volatile unsigned char second, minute;

ISR(TIMER1_COMPA_vect)				/* OC1A interrupt function (1 sec) */
{
  second++;					// increment second
  if(second == 60)                              // if second = 60, second = 0
    { second = 0;
      minute++;                                 // increment minute
      if(minute == 60)                          // if minute = 60, minute = 0
        minute = 0;
     }
}

int main(void)
{
  unsigned char key, exit;

  MCU_initialize();                             // initialize MCU and kit
  Delay_ms(50);                                 // wait for system stabilization
  LCD_initialize();                             // initialize text LCD module
  TFT_initialize();				// initialize TFT-LCD module
  Beep();

  LCD_string(0x80," OK-128TFT V1.0 ");		// display title
  LCD_string(0xC0,"   Exp22_2.c    ");

  Initialize_touch_screen();			// initialize touch screen

  beep_flag = 1;				// beep on

  TCCR1A = 0x00;                                // CTC mode(4), don't output OC1A
  TCCR1B = 0x0C;                                // 16MHz/256/(1+62499) = 1Hz
  TCCR1C = 0x00;
  OCR1A = 62499;
  TCNT1 = 0x0000;                               // clear Timer/Counter1

  TIFR = 0x10;                                  // clear OC1A interrupt flag
  TIMSK = 0x10;                                 // enable OC1A interrupt
  sei();                                        // global interrupt enable

  while(1)
    { Initialize_board();			// initialize board
      game_over = 0;				// initial game state
      exit = 0;

      Draw_control();				// draw control
      second = 0;
      minute = 0;

      while(!exit) 
	{ Draw_board(); 			// draw board

          key = Icon_input();			// read touch screen icon
	  switch(key) 
	    { case Left:  if((game_over == 0) && (cursor_x != 0))  cursor_x--;
	                  break; 
	      case Right: if((game_over == 0) && (cursor_x != 18)) cursor_x++;
	                  break; 
	      case Down:  if((game_over == 0) && (cursor_y != 18)) cursor_y++;
	                  break; 
	      case Up:    if((game_over == 0) && (cursor_y != 0))  cursor_y--;
	                  break; 
	      case Enter: if((game_over == 0) && (board[cursor_y][cursor_x] == 0))
			    { board[cursor_y][cursor_x] = Black_stone;
                              if(cursor_x != 18)      cursor_x++;
			      else if(cursor_y != 18) cursor_y++;
			      Omok_game();	// omok game algorithm
			    }
			  break; 
	      case Next:  exit = 1; 
		 	  break; 
	    }

          if(game_over == 0)
	    { Check_user(5,0);			// check if user win
	      Check_CPU(5,0);			// check if CPU win
            }

          if(game_over == 0)			// display time
            { TFT_color(White,Black);
              TFT_xy(5,38); TFT_unsigned_decimal(minute,1,2);
              TFT_xy(8,38); TFT_unsigned_decimal(second,1,2);
            }
	}
    }
}

/* ---------------------------------------------------------------------------- */

unsigned char icon_flag = 0;

unsigned char Icon_input(void)			/* input touch screen icon */
{
  unsigned char icon;

  Touch_screen_input();				// input touch screen

  if((icon_flag == 0) && (x_touch >= 83) && (x_touch <= 106) && (y_touch >= 269) && (y_touch <= 289))
    { icon = Left;				// left
      icon_flag = 1;
      Draw_left(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 132) && (x_touch <= 155) && (y_touch >= 269) && (y_touch <= 289))
    { icon = Right;				// right
      icon_flag = 1;
      Draw_right(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 109) && (x_touch <= 129) && (y_touch >= 243) && (y_touch <= 266))
    { icon = Up;				// up
      icon_flag = 1;
      Draw_up(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 109) && (x_touch <= 129) && (y_touch >= 292) && (y_touch <= 315))
    { icon = Down;				// down
      icon_flag = 1;
      Draw_down(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 173) && (x_touch <= 218) && (y_touch >= 262) && (y_touch <= 296))
    { icon = Enter;				// stone enter
      icon_flag = 1;
      Draw_enter(Magenta, Magenta);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 20) && (x_touch <= 58) && (y_touch >= 262) && (y_touch <= 296))
    { icon = Next;				// next game
      icon_flag = 1;
      Draw_nextgame(White, White);
      if(beep_flag == 1) Beep();
    }
  else if((icon_flag == 0) && (x_touch >= 160) && (x_touch <= 232) && (y_touch >= 304) && (y_touch <= 319))
    { icon = no_key;				// beep on/off
      icon_flag = 1;
      Beep();
      if(beep_flag == 0)
        { beep_flag = 1;
	  TFT_string(24,38, Red, Cyan, "ON");
          TFT_string(27,38, Blue,Cyan, "OFF");
        }
      else
        { beep_flag = 0;
	  TFT_string(24,38, Blue,Cyan, "ON");
          TFT_string(27,38, Red, Cyan, "OFF");
        }
    }
  else if((icon_flag == 1) && (x_touch == 0) && (y_touch == 0))
    { icon = no_key;
      icon_flag = 0;
      Draw_left(White, Cyan);
      Draw_right(White, Cyan);
      Draw_up(White, Cyan);
      Draw_down(White, Cyan);
      Draw_enter(White, Cyan);
      Draw_nextgame(White, Magenta);
      Delay_ms(50);
    }
  else
    icon = no_key;

  return icon;
}

void Initialize_board(void)			/* initialize board */
{
  unsigned char x, y;

  cursor_x = 9;					// initial cursor position
  cursor_y = 9;

  for(y = 0 ; y < 19; y++)			// clear board
    { for(x = 0; x < 19; x++)
        { board_old[y][x] = 0xFF;
	  board[y][x] =  0; 
	  danger_CPU[y][x] = 0; 
	  danger_user[y][x] = 0;
	}
    }
}

prog_uint16_t symbol[11][11] =
 {{ 0x0000,0x0E00,0x3F80,0x3F80,0x7FC0,0x7FC0,0x7FC0,0x3F80,0x3F80,0x0E00,0x0000 },	// stone 
  { 0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,0x0400,0x0400,0x0400,0x0400,0x0840 },	// "┌" 
  { 0x0000,0x0000,0x0000,0x0000,0x0000,0xFC00,0x0400,0x0400,0x0400,0x0400,0x0400 },	// "┐" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000 },	// "└" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0xFC00,0x0000,0x0000,0x0000,0x0000,0x0000 },	// "┘" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0x07E0,0x0400,0x0400,0x0400,0x0400,0x0400 },	// "├" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0xFC00,0x0400,0x0400,0x0400,0x0400,0x0400 },	// "┤" 
  { 0x0000,0x0000,0x0000,0x0000,0x0000,0xFFE0,0x0400,0x0400,0x0400,0x0400,0x0400 },	// "┬" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0xFFE0,0x0000,0x0000,0x0000,0x0000,0x0000 },	// "┴" 
  { 0x0400,0x0400,0x0400,0x0400,0x0400,0xFFE0,0x0400,0x0400,0x0400,0x0400,0x0400 },	// "┼" 
  { 0x0000,0x7FC0,0x7FC0,0x60C0,0x60C0,0x60C0,0x60C0,0x60C0,0x7FC0,0x7FC0,0x0000 }};	// cursor

void Draw_symbol(U08 xPos,U08 yPos, U16 color, U08 code)	/* draw cross of board */
{
  unsigned char x, y;
  unsigned int font;

  for(y = 0; y < 11; y++)
    { TFT_GRAM_address(Xbase+(xPos*11),Ybase+(yPos*11)+y);
      font = pgm_read_word(&symbol[code][y]);
      for(x = 0; x < 11; x++)
        { if((font << x) & 0x8000) TFT_data(color);
	  else                     TFT_data(Orange);
	}
    }
}

void Draw_board(void)				/* draw board */ 
{ 
  unsigned char x, y; 

  for(y = 0; y < 19; y++) 
    { for(x = 0; x < 19; x++) 
	{ if(cursor_x == x && cursor_y == y)	// draw cursor
            Draw_symbol(x,y, Red, 10);
	  else if((board[y][x] != board_old[y][x]) || (cursor_x_old == x && cursor_y_old == y))
	    { board_old[y][x] = board[y][x];
	      if(board[y][x] == White_stone)	// draw white stone
	        Draw_symbol(x,y, White, 0);	
	      else if(board[y][x] == Black_stone) // draw black stone
	        Draw_symbol(x,y, Black, 0);	
	      else 				// draw cross line
		{ if(x == 0 && y == 0)                   Draw_symbol(x,y,Black,1); // "┌" 
		  else if(x == 18 && y == 0)             Draw_symbol(x,y,Black,2); // "┐" 
		  else if(x == 0  && y == 18)            Draw_symbol(x,y,Black,3); // "└" 
		  else if(x == 18 && y == 18)            Draw_symbol(x,y,Black,4); // "┘" 
		  else if(x == 0  && y != 0  && y != 18) Draw_symbol(x,y,Black,5); // "├" 
		  else if(x == 18 && y != 0  && y != 18) Draw_symbol(x,y,Black,6); // "┤" 
		  else if(x != 0  && x != 18 && y == 0)  Draw_symbol(x,y,Black,7); // "┬" 
		  else if(x != 0  && x != 18 && y == 18) Draw_symbol(x,y,Black,8); // "┴" 
		  else                                   Draw_symbol(x,y,Black,9); // "┼" 
		} 
	    }
	}
    } 

  cursor_x_old = cursor_x;
  cursor_y_old = cursor_y;
}

void Draw_control(void)				/* draw game control */
{
  TFT_string( 3, 0,White,Blue, "       오목 게임        "); // display title

  TFT_string( 0,29, White,Black, "(CPU = White)");
  TFT_string(17,29, Green,Black, "(You = Black)");

  Draw_nextgame(White,Magenta);			// display clear for new game

  Draw_left(White,Cyan);			// display arrow for touch screen
  Draw_right(White,Cyan);
  Draw_up(White,Cyan);
  Draw_down(White,Cyan);

  Draw_enter(White,Cyan);			// display stone enter

  TFT_string(0,38, White,Black, "시간 00:00");	// display time

  TFT_string(20,38, Blue,Cyan, "소리ON/OFF");	// beep on/off
  if(beep_flag == 1)
    TFT_string(24,38, Red,Cyan, "ON");
  else
    TFT_string(27,38, Red,Cyan, "OFF");
}

void Draw_nextgame(U16 color1, U16 color2)	/* display next game for touch screen */
{
  Rectangle(20,262, 58,296, color1);
  TFT_string(3,33, color2,Black, "NEXT");
  TFT_string(3,35, color2,Black, "GAME");
}

void Draw_left(U16 color1, U16 color2)		/* display left arrow for touch screen */
{
  Rectangle( 83,269, 106,289, color1);
  Draw_arrow( 87, 271, color2, 0);
}

void Draw_right(U16 color1, U16 color2)		/* display right arrow for touch screen */
{
  Rectangle(132,269, 155,289, color1);
  Draw_arrow(136, 271, color2, 1);
}

void Draw_up(U16 color1, U16 color2)		/* display up arrow for touch screen */
{
  Rectangle(109,243, 129,266, color1);
  Draw_arrow(112, 247, color2, 3);
}

void Draw_down(U16 color1, U16 color2)		/* display down arrow for touch screen */
{
  Rectangle(109,292, 129,315, color1);
  Draw_arrow(112, 296, color2, 2);
}

void Draw_enter(U16 color1, U16 color2)		/* display stone enter for touch screen */
{
  Rectangle(173,262, 218,296, color1);
  TFT_string(22,33, color2,Black, "STONE");
  TFT_string(22,35, color2,Black, "ENTER");
}

prog_uint16_t Arrow[4][16] = {					/* 16x16 arrow */
 { 0x0000,0x0100,0x0300,0x0700,0x0F00,0x1F00,0x3FFF,0x7FFF,	// (0) left arrow
   0xFFFF,0x7FFF,0x3FFF,0x1F00,0x0F00,0x0700,0x0300,0x0100 },
 { 0x0000,0x0080,0x00C0,0x00E0,0x00F0,0x00F8,0xFFFC,0xFFFE,	// (1) right arrow
   0xFFFF,0xFFFE,0xFFFC,0x00F8,0x00F0,0x00E0,0x00C0,0x0080 },
 { 0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,	// (2) up arrow
   0xFFFE,0x7FFC,0x3FF8,0x1FF0,0x0FE0,0x07C0,0x0380,0x0100 },
 { 0x0100,0x0380,0x07C0,0x0FE0,0x1FF0,0x3FF8,0x7FFC,0xFFFE,	// (3) down arrow
   0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0,0x07C0 }};

void Draw_arrow(U16 xPos,U16 yPos, U16 color, U08 code)	/* draw arrow for touch screen */
{
  unsigned char x, y;
  unsigned int font;

  for(y = 0; y < 16; y++)
    { TFT_GRAM_address(xPos,yPos+y);
      font = pgm_read_word(&Arrow[code][y]);
      for(x = 0; x < 16; x++)
        { if((font << x) & 0x8000) TFT_data(color);	
	  else                     TFT_data(Black);
	}
    }
}

/* ---------------------------------------------------------------------------- */

void Omok_game(void)				/* omok game algorism */
{
  unsigned char x, y; 
  unsigned char max_danger_value = 0, max_attack_value = 0; 
  unsigned char max_danger_x = 0, max_danger_y = 0; 
  unsigned char max_attack_x = 0, max_attack_y = 0; 

  for(y = 0; y < 19; y++)			// 위험도 초기화 
    for(x = 0; x < 19; x++)
      { danger_CPU[y][x] = 0; 
        danger_user[y][x] = 0; 
      } 

  for(y = 0; y < 19; y++)			// 상,하,좌,우,대각선 방향의 위험도
    for(x = 0; x < 19; x++)
      { if(board[y][x] == Black_stone) 
	  { if(x > 0)  if(board[y][x-1] == 0) danger_CPU[y][x-1]++; 
	    if(x < 18) if(board[y][x+1] == 0) danger_CPU[y][x+1]++; 
	    if(y > 0)  if(board[y-1][x] == 0) danger_CPU[y-1][x]++; 
	    if(y < 18) if(board[y+1][x] == 0) danger_CPU[y+1][x]++; 

	    if(y > 0 && x > 0)   if(board[y-1][x-1] == 0) danger_CPU[y-1][x-1]++; 
	    if(y > 0 && x < 18)  if(board[y-1][x+1] == 0) danger_CPU[y-1][x+1]++; 
	    if(y < 18 && x > 0)  if(board[y+1][x-1] == 0) danger_CPU[y+1][x-1]++; 
	    if(y < 18 && x < 18) if(board[y+1][x+1] == 0) danger_CPU[y+1][x+1]++; 
	  } 
      } 

  Check_user(4,10);				// check user
  Check_user(3,7); 
  Check_user(2,5); 
  Check_CPU(4,10);				// check CPU
  Check_CPU(3,7); 
  Check_CPU(2,5); 

  for(y = 0; y < 19; y++)			// 최고 위험지역 검색
    for(x = 0; x < 19; x++)
      { if(danger_CPU[y][x] > max_danger_value) 
	  { if(board[y][x] == 0) 
	      { max_danger_value = danger_CPU[y][x]; 
	        max_danger_x = x; 
	        max_danger_y = y; 
	      } 
	  } 
      } 

  for(y = 0; y < 19; y++)			// 최고 공격지역 검색
    for(x = 0; x < 19; x++)
      { if(danger_user[y][x] > max_attack_value) 
          { if(board[y][x] == 0) 
	      { max_attack_value = danger_user[y][x]; 
	        max_attack_x = x; 
	        max_attack_y = y; 
	      } 
	  } 
      }

  if(max_danger_value > max_attack_value)	// 최고 위험지역과 최고 공격지역의 수치중 높은 곳을 선택
    board[max_danger_y][max_danger_x] = White_stone; 
  else
    board[max_attack_y][max_attack_x] = White_stone; 
} 

void Check_user(U08 countmax, U08 danger)	/* check if user win */
{ 
  unsigned char x, y, i; 
  unsigned char count = 0;			// 돌 갯수를 세는 변수 
  unsigned char count2 = 0;			// 양쪽 모두 비어있는지 검사용 

  for(y = 0; y < 19; y++)			// ***** 가로 방향 검사
    { for(x = 0; x < 19-countmax; x++)
	{ for(i = x; i < x+countmax; i++)
	    if(board[y][i] == Black_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
		{ TFT_string(2,0, Green,Black, "You win.  Congratulation !");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
		} 

	      if(x > 0)               danger_CPU[y][x-1] += danger; 
	      if(x+(countmax-1) < 18) danger_CPU[y][x+(countmax-1)+1] += danger; // 위험값을 추가

	      if(board[y][x-1] == 0)              count2++; 
	      if(board[y][x+(countmax-1)+1] == 0) count2++; 

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(x > 0)               danger_CPU[y][x-1] += danger/2;
	          if(x+(countmax-1) < 18) danger_CPU[y][x+(countmax-1)+1] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(x > 0)               danger_CPU[y][x-1] += danger/4;
	          if(x+(countmax-1) < 18) danger_CPU[y][x+(countmax-1)+1] += danger/4;
	        } 
	      count2 = 0; 
	    } 
          if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = x; i < x+countmax; i++)
	        { if(board[y][i] == 0 && i > x && i < x+countmax-1) // 빈곳이면서 사이에 들어있으면
		    danger_CPU[y][i] += danger/2;	// 위험값의 50%를 추가
	        } 
	    } 
          count = 0; 
        } 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++)		// ***** 세로 방향 검사
    { for(x = 0; x < 19; x++)
	{ for(i = y; i < y+countmax; i++)
	    if(board[i][x] == Black_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
		{ TFT_string(2,0, Green,Black, "You win.  Congratulation !");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
		} 

	      if(y > 0)               danger_CPU[y-1][x] += danger;
	      if(y+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x]+=danger; // 위험값을 추가

	      if(board[y-1][x] == 0)              count2++;
	      if(board[y+(countmax-1)+1][x] == 0) count2++;

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
		{ if(y > 0)               danger_CPU[y-1][x] += danger/2;
		  if(y+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x] += danger/2;
		} 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
		{ if(y > 0)               danger_CPU[y-1][x] += danger/4; 
		  if(y+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x] += danger/4;
		} 
	      count2 = 0; 
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = y; i < y+countmax; i++)
		{ if(board[i][x] == 0 && i > y && i < y+countmax-1) // 빈곳이면서 사이에 들어있으면
		    danger_CPU[i][x] += danger/2;	// 위험값의 50%를 추가 
		} 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++)		// ***** 대각선 ＼ 방향 검사
    { for(x = 0; x < 19-countmax; x++)
	{ for(i = 0; i < countmax; i++)
	    if(board[y+i][x+i] == Black_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
		{ TFT_string(2,0, Green,Black, "You win.  Congratulation !");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
		} 

	      if(x > 0 && y > 0)                             danger_CPU[y-1][x-1]+=danger;
	      if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x+(countmax-1)+1] += danger;

	      if(board[y-1][x-1] == 0)                           count2++; 
	      if(board[y+(countmax-1)+1][x+(countmax-1)+1] == 0) count2++; 

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(x > 0 && y > 0)                             danger_CPU[y-1][x-1] += danger/2;
	          if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x+(countmax-1)+1] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(x > 0 && y > 0)                             danger_CPU[y-1][x-1] += danger/4;
	 	  if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_CPU[y+(countmax-1)+1][x+(countmax-1)+1] += danger/4;
	        } 
	      count2 = 0; 
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = 0; i < countmax; i++)
		{ if(board[y+i][x+i] == 0 && i > 0 && i < countmax-1) // 빈곳이면서 사이에 들어있으면
		    danger_CPU[y+i][x+i] += danger/2; // 위험값의 50%를 추가 
		} 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++)		// ***** 대각선 / 방향 검사
    { for(x = (countmax-1); x < 19; x++)
	{ for(i = 0; i < countmax; i++)
	    if(board[y+i][x-i] == Black_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
		{ TFT_string(2,0, Green,Black, "You win.  Congratulation !");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
		} 

	      if(x+1 < 18 && y > 0)                         danger_CPU[y-1][x+1] += danger; 
	      if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_CPU[y+(countmax-1)+1][x-(countmax-1)-1] += danger; 

	      if(board[y-1][x+1] == 0)                           count2++; 
	      if(board[y+(countmax-1)+1][x-(countmax-1)-1] == 0) count2++; 

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
		{ if(x+1 < 18 && y > 0)                         danger_CPU[y-1][x+1] += danger/2;
		  if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_CPU[y+(countmax-1)+1][x-(countmax-1)-1] += danger/2;
		} 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가 
		{ if(x+1 < 18 && y > 0)                         danger_CPU[y-1][x+1] += danger/4;
		  if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_CPU[y+(countmax-1)+1][x-(countmax-1)-1] += danger/4;
		} 
	      count2 = 0; 
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = 0; i < countmax; i++)
		{ if(board[y+i][x-i] == 0 && i > 0 && i < countmax-1) // 빈곳이면서 사이에 들어있으면
		    danger_CPU[y+i][x-i] += danger/2; // 위험값의 50%를 추가 
		} 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 
} 

void Check_CPU(U08 countmax, U08 danger)	/* check if CPU win */
{ 
  unsigned char x, y, i; 
  unsigned char count = 0;			// 돌 갯수를 세는 변수 
  unsigned char count2 = 0;			// 양쪽 모두 비어있는지 검사용 

  for(y = 0; y < 19; y++)			// ***** 가로 방향 검사
    { for(x = 0; x < 19-countmax; x++)
        { for(i = x; i < x+countmax; i++)
	    if(board[y][i] == White_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
                { TFT_string(2,0, White,Black, "       CPU 승리 !!!       ");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
	        } 

	      if(x > 0)               danger_user[y][x-1] += danger; 
	      if(x+(countmax-1) < 18) danger_user[y][x+(countmax-1)+1] += danger; // 위험값을 추가

	      if(board[y][x-1] == 0)              count2++;
	      if(board[y][x+(countmax-1)+1] == 0) count2++;

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(x > 0)               danger_user[y][x-1] += danger/2;
	          if(x+(countmax-1) < 18) danger_user[y][x+(countmax-1)+1] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(x > 0)               danger_user[y][x-1] += danger/4;
	          if(x+(countmax-1) < 18) danger_user[y][x+(countmax-1)+1] += danger/4;
	        } 
	      count2 = 0; 
	    } 
 	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = x; i < x+countmax; i++) 
	        { if(board[y][i] == 0 && i > x && i < x+countmax-1)	// 빈곳이면서 사이에 들어있으면
	            danger_user[y][i] += danger/2; // 위험값의 50%를 추가 
	        } 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++) 		// ***** 가로 방향 검사
    { for(x = 0; x < 19; x++)
        { for(i = y; i < y+countmax; i++)
	    if(board[i][x] == White_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
                { TFT_string(2,0, White,Black, "       CPU 승리 !!!       ");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
	        } 

	      if(y > 0)               danger_user[y-1][x] += danger; 
	      if(y+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x] += danger; // 위험값을 추가

	      if(board[y-1][x] == 0)              count2++;
	      if(board[y+(countmax-1)+1][x] == 0) count2++;

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(y > 0)               danger_user[y-1][x] += danger/2;
	          if(y+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(y > 0)               danger_user[y-1][x] += danger/4;
	          if(y+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x] += danger/4;
	        } 
	      count2 = 0; 
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = y; i < y+countmax; i++)
	        { if(board[i][x] == 0 && i > y && i < y+countmax-1)	// 빈곳이면서 사이에 들어있으면
	            danger_user[i][x] += danger/2; // 위험값의 50%를 추가 
	        } 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++) 		// ***** 대각선 ＼ 방향 검사
    { for(x = 0; x < 19-countmax; x++)
        { for(i = 0; i < countmax; i++)
	    if(board[y+i][x+i] == White_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
                { TFT_string(2,0, White,Black, "       CPU 승리 !!!       ");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
	        } 

	      if(x > 0 && y > 0)                             danger_user[y-1][x-1] += danger; 
	      if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x+(countmax-1)+1] += danger; 

	      if(board[y-1][x-1] == 0)                           count2++; 
	      if(board[y+(countmax-1)+1][x+(countmax-1)+1] == 0) count2++; 

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(x > 0 && y > 0)                             danger_user[y-1][x-1] += danger/2;
	          if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x+(countmax-1)+1] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(x > 0 && y > 0)                             danger_user[y-1][x-1] += danger/4;
	          if(y+(countmax-1) < 18 && x+(countmax-1) < 18) danger_user[y+(countmax-1)+1][x+(countmax-1)+1] += danger/4;
	        } 
	      count2 = 0;
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = 0; i < countmax; i++)
	        { if(board[y+i][x+i] == 0 && i > 0 && i < countmax-1)	// 빈곳이면서 사이에 들어있으면
	            danger_user[y+i][x+i] += danger/2; // 위험값의 50%를 추가 
	        } 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 

  for(y = 0; y < 19-countmax; y++)		// ***** 대각선 / 방향 검사
    { for(x = (countmax-1); x < 19; x++)
        { for(i = 0; i < countmax; i++)
	    if(board[y+i][x-i] == White_stone) count++; // 돌 갯수 검사 
	  if(count >= countmax)
	    { if(countmax >= 5)			// 5개가 연결되면 게임 끝 
                { TFT_string(2,0, White,Black, "       CPU 승리 !!!       ");
		  game_over = 1; 
                  if(beep_flag == 1) Beep_3times();
		  return;
	        } 

	      if(x+1 < 18 && y > 0)                         danger_user[y-1][x+1] += danger; 
	      if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_user[y+(countmax-1)+1][x-(countmax-1)-1] += danger; 

	      if(board[y-1][x+1] == 0)                           count2++;
	      if(board[y+(countmax-1)+1][x-(countmax-1)-1] == 0) count2++;

	      if(count2 >= 2)			// 만약에 양쪽이 전부 비어있으면 위험값의 50%를 추가
	        { if(x+1 < 18 && y > 0)                         danger_user[y-1][x+1] += danger/2;
	          if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_user[y+(countmax-1)+1][x-(countmax-1)-1] += danger/2;
	        } 
	      else if(count2 >= 1)		// 한쪽만 비어있으면 위험값의 25%를 추가
	        { if(x+1 < 18 && y > 0)                         danger_user[y-1][x+1] += danger/4;
	          if(y+(countmax-1) < 18 && x-(countmax-1) > 0) danger_user[y+(countmax-1)+1][x-(countmax-1)-1] += danger/4;
	        } 
	      count2 = 0;
	    } 
	  if(countmax == 4 && count == 3)	// 4개를 체크했는데 한칸 띄고 3개가 있으면
	    { for(i = 0; i < countmax; i++)
	        { if(board[y+i][x-i] == 0 && i > 0 && i < countmax-1)	// 빈곳이면서 사이에 들어있으면
	            danger_user[y+i][x-i] += danger/2; // 위험값의 50%를 추가 
	        } 
	    } 
	  count = 0; 
	} 
    } 
  count = 0; 
  count2 = 0; 
}
