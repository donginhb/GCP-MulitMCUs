/**
  ******************************************************************************
  * @file    HAL_PwmCount.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.04
  * @brief   Pwm count mode driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_PwmCount.h"
#include "DataType/DataType.h"

#if defined(LPC17XX)
#include "HwDrivers/HW_TimeInput.h"
#elif defined(STM32F10X)
#include "HwDrivers/HW_TimeOutput.h"
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * PWM 数量控制相关接口
 ****************************************************************************/

#if 0
#if 0
extern 
void HW_TIM_SetMaxInputCount(uBit8 uTimeNode, uBit8 uChannelNum, uBit32 ulMaxCount);

//输入计数中断
static void HAL_PWM_InputCountHanlder(void)
{
    if (HW_TIM_GetInputITStatus(m_uPwmCapNode, m_uCapChNum))
    {
        HW_TIM_ClearInputITStatus(m_uPwmCapNode, m_uCapChNum);    //清标志位
        
        HAL_PWM_EnableChannel(m_uPwmOuputNode, m_uOuputChNum, false);   //关闭PWM输出
        //HW_TIM_EnableInputIRQ(uPwmCapNode, uCapChNum, false);           //关闭PWM捕获
        HW_TIM_InputEnable(m_uPwmCapNode, false);
    }
    
}
#endif


void HAL_PWM_CountInit(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit8 uPwmCapNode, uBit8 uCapChNum)
{
#if defined(LPC17XX)
    HW_TIM_InputInit(uPwmCapNode, 0x1<<uCapChNum, 0);
    HW_TIM_EnableInputIRQ(uPwmCapNode, uCapChNum, true);
    
#elif defined(STM32F10X)
#endif
    
}


void HAL_PWM_SendCount(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit32 ulPwmCount)
{
    //设置捕获值
    HW_TIM_SetMaxInputCount(m_uPwmCapNode, m_uCapChNum, ulPwmCount);
    
    //使能捕获
    HW_TIM_InputEnable(m_uPwmCapNode, true);
    
#if 0
    //使能输出
    HAL_PWM_EnableChannel(m_uPwmOuputNode, m_uOuputChNum, true);
#endif
    
    
}
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
void HAL_TIM_InputCountInit(uBit8 uTimeNode, uBit8 uChannelMask)
{
#if defined(LPC17XX)
    HW_TIM_InputInit(uTimeNode, uChannelMask, HW_TIME_INPUT_COUNT_MODE);
#endif
    
}


/**
  * @brief  定时器使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnablle 定时器使能位
  * @retval None
  */
void HAL_TIM_InputCountEnable(uBit8 uTimeNode, bool bIsEnablle)
{
    HW_TIM_InputEnable(uTimeNode, bIsEnablle);
    
}


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uBit32 HAL_TIM_GetInputCount(uBit8 uTimeNode)
{
    
    return HW_TIM_GetInputCount(uTimeNode);
}


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
void HAL_TIM_SetMaxInputCount(uBit8 uTimeNode, uBit8 uChannelNum, uBit32 ulMaxCount)
{
    HW_TIM_SetMaxInputCount(uTimeNode, uChannelNum, ulMaxCount);
    
}


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
void HAL_TIM_EnableInputCountIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable)
{
    HW_TIM_EnableInputIRQ(uTimeNode, uChannelNum, bIsEnable);
    
}


/**
  * @brief  定时器输入中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HAL_TIM_GetInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum)
{
    
    return HW_TIM_GetInputITStatus(uTimeNode, uChannelNum);
}


/**
  * @brief  定时器输入中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIM_ClearInputCountITStatus(uBit8 uTimeNode, uBit8 uChannelNum)
{
    HW_TIM_ClearInputITStatus(uTimeNode, uChannelNum);
    
}
