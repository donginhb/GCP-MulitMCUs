/**
  ******************************************************************************
  * @file    HAL_SysTick.h
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.06
  * @brief   SysTick Driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "HAL_SysTick.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_SysTick.h"


/*****************************************************************************
 * SysTick 时间管理
 ****************************************************************************/

/**
  * @brief  系统滴答计数获取
  * @param  None
  * @retval 系统滴答计数值
  */
uBit32 HAL_SysTick_GetCount(void)
{
    
    return HW_SysTick_GetCount();
}


/**
  * @brief  系统滴答初始化函数
  * @param  ulTickTime 中断周期(单位是毫秒)
  * @retval None
  */
void HAL_SysTick_Init(uBit32 ulTickTime)
{
    HW_SysTick_Init(ulTickTime);
 
}
