#ifndef  __GCP_COM_MAN_H
#define  __GCP_COM_MAN_H

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
uBit32 GCP_ComInit(void);


/**
  * @brief  通信处理
  * @param  None
  * @retval None
  */
void GCP_ComHandler(void);
    

#ifdef __cplusplus
}
#endif

#endif /* __GCP_COM_MAN_H */
