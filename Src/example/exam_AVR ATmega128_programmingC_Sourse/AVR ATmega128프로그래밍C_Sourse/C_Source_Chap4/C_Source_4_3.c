//AVR128  interrupt_3
//10�� ī����
#include<mega128.h>
#include<delay.h>
int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
//FNDǥ��   { 0  ,  1 , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  }
int cnt=0;//10������ ǥ���ϱ� ���� ����
void main()
{
  DDRA=0xff;//A_PORT OUT
  DDRD=0x00;//D_PORT IN
  PORTA=0xff;//�ʱ� �� A_PORT LED OFF
 
 //���ͷ�Ʈ �ʱ�ȭ
  EICRA=0b00000010;//�ܺ� ���ͷ�Ʈ 0 : �ϰ� ����
  EIMSK=0b00000001;//�ܺ� ���ͷ�Ʈ 0 : �ο��̺�
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  while(1)
  {   
   PORTA=FND[cnt];//FND(+com) Sink_current   
   if(cnt>9)cnt=0; 
  }
  
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  delay_ms(10);
  if(PIND.0==0)cnt++;
  if(PIND.0==1)cnt=cnt; 
  delay_ms(10);
}
