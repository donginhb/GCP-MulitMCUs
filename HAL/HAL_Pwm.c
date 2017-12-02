/**
  ******************************************************************************
  * @file    HAL_Pwm.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Pwm.h"
#include "DataType/DataType.h"

#if defined(LPC17XX)
#if 0
#include "HwDrivers/HW_Pwm.h"
#else
#include "HwDrivers/HW_Pwm.h"
#include "HwDrivers/HW_TimeOutput.h"
#include "HwDrivers/HW_TimeInput.h"
#endif
#elif defined(STM32F10X)
#include "HwDrivers/HW_TimeOutput.h"
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * PWM 相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HAL_PWM_Init(uBit8 uPwmNode, uBit8 uChannelMask)
{
#if defined(LPC17XX)
#if 0
    HW_PWM_Init(uPwmNode, uChannelMask);
#else 
    HW_PWM_Init(uPwmNode, uChannelMask);
    HW_TIM_OutputInit(uPwmNode, uChannelMask, 0);
#endif
#elif defined(STM32F10X)
    HW_TIM_OutputInit(uPwmNode, uChannelMask, TIM_OUTPUT_MODE_PWM1);
#endif
    
}


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HAL_PWM_EnableChannel(uBit8 uPwmNode, uBit8 uChannelMask, bool bIsEnble)
{
#if defined(LPC17XX)
#if 0
    HW_PWM_EnableChannel(uPwmNode, uChannelMask, bIsEnble);
#else 
    
#endif
#elif defined(STM32F10X)
    
#endif
    
}


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HAL_PWM_SetOutputPwmDutyRatio(uBit8 uPwmNode, uBit8 uChannelMask, float fDutyRatio)
{
#if defined(LPC17XX)
#if 0
    HW_PWM_SetOutputPwmDutyRatio(uPwmNode, uChannelMask, fDutyRatio);
#else 
    HW_PWM_SetOutputPwmDutyRatio(uPwmNode, uChannelMask, fDutyRatio);
    HW_TIM_SetOutputPwmDutyRatio(uPwmNode, uChannelMask, fDutyRatio);
#endif
#elif defined(STM32F10X)
    HW_TIM_SetOutputPwmDutyRatio(uPwmNode, uChannelMask, fDutyRatio);
#endif
    
}


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HAL_PWM_SetOutputPwmFrq(uBit8 uPwmNode, uBit32 ulFrequency)
{
#if defined(LPC17XX)
#if 0
    HW_PWM_SetOutputPwmFrq(uPwmNode, ulFrequency);
#else 
    HW_PWM_SetOutputPwmFrq(uPwmNode, ulFrequency);
    HW_TIM_SetOutputPwmFrq(uPwmNode, ulFrequency);
#endif
#elif defined(STM32F10X)
    HW_TIM_SetOutputPwmFrq(uPwmNode, ulFrequency);
#endif
    
}


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void HAL_PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnablle)
{
#if defined(LPC17XX)
#if 0
    HW_PWM_OutputEnable(uPwmNode, bIsEnablle);
#else
    HW_PWM_OutputEnable(uPwmNode, bIsEnablle);
    HW_TIM_OutputEnable(uPwmNode, bIsEnablle);
#endif
#elif defined(STM32F10X)
    HW_TIM_OutputEnable(uPwmNode, bIsEnablle);
#endif
    
}


/*****************************************************************************
 * PWM 数量控制相关接口
 ****************************************************************************/

static uBit32 m_ulPwmCount = 0;
static uBit8 m_uPwmOuputNode = 0;
static uBit8 m_uOuputChNum = 0;
static uBit8 m_uPwmCapNode = 0;
static uBit8 m_uCapChNum = 0;


//输入计数中断
void HAL_PWM_InputCountHanlder()
{
    if (HW_TIM_GetInputITStatus(m_uPwmCapNode, m_uCapChNum))
    {
        HW_TIM_ClearInputITStatus(m_uPwmCapNode, m_uCapChNum);    //清标志位
        
        HAL_PWM_EnableChannel(m_uPwmOuputNode, m_uOuputChNum, false);   //关闭PWM输出
        //HW_TIM_EnableInputIRQ(uPwmCapNode, uCapChNum, false);           //关闭PWM捕获
    }
    
}




void HAL_PWM_CountInit(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit8 uPwmCapNode, uBit8 uCapChNum)
{
#if defined(LPC17XX)
    HW_TIM_OutputInit(uPwmOuputNode, 0x1<<uOuputChNum, 0);
    HW_TIM_InputInit(uPwmCapNode, 0x1<<uCapChNum, 0);
    HW_TIM_EnableInputIRQ(uPwmCapNode, uCapChNum, true);
    
    m_uPwmOuputNode = uPwmOuputNode;
    m_uOuputChNum = uOuputChNum;
    m_uPwmCapNode = uPwmCapNode;
    m_uCapChNum = uCapChNum;
    
#elif defined(STM32F10X)
#endif
    
}



void HAL_PWM_SendCount(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit32 ulPwmCount)
{
    //设置捕获中断
    
}





