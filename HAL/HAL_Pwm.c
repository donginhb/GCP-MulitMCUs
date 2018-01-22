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
  *        本模块无法应用于分辨率固定而频率可变的场合;
  * TIM:   只有比较输出,所以无法调节占空比;
  * PWM:   功能齐全,分辨率,频率,占空比都可控;因为LPC43XX无PWM模块,所以考虑到两个
  *        平台之间的移植成本,在能满足需求的前提下,尽量改用MCPWM/TIM;
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
#include "HwDrivers/HW_Tim.h"
#include "HwDrivers/HW_Pwm.h"
#include "HwDrivers/HW_IRQHandler.h"

#elif defined(STM32F10X)
#include "HwDrivers/HW_TimeOutput.h"
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//LPC17XX PWM模块定义
#define HAL_LPC17XX_MCPWM_NODE_COUNT        (3)
#define HAL_LPC17XX_TIMER_NODE_COUNT        (4)
#define HAL_LPC17XX_PWM_NODE_COUNT          (1)
#define HAL_LPC17XX_MAX_NODE_COUNT          (HAL_LPC17XX_MCPWM_NODE_COUNT + \
                                             HAL_LPC17XX_TIMER_NODE_COUNT + \
                                             HAL_LPC17XX_PWM_NODE_COUNT)

//LPC17XX 模块定义
#define HAL_LPC17XX_MCPWM_MODULE            (0)
#define HAL_LPC17XX_TIMER_MODULE            (1)
#define HAL_LPC17XX_PWM_MODULE              (2)
#define HAL_LPC17XX_RESERVER_MODULE         (0xFF)

//LPC17XX 通道掩码定义
#define HAL_LPC17XX_TIM_CHANNEL_MASK(x)     ((x) & 0x03)
#define HAL_LPC17XX_TIM_PWM_MASK(x)         ((x) & 0x3F)


/**
  * @brief  PWM 节点索引获取
  * @param  uPwmNode PWM节点
  * @retval PWM 节点索引
  */
static uBit8 HAL_PWM_GetNodeIndex(uBit8 uPwmNode)
{
    uBit8 uPwmNodeIndex = 0;
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_NODE_COUNT)
    {
        uPwmNodeIndex = uPwmNode;
    }
    else if (uPwmNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT))
    {
        uPwmNodeIndex = (uPwmNode - HAL_LPC17XX_MCPWM_NODE_COUNT);
    }
    else if (uPwmNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT + HAL_LPC17XX_PWM_NODE_COUNT))
    {
        uPwmNodeIndex = (uPwmNode - (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT));
    }
    
    return uPwmNodeIndex;
}


/**
  * @brief  PWM 模块索引获取
  * @param  uPwmNode PWM节点
  * @retval PWM 模块索引
  */
static uBit8 HAL_PWM_GetModuleIndex(uBit8 uPwmNode)
{
    uBit8 uModuleIndex = 0;
    
    if (uPwmNode < HAL_LPC17XX_MCPWM_NODE_COUNT)
    {
        uModuleIndex = HAL_LPC17XX_MCPWM_MODULE;
    }
    else if (uPwmNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT))
    {
        uModuleIndex = HAL_LPC17XX_TIMER_MODULE;
    }
    else if (uPwmNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT + HAL_LPC17XX_PWM_NODE_COUNT))
    {
        uModuleIndex = HAL_LPC17XX_PWM_MODULE;
    }
    else 
    {
        uModuleIndex = HAL_LPC17XX_RESERVER_MODULE;
    }
    
    return uModuleIndex;
}


/*****************************************************************************
 * PWM 输出相关控制接口
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
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_InitOutput(uPwmNodeIndex, false);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_InitOutput(uPwmNodeIndex, HAL_LPC17XX_TIM_CHANNEL_MASK(uChannelMask));
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        HW_PWM_Init(uPwmNodeIndex, HAL_LPC17XX_TIM_PWM_MASK(uChannelMask));
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif defined(STM32F10X)
    HW_TIM_InitOutput(uPwmNode, uChannelMask, TIM_OUTPUT_MODE_PWM1);
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
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_EnableChannel(uPwmNodeIndex, HAL_LPC17XX_TIM_CHANNEL_MASK(uChannelMask), bIsEnble);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        HW_PWM_EnableChannel(uPwmNodeIndex, HAL_LPC17XX_TIM_PWM_MASK(uChannelMask), bIsEnble);
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
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
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_SetOutputPwmDutyRatio(uPwmNodeIndex, fDutyRatio);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_SetOutputPwmDutyRatio(uPwmNodeIndex, HAL_LPC17XX_TIM_CHANNEL_MASK(uChannelMask), fDutyRatio);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        HW_PWM_SetOutputPwmDutyRatio(uPwmNodeIndex, HAL_LPC17XX_TIM_PWM_MASK(uChannelMask), fDutyRatio);
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
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
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        HW_PWM_SetOutputPwmFrq(uPwmNodeIndex, ulFrequency);
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif defined(STM32F10X)
    HW_TIM_SetOutputPwmFrq(uPwmNode, ulFrequency);
#endif
    
}


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnable PWM使能位
  * @retval None
  */
