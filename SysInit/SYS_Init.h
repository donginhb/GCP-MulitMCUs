#ifndef  __SYS_INIT_H
#define  __SYS_INIT_H

#include "../DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  系统初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_Init(void);


/**
  * @brief  系统主任务处理
  * @param  None
  * @retval None
  */
void SYS_MainTaskHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_INIT_H */
