/* =========================================================
// Filename : qbuffer.c
// 
//
==========================================================*/
/* original from================================================
//[출처] [19년도 AVR 펌웨어 실습] - UART , 6기 97조한철|작성자 바람
//https://blog.naver.com/chcbaram/221528379706
//========================================================*/

#include "def.h"
#include "qbuffer.h"

void qbufferInit(void)
{

}

bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
  bool ret = true;


  p_node->ptr_in  = 0;
  p_node->ptr_out = 0;
  p_node->length  = length;
  p_node->p_buf   = p_buf;

  if (p_node->p_buf == NULL)
  {
    p_node->length = 0;
    ret = false;
  }

  return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t index;
  uint32_t next_index;
  uint32_t i;

  if (p_node->p_buf == NULL) return false;


  for (i=0; i<length; i++)
  {
    index      = p_node->ptr_in;
    next_index = p_node->ptr_in + 1;

    if (next_index == p_node->length)
    {
      next_index = 0;;
    }

    if (next_index != p_node->ptr_out)
    {
      p_node->p_buf[index] = p_data[i];
      p_node->ptr_in       = next_index;
    }
    else
    {
      ret = false; // ERR_FULL
      break;
    }
  }

  return ret;
}

bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t index;
  uint32_t next_index;
  uint32_t i;

  if (p_node->p_buf == NULL) return false;


  for (i=0; i<length; i++)
  {
    index      = p_node->ptr_out;
    next_index = p_node->ptr_out + 1;

    if (next_index == p_node->length)
    {
      next_index = 0;
    }

    if (index != p_node->ptr_in)
    {
      p_data[i]       = p_node->p_buf[index];
      p_node->ptr_out = next_index;
    }
    else
    {
      ret = false; // ERR_EMPTY
      break;
    }
  }

  return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
  uint32_t length;


  length = (p_node->length + p_node->ptr_in - p_node->ptr_out) % p_node->length;

  return length;
}

void qbufferFlush(qbuffer_t *p_node)
{
  p_node->ptr_in  = 0;
  p_node->ptr_out = 0;
}
