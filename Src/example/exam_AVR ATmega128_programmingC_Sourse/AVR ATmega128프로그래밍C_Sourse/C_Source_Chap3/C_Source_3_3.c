//LED2_sign
#include<mega128.h>
void main()
{
 char data=-1;//-128로 변경을 하면 ? 
 DDRE=0xFF;//E포트  출력   
 PORTE=data;//LED -> 소스전류 연결
}
