#ifndef  __HAL_PWM_COUNT_H
#define  __HAL_PWM_COUNT_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 定时器输入相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入计数初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void HAL_TIM_InputCountInit(uBit8 uTimeNode, uBit8 uChannelMask);


/**
  * @brief  定时器使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnablle 定时器使能位
  * @retval None
  */
void HAL_TIM_InputCountEnable(uBit8 uTimeNode, bool bIsEnablle);


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uBit32 HAL_TIM_GetInputCount(uBit8 uTimeNode);


/*****************************************************************************
 * 定时器输入计数模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器最大输入计数设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void HAL_TIM_SetMaxInputCount(uBit8 uTimeNode, uBit8 uChannelNum, uBit32 ulMaxCount);


/*****************************************************************************
 * 定时器输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HAL_TIM_EnableInputCountIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable);


/**
  * @brief  定时器输入中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HAL_TIM_GetInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  定时器输入中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIM_ClearInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum);



#ifdef __cplusplus
}
#endif

#endif /* __HAL_PWM_COUNT_H */
