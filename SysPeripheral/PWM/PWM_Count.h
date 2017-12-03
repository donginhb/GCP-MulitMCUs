#ifndef  __PWM_COUNT_H
#define  __PWM_COUNT_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * PWM输入相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM输入计数初始化
  * @param  uTimeNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void PWM_InputCountInit(uBit8 uTimeNode, uBit8 uChannelMask);


/**
  * @brief  PWM使能
  * @param  uTimeNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void PWM_InputCountEnable(uBit8 uTimeNode, bool bIsEnablle);


/**
  * @brief  PWM计数值获取
  * @param  uTimeNode PWM节点
  * @retval PWM计数寄存器值
  */
uBit32 PWM_GetInputCount(uBit8 uTimeNode);


/*****************************************************************************
 * PWM输入计数模式相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM最大输入计数设置
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void PWM_SetMaxInputCount(uBit8 uTimeNode, uBit8 uChannelNum, uBit32 ulMaxCount);


/*****************************************************************************
 * PWM输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM输入计数中断使能
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  bIsEnable 使能位
  * @retval None
  */
void PWM_EnableInputCountIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable);


/**
  * @brief  PWM输入中断状态获取
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool PWM_GetInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum);


/**
  * @brief  PWM输入中断标志清除
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void PWM_ClearInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum);


#ifdef __cplusplus
}
#endif

#endif /* __PWM_COUNT_H */
