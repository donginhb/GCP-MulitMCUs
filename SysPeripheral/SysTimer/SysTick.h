#ifndef  __SYS_TICK_H
#define  __SYS_TICK_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/*****************************************************************************
 * 系统滴答控制接口
 ****************************************************************************/

/**
  * @brief  系统滴答初始化函数
  * @param  None
  * @retval None
  */
void SysTick_Init(void);


/**
  * @brief  系统滴答时基获取
  * @param  None
  * @retval float32 系统滴答时基
  */
uBit32 SysTick_GetCount(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_TICK_H */
