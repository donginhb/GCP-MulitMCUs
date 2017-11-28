#ifndef  __SYS_CTRL_H
#define  __SYS_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/*****************************************************************************
 * 系统复位相关接口函数
 ****************************************************************************/

/**
  * @brief  立即复位
  * @param  None
  * @retval None
  */
void SysCtrl_ResetSystemNow(void);


/**
  * @brief  延时复位
  * @param  ulDelayMs 延时的时间(单位:MS)
  * @retval None
  */
void SysCtrl_ResetSystemDelay(uBit32 ulDelayMs);


/**
  * @brief  系统复位处理(放在while循环中处理)
  * @param  None
  * @retval None
  */
void SysCtrl_ResetHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_CTRL_H */