void HAL_PWM_OutputEnable(uBit8 uPwmNode, bool bIsEnable)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_EnableOutput(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_OutputEnable(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        HW_PWM_OutputEnable(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif defined(STM32F10X)
    HW_TIM_OutputEnable(uPwmNode, bIsEnable);
#endif
    
}


/*****************************************************************************
 * PWM 输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入计数初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void HAL_PWM_InitInputCount(uBit8 uPwmNode, uBit8 uChannelMask)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_InitInputCount(uPwmNodeIndex, uPwmNodeIndex, HW_MCPWM_INPUT_TRG_R);    //上升沿触发
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_InitInputCount(uPwmNodeIndex, uChannelMask);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
    
}


/**
  * @brief  定时器使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HAL_PWM_EnableInputCount(uBit8 uPwmNode, bool bIsEnable)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_EnableInputCount(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_EnableInputCount(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
}


/**
  * @brief  定时器计数值获取
  * @param  uPwmNode PWM节点
  * @retval 定时器计数寄存器值
  */
uBit32 HAL_PWM_GetInputCount(uBit8 uPwmNode)
{
    uBit32 ulInputCount = 0;
    
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        ulInputCount = HW_MCPWM_GetCurInputCount(uPwmNodeIndex);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        ulInputCount = HW_TIM_GetCurInputCount(uPwmNodeIndex);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
    return ulInputCount;
}


/**
  * @brief  定时器最大输入计数设置
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void HAL_PWM_SetMaxInputCount(uBit8 uPwmNode, uBit8 uChannelNum, uBit32 ulMaxCount)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_SetMaxInputCount(uPwmNodeIndex, ulMaxCount);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_SetMaxInputCount(uPwmNodeIndex, uChannelNum, ulMaxCount);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
}


/**
  * @brief  定时器输入中断使能
  * @param  uPwmNode PWM节点
  * @retval 中断触发源
  */
uBit32 HAL_PWM_GetIRQTrgSource(uBit8 uPwmNode)
{
    uBit32 ulTrgSource = {0};
    
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        ulTrgSource = LPC_IRQ_TRG_MCPWM0 + uPwmNodeIndex;
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        ulTrgSource = LPC_IRQ_TRG_TIME0 + uPwmNodeIndex;
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
    return ulTrgSource;
}




/**
  * @brief  定时器输入中断使能
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HAL_PWM_EnableInputCountIRQ(uBit8 uPwmNode, uBit8 uChannelNum, bool bIsEnable)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_EnableIRQ(uPwmNodeIndex, HW_TIM_INT_MATCH, bIsEnable);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_EnableIRQ(uPwmNodeIndex, uChannelNum, HW_TIM_INT_MATCH, bIsEnable);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
}


/**
  * @brief  定时器输入中断状态获取
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HAL_PWM_GetInputCountITStatus(uBit8 uPwmNode, uBit8 uChannelNum)
{
    bool bITStatus = false;
    
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_GetITStatus(uPwmNodeIndex, HW_TIM_INT_MATCH);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        bITStatus = HW_TIM_GetITStatus(uPwmNodeIndex, uChannelNum, HW_TIM_INT_MATCH);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    return bITStatus;
}


/**
  * @brief  定时器输入中断标志清除
  * @param  uPwmNode PWM节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_PWM_ClearInputCountITStatus(uBit8 uPwmNode, uBit8 uChannelNum)
{
#if defined(LPC17XX)
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uPwmNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uPwmNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        HW_MCPWM_ClearITStatus(uPwmNodeIndex, HW_TIM_INT_MATCH);
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_ClearITStatus(uPwmNodeIndex, uChannelNum, HW_TIM_INT_MATCH);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#endif
    
}























