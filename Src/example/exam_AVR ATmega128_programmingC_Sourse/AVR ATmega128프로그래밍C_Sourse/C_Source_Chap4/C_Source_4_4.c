//AVR128  interrupt_4
//10�� ī����
#include<mega128.h>
#include<delay.h>
int FND[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10};
//FNDǥ��   { 0  ,  1 , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  }
int cnt=0,cnt2,cnt1;//10������ ǥ���ϱ� ���� ����
char flag=0;
void main()
{
  DDRA=0xff;//A_PORT OUT 
  DDRC=0xff;//C_PORT OUT  
  DDRD=0x00;//D_PORT IN
  
  PORTA=0xff;//�ʱ� �� A_PORT FND(LED) OFF   
  PORTC=0xff;//�ʱ� �� C_PORT FND(LED) OFF
 
 //���ͷ�Ʈ �ʱ�ȭ
  EICRA=0b00001010;//�ܺ� ���ͷ�Ʈ : �ϰ� ����
  EIMSK=0b00000011;//�ܺ� ���ͷ�Ʈ : �ο��̺�
  SREG=0x80;//���� ���ͷ�Ʈ �ο��̺� ��Ʈ ��
  while(1)
  {   
    cnt2=cnt/10;
    cnt1=cnt%10;
    PORTC=FND[cnt2];//FND(+com) Sink_current
    PORTA=FND[cnt1];//FND(+com) Sink_current    
    delay_ms(300); 
    if(flag==0)cnt=cnt;
    if(flag==1)cnt++;    
    if(cnt>59)cnt=0; 
  }
  
}

//�ܺ� ���ͷ�Ʈ 0 
interrupt[EXT_INT0] void PD0_int0(void)
{
  flag=0;
}

//�ܺ� ���ͷ�Ʈ 1 
interrupt[EXT_INT1] void PD1_int1(void)
{
  flag=1;
}