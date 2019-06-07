//AVR128   LCD_1.C
#include<mega128.h>
#include<delay.h>

//A��Ʈ - ��Ʈ���� ��� ����
#define LCD_RS PORTA.0
#define LCD_RW PORTA.1
#define LCD_E  PORTA.2

//C��Ʈ - ��Ʈ���� ��� ����
#define LCD_DATA PORTC

//�Լ��� ���� ���ڿ� ����
#define ON    1
#define OFF   2
#define NO    3
#define RIGHT 5
#define LEFT  6

//�⺻ �Լ�����
void E_Pulse(void);
void Func_set(void);
void Init_LCD(void);

//������ �Լ� ����
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
//�Լ� ����
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
  //���÷��̿� Ŀ������
  LCD_DATA=0x0f;
  E_Pulse();
  //��Ʈ�� ���
  LCD_DATA=0x06;
  E_Pulse();
}

//�����
void clrscr(void)
{
 LCD_RW=0;
 LCD_RS=0;
 LCD_DATA=0x01;
 E_Pulse();
 delay_us(1640);
}

//x,y��ǥ
void gotoxy(char x, char y)
{
 LCD_RW=0;
 LCD_RS=0;

 if(y==0)LCD_DATA=x+0x80;
 else if(y==1)LCD_DATA=x+0xc0;
 E_Pulse();
}

//�� ���� ����
void Write_Char(char c)
{
 LCD_RS=1;
 LCD_DATA=c;
 E_Pulse();
 }

//��ǥ �� ���ڿ� ����
void Write_LCD(char x,char y,unsigned char flash *str)
{
 gotoxy(x,y);
 while(*str) Write_Char(*str++);
}

//��.��� �̵�
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
//10�� ī����
void Cnt(char n)
{
  LCD_RS=1;
  LCD_DATA=n+0x30;
  E_Pulse(); 
}

char sec=0,scale=0;//1�� �� 1���� ����� ���� ����
char AP=0,hour=1,min=0,h2,h1,m2,m1,s2,s1;
char AP_flag=0, hm_flag; 

void main()
{
 
 //A��Ʈ ��Ʈ���� ���
  DDRA.0=1;//LCD_RS
  DDRA.1=1;//LCD_R/W
  DDRA.2=1;//LCD_E
  //C��Ʈ ���
  DDRC=0xFF; 
  //D��Ʈ �Է�
  DDRD=0x00;//D_PORT IN, ��, ��, �ʸ� ���߱� ���� �Է�
  PORTD=0xFF;
  //���ͷ�Ʈ ����
  TIMSK=0x01;//Timer/Counter Mask Register(TOIEO=1)
  //Timer0 overflow interrupt, 1=enabled, 0 = disabled
  TCCR0=0x07;//Timer/Counter Control Register(CS02=1,CS01=1,CS00=1)
  TCNT0=0x00;//Ÿ�̸� ī���� �ʱ� ��  
  
  EICRA=0b00101010;//�ܺ� ���ͷ�ƮPD2(INT2),���ͷ�ƮPD1(INT1), PD0(INT0)�ϰ� ����
  EIMSK=0b00000111;//3���� �ܺ� ���ͷ�Ʈ �ο��̺�
  
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��  
  
  Init_LCD();
  clrscr();
  Display_ON_OFF(ON,OFF,OFF);
  
  //LCD�� ù��° �� �޽��� ǥ�� 
  Write_LCD(0,0,"LCD Control Pro."); 
  
  while(1)
  {
   //LCD�� �ι�° �� �ð� ǥ�� 
   if(AP==0)Write_LCD(3,1,"AM");
   if(AP==1)Write_LCD(3,1,"PM");  
   //�� ���� ǥ��
   gotoxy(5,1); Write_Char(':');
   gotoxy(6,1); Cnt(h2); gotoxy(7,1); Cnt(h1);  
   //�� ���� ǥ��   
   gotoxy(8,1); Write_Char(':');
   gotoxy(9,1); Cnt(m2); gotoxy(10,1); Cnt(m1);
   //�� ���� ǥ��  
   gotoxy(11,1); Write_Char(':');
   gotoxy(12,1); Cnt(s2); gotoxy(13,1); Cnt(s1); 
  }
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  sec=0;//�� ���� ����
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD0_int1(void)
{
 //�� ���� 
  hm_flag=1;
 //if(PIND.1==1)delay_ms(50);
 
 //ä�͸��� �����ϱ� ���Ͽ� 
}

//�ܺ� ���ͷ�Ʈ 2
interrupt[EXT_INT2] void PD1_int2(void)
{
   hm_flag=2;
}

//Ÿ�̸�/ī���� ���ͷ�Ʈ 
interrupt[TIM0_OVF] void Timer0(void)
{
 // (1/16)us X 1024 X 256 = 16.384ms
 // 1�ʸ� ������� 0.016384 X sec = 1
 // sec = 1/0.016384 -> �� 61.035  

//�� ����,�ѹ����� �����ֱ� ���ؼ� flag���� ���
//�ܺ� ���ͷ�Ʈ�� �޴��� ä�͸��� �����ϱ� ���ؼ�  
 if(hm_flag==1){hm_flag=0;min++;}//������
 if(hm_flag==2){hm_flag=0;hour++;}//�� ����  
 
 scale++;
 if(scale>=61){scale=0; sec++;}//1�� ���� ����
 if(sec>=60){sec=0; min++;}//1�� ���� ����
 if(min>=60){min=0; hour++;}//1�ð� ���� ����

 if(hour==12&&AP_flag==0){AP_flag=1; AP++;}//AM->PM �ѹ��� ����
 if(hour!=12)AP_flag=0;
 
 if(AP>1)AP=0;
 if(hour>12)hour=1;
 
 s2=sec/10; s1=sec%10;
 m2=min/10; m1=min%10;  
 h2=hour/10; h1=hour%10;  
} 
