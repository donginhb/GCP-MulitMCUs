#ifndef  __HAL_GPIO_H
#define  __HAL_GPIO_H

#include "HwDrivers/HW_Gpio.h"
#include "DataType/DataType.h"


/*****************************************************************************
 * GPIO相关控制接口
 ****************************************************************************/

/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOD(uBit16 nPort, uBit16 nPin);


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigInput(uBit16 nPort, uBit16 nPin);


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOutput(uBit16 nPort, uBit16 nPin);


/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HAL_GPIO_SetOutputState(uBit16 nPort, uBit16 nPin, bool bState);


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HAL_GPIO_GetIOState(uBit16 nPort, uBit16 nPin);
    
    
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __HAL_GPIO_H */
