#ifndef  __SYS_CNC_CONFIG_H
#define  __SYS_CNC_CONFIG_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  CNC初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCNC(void);
    
#ifdef __cplusplus
}
#endif

#endif /* __SYS_CNC_CONFIG_H */
