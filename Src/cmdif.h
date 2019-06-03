//=============================================
//
//[출처] ARM 프로젝트 실습2 - Cmdif|작성자 바람
//
//


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define CMDIF_LIST_MAX            16
#define CMDIF_CMD_STR_MAX         16
#define CMDIF_CMD_BUF_LENGTH      128


typedef struct
{
  int argc;
  char **argv;
} cmdif_param_t;


typedef struct
{
  char  cmd_str[CMDIF_CMD_STR_MAX];
  void (*cmd_func)(void);
} cmdif_cmd_node_t;


typedef struct
{
  uint8_t		     init;
  uint8_t            err_code;
  uint8_t            ch;
  uint32_t           baud;
  uint16_t           index;
  uint8_t            exit;

  cmdif_param_t      param;
  cmdif_cmd_node_t  *node;

  char  read_buffer[CMDIF_CMD_BUF_LENGTH];
  char *p_read_buffer;

} cmdif_cmd_t;



uint8_t cmdifInit(void);
uint8_t  cmdifIsInit(void);
void cmdifOpen(uint8_t ch, uint32_t baud);
void cmdifAdd(const char *cmd_str, void (*p_func)(void));
void cmdifMain(void);
void cmdifLoop(void);

void cmdifPrintf(const char *fmt, ...);
void cmdifPrint(char *str);
void cmdifPutch(char data);
uint8_t  cmdifGetch(void);
uint32_t cmdifRxAvailable(void);


uint8_t cmdifHasString(char *p_str, uint8_t index);
unsigned long cmdifGetParam(uint8_t index);
uint32_t cmdifGetParamCnt(void);
