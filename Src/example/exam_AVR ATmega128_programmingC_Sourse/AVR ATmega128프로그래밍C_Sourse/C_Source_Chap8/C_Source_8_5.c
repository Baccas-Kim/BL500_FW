//AVR128   LCD_1.C
#include<mega128.h>
#include<delay.h>

//A포트 - 비트단위 출력 설정
#define LCD_RS PORTA.0
#define LCD_RW PORTA.1
#define LCD_E  PORTA.2

//C포트 - 포트단위 출력 설정
#define LCD_DATA PORTC

//함수에 사용될 문자열 정의
#define ON    1
#define OFF   2
#define NO    3
#define RIGHT 5
#define LEFT  6

//기본 함수선언
void E_Pulse(void);
void Func_set(void);
void Init_LCD(void);

//유용한 함수 선언
void clrscr(void);
void gotoxy(char , char );
void Write_Char(char );
void Write_LCD(char ,char ,unsigned char flash *);
void Move_Display(char);
void Move_Cursor(char);
void Entry_Shift(char);
void Display_ON_OFF(char ,char ,char );//display,cursor,blank
void Cursor_home(void);
void Cnt(char n);
//함수 정의
void E_Pulse()
{
  LCD_E=1;
  delay_ms(2);
  LCD_E=0;

}

void Func_set()
{
  LCD_RW=0;
  LCD_RS=0;
  LCD_DATA=0x38;
  E_Pulse();
}

void Init_LCD(void)
{
  LCD_E=0;
  delay_ms(15);
  Func_set();
  delay_ms(5);
  Func_set();
  delay_us(100);
  Func_set();
  //디스플레이와 커서제어
  LCD_DATA=0x0f;
  E_Pulse();
  //엔트리 모드
  LCD_DATA=0x06;
  E_Pulse();
}

//지우기
void clrscr(void)
{
 LCD_RW=0;
 LCD_RS=0;
 LCD_DATA=0x01;
 E_Pulse();
 delay_us(1640);
}

//x,y좌표
void gotoxy(char x, char y)
{
 LCD_RW=0;
 LCD_RS=0;

 if(y==0)LCD_DATA=x+0x80;
 else if(y==1)LCD_DATA=x+0xc0;
 E_Pulse();
}

//한 문자 쓰기
void Write_Char(char c)
{
 LCD_RS=1;
 LCD_DATA=c;
 E_Pulse();
 }

//좌표 및 문자열 쓰기
void Write_LCD(char x,char y,unsigned char flash *str)
{
 gotoxy(x,y);
 while(*str) Write_Char(*str++);
}

//좌.우로 이동
void Move_Display(char m)
{
 LCD_RW=0;
 LCD_RS=0;

 if(m==LEFT) LCD_DATA=0x18;
 else if(m==RIGHT) LCD_DATA=0x1c;
 E_Pulse();
}

void Move_Cursor(char m)
{
 LCD_RW=0;
 LCD_RS=0;

 if(m==RIGHT) LCD_DATA=0x14;
 else if(m==LEFT) LCD_DATA=0x10;
 E_Pulse();
}
void Entry_Shift(char e)
{
 LCD_RW=0;
 LCD_RS=0;

 if(e==RIGHT) LCD_DATA=0x05;
 else if(e==LEFT) LCD_DATA=0x07;
 else if(e==NO) LCD_DATA=0x06;
 E_Pulse();
}

void Display_ON_OFF(char d,char c,char b)
{
 LCD_RW=0;
 LCD_RS=0;

 if(d==ON) d=0x04; else d=0x00;
 if(c==ON) c=0x02; else c=0x00;
 if(b==ON) b=0x01; else b=0x00;
 LCD_DATA=0x08|d|c|b;
 E_Pulse();
}

void Cursor_home(void)
{
 LCD_RW=0;
 LCD_RS=0;
 LCD_DATA=0x02;

 delay_us(1640);
 E_Pulse();
}
//10진 카운터
void Cnt(char n)
{
  LCD_RS=1;
  LCD_DATA=n+0x30;
  E_Pulse(); 
}

void main()
{
 //A포트 비트단위 출력
  DDRA.0=1;//LCD_RS
  DDRA.1=1;//LCD_R/W
  DDRA.2=1;//LCD_E
  //C포트 출력
  DDRC=0xFF;
  Init_LCD();
  clrscr();
  Display_ON_OFF(ON,OFF,OFF);
  Write_LCD(0,0,"LCD Control Pro.");
  Write_LCD(0,1,"Ryu Gi Ju");
}
