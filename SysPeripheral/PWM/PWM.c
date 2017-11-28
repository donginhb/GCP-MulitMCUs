/**
  ******************************************************************************
  * @file    PWM.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
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
#include "PWM.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Pwm.h"

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
void PWM_Init(uBit8 uPwmNode, uBit8 uChannelMask)
{
    HAL_PWM_Init(uPwmNode, uChannelMask);
    
}


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void PWM_EnableChannel(uBit8 uPwmNode, uBit8 uChannelMask, bool bIsEnble)
{
    HAL_PWM_EnableChannel(uPwmNode, uChannelMask, bIsEnble);
    
}


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void PWM_SetOutputPwmDutyRatio(uBit8 uPwmNode, uBit8 uChannelMask, float fDutyRatio)
{
    HAL_PWM_SetOutputPwmDutyRatio(uPwmNode, uChannelMask, fDutyRatio);
    
}



/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void PWM_SetOutputPwmFrq(uBit8 uPwmNode, uBit32 ulFrequency)
{
    HAL_PWM_SetOutputPwmFrq(uPwmNode, ulFrequency);
    
}


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnablle)
{
    HAL_PWM_OutputEnable(uPwmNode, bIsEnablle);
    
}
