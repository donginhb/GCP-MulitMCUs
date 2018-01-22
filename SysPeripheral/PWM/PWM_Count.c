/**
  ******************************************************************************
  * @file    PWM_Count.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.04
  * @brief   pwm application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "PWM_Count.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Pwm.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/*****************************************************************************
 * PWM输入计数模式相关控制接口
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
void PWM_InputCountInit(uBit8 uTimeNode, uBit8 uChannelMask)
{
    HAL_PWM_InitInputCount(uTimeNode, uChannelMask);
    
}


/**
  * @brief  PWM使能
  * @param  uTimeNode PWM节点
  * @param  bIsEnable PWM使能位
  * @retval None
  */
void PWM_EnableInputCount(uBit8 uTimeNode, bool bIsEnable)
{
    HAL_PWM_EnableInputCount(uTimeNode, bIsEnable);
    
}


/**
  * @brief  PWM计数值获取
  * @param  uTimeNode PWM节点
  * @retval PWM计数寄存器值
  */
uBit32 PWM_GetInputCount(uBit8 uTimeNode)
{
    
    return HAL_PWM_GetInputCount(uTimeNode);
}


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
void PWM_SetMaxInputCount(uBit8 uTimeNode, uBit8 uChannelNum, uBit32 ulMaxCount)
{
    HAL_PWM_SetMaxInputCount(uTimeNode, uChannelNum, ulMaxCount);
    
}


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
void PWM_EnableInputCountIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable)
{
    HAL_PWM_EnableInputCountIRQ(uTimeNode, uChannelNum, bIsEnable);
    
}


/**
  * @brief  PWM输入中断状态获取
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool PWM_GetInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum)
{
    
    return HAL_PWM_GetInputCountITStatus(uTimeNode, uChannelNum);
}


/**
  * @brief  PWM输入中断标志清除
  * @param  uTimeNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void PWM_ClearInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum)
{
    HAL_PWM_ClearInputCountITStatus(uTimeNode, uChannelNum);
    
}

