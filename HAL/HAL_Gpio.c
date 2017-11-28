/**
  ******************************************************************************
  * @file    HAL_Gpio.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.08.22
  * @brief   Gpio drivers
  ******************************************************************************
  * @attention
  * 
  * 设置重试机制,如果FLASH执行失败,重试一定次数.
  * 具体数值由宏FLASH_OPERA_RETRY_NUM 所定义
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Gpio.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_Gpio.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * GPIO相关控制接口
 ****************************************************************************/

/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOD(uBit16 nPort, uBit16 nPin)
{
    HW_GPIO_ConfigOD(nPort, nPin);
    
}


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigInput(uBit16 nPort, uBit16 nPin)
{
    HW_GPIO_ConfigInput(nPort, nPin);
    
}


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOutput(uBit16 nPort, uBit16 nPin)
{
    HW_GPIO_ConfigOutput(nPort, nPin);
    
}


/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HAL_GPIO_SetOutputState(uBit16 nPort, uBit16 nPin, bool bState)
{
    HW_GPIO_SetOutputState(nPort, nPin, bState);
    
}


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HAL_GPIO_GetIOState(uBit16 nPort, uBit16 nPin)
{
    
    return HW_GPIO_GetIOState(nPort, nPin);
}

