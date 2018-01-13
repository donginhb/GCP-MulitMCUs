/**
  ******************************************************************************
  * @file    HAL_Pwm.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   Hal pwm drivers
  ******************************************************************************
  * @attention
  * 
  * LPC17XX:
  *   MCPWM: NODE 0~2
  *   TIM:   NODE 3~6
  *   PWM:   NODE 7~12
  * 
  * MCPWM: 无预分频处理器,所以频率与分辨率互斥,频率越高,分辨率越低,反之亦然;
  *        本模块无法应用于分辨率需要明确的场合;
  * TIM:   只有比较输出,所以无法调节占空比;
  * PWM:   功能齐全,分辨率,频率,分辨都可控;
  * 
  * V1.1------------
  * 修改描述: 整合LPC17XX平台,将MCPWM,TIM,PWM等模块整合在一起
  * 修改作者: Duhanfeng
  * 当前版本: V1.1
  * 修改日期: 2017.08.08
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Pwm.h"
#include "DataType/DataType.h"

#if defined(LPC17XX)

#include "HwDrivers/HW_Mcpwm.h"
#include "HwDrivers/HW_TimeOutput.h"
#include "HwDrivers/HW_Pwm.h"

#elif defined(STM32F10X)
#include "HwDrivers/HW_TimeOutput.h"
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//LPC17XX PWM模块定义
#define HAL_LPC17XX_MCPWM_MAX_NODE      (3)
#define HAL_LPC17XX_TIMER_MAX_NODE      (7)
#define HAL_LPC17XX_PWM_MAX_NODE        (13)
#define HAL_LPC17XX_MAX_NODE            (HAL_LPC17XX_PWM_MAX_NODE)

/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
static uBit8 HAL_PWM_GetNodeIndex(uBit8 uPwmNode)
{
    uBit8 uPwmNodeIndex = 0;
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        uPwmNodeIndex = uPwmNode;
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        uPwmNodeIndex = (uPwmNode - HAL_LPC17XX_MCPWM_MAX_NODE);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        uPwmNodeIndex = (uPwmNode - HAL_LPC17XX_TIMER_MAX_NODE);
    }
    
    return uPwmNodeIndex;
}



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
    
    //数据校验
    if (uPwmNode >= HAL_LPC17XX_MAX_NODE)
    {
        return;
    }
    
    //获取节点索引
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        HW_MCPWM_InitOutput(uPwmNodeIndex, false);
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        HW_TIM_OutputInit(uPwmNodeIndex, uChannelMask);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        HW_PWM_Init(uPwmNodeIndex, uChannelMask);
        
    }
    
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
    
    //数据校验
    if (uPwmNode >= HAL_LPC17XX_MAX_NODE)
    {
        return;
    }
    
    //获取节点索引
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        HW_TIM_OutputEnable(uPwmNodeIndex, bIsEnble);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        HW_PWM_EnableChannel(uPwmNodeIndex, uChannelMask, bIsEnble);
    }
    
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
    
    //数据校验
    if (uPwmNode >= HAL_LPC17XX_MAX_NODE)
    {
        return;
    }
    
    //获取节点索引
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        HW_MCPWM_SetOutputPwmDutyRatio(uPwmNodeIndex, fDutyRatio);
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        HW_TIM_SetOutputPwmDutyRatio(uPwmNodeIndex, uChannelMask, fDutyRatio);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        HW_PWM_SetOutputPwmDutyRatio(uPwmNodeIndex, uChannelMask, fDutyRatio);
    }
    
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
    
    //数据校验
    if (uPwmNode >= HAL_LPC17XX_MAX_NODE)
    {
        return;
    }
    
    //获取节点索引
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        HW_MCPWM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        HW_TIM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        HW_PWM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
    }
    
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
    
    //数据校验
    if (uPwmNode >= HAL_LPC17XX_MAX_NODE)
    {
        return;
    }
    
    //获取节点索引
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_MAX_NODE)
    {
        HW_MCPWM_EnableOutput(uPwmNodeIndex, bIsEnablle);
    }
    else if (uPwmNode < HAL_LPC17XX_TIMER_MAX_NODE)
    {
        HW_TIM_OutputEnable(uPwmNodeIndex, bIsEnablle);
    }
    else if (uPwmNode < HAL_LPC17XX_PWM_MAX_NODE)
    {
        HW_PWM_OutputEnable(uPwmNodeIndex, bIsEnablle);
    }
    
#elif defined(STM32F10X)
    HW_TIM_OutputEnable(uPwmNode, bIsEnablle);
#endif
    
}

