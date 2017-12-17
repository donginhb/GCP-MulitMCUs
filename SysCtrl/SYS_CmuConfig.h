#ifndef  __SYS_CMU_CONFIG_H
#define  __SYS_CMU_CONFIG_H

#include "../DataType/DataType.h"
#include "SysConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/**
  * @brief  系统初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_CMU_CONFIG_H */
