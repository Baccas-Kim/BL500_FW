/* =========================================================
// Filename : qbuffer.h
// 
//
==========================================================*/
/* original from================================================
//[출처] [19년도 AVR 펌웨어 실습] - UART , 6기 97조한철|작성자 바람
//https://blog.naver.com/chcbaram/221528379706
//========================================================*/


#include "def.h"


typedef struct
{
  uint32_t   ptr_in;
  uint32_t   ptr_out;
  uint32_t   length;
  uint8_t   *p_buf;
} qbuffer_t;


void qbufferInit(void);

bool     qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length);
bool     qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
bool     qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
uint32_t qbufferAvailable(qbuffer_t *p_node);
void     qbufferFlush(qbuffer_t *p_node);
