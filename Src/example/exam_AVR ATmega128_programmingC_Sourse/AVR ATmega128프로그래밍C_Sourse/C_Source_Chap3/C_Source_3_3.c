//LED2_sign
#include<mega128.h>
void main()
{
 char data=-1;//-128�� ������ �ϸ� ? 
 DDRE=0xFF;//E��Ʈ  ���   
 PORTE=data;//LED -> �ҽ����� ����
}
