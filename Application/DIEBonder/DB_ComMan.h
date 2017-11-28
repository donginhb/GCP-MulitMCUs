#ifndef  __DB_COM_MAN_H
#define  __DB_COM_MAN_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  通信管理单元初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 DB_ComInit(void);


/**
  * @brief  通信处理
  * @param  None
  * @retval None
  */
void DB_ComHandler(void);
    

#ifdef __cplusplus
}
#endif

#endif /* __DB_COM_MAN_H */
