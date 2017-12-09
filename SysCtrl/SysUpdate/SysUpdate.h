#ifndef  __SYS_UPDATE_H
#define  __SYS_UPDATE_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 1-失败
  * @note   本函数决定是跳转到APP还是执行Bootloader
  */
uBit32 SYS_UPDATE_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_UPDATE_H */
