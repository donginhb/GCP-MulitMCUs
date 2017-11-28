#ifndef  __HAL_SYSTICK_H
#define  __HAL_SYSTICK_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * SysTick 时间管理
 ****************************************************************************/

/**
  * @brief  滴答时间设置
  * @param  ulTickTime 滴答时间
  * @retval None
  */
void HAL_SysTick_SetTickTime(uBit32 ulTickTime);


/**
  * @brief  系统滴答计数更新
  * @param  None
  * @retval None
  * @note   此函数放在m_ulSysTickTime毫秒间隔的定时中断中执行
  */
void HAL_SysTick_Update(void);


/**
  * @brief  系统滴答计数获取
  * @param  None
  * @retval 系统滴答计数值
  * @note   若1MS记一个数,约49.7天一个循环
  */
uBit32 HAL_SysTick_GetCount(void);


/**
  * @brief  系统滴答初始化函数
  * @param  ulTickTime 中断周期(单位是毫秒)
  * @retval None
  */
void HAL_SysTick_Init(uBit32 ulTickTime);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYSTICK_H */

