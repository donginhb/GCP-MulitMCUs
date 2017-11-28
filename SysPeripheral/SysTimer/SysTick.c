/**
  ******************************************************************************
  * @file    SysTick.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   SysTick API
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysTick.h"
#include "DataType/DataType.h"
#include "HAL/HAL_SysTick.h"


/*****************************************************************************
 * 系统滴答控制接口
 ****************************************************************************/

/**
  * @brief  系统滴答初始化函数
  * @param  None
  * @retval None
  */
void SysTick_Init(void)
{
    HAL_SysTick_Init(1);
    
}


/**
  * @brief  系统滴答时基获取
  * @param  None
  * @retval float32 系统滴答时基
  */
uBit32 SysTick_GetCount(void)
{
    
    return HAL_SysTick_GetCount();
}


