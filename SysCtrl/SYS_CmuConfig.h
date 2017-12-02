#ifndef  __SYS_CMU_CONFIG_H
#define  __SYS_CMU_CONFIG_H

#include "../DataType/DataType.h"
#include "SYS_Config.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if SYS_USING_CMU

/**
  * @brief  系统初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void);


#endif //SYS_USING_CMU





#ifdef __cplusplus
}
#endif

#endif /* __SYS_CMU_CONFIG_H */
