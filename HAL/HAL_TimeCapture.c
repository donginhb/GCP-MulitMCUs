/**
  ******************************************************************************
  * @file    HAL_TimeCapture.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.30
  * @brief   hal time capture drivers
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_TimeCapture.h"
#include "DataType/DataType.h"

#if (defined(LPC17XX)||defined(LPC43XX))
#include "HwDrivers/HW_Mcpwm.h"
#include "HwDrivers/HW_Tim.h"
#if defined(LPC17XX)
#include "HwDrivers/HW_Pwm.h"
#endif
#elif (defined(STM32F10X)||defined(STM32F0XX))
#include "HwDrivers/HW_TimeInput.h"
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#if defined(LPC17XX)
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
  * @param  uTimeNode PWM节点
  * @retval PWM 节点索引
  */
static uBit8 HAL_PWM_GetNodeIndex(uBit8 uTimeNode)
{
    uBit8 uPwmNodeIndex = 0;
    
    if (uTimeNode < HAL_LPC17XX_MCPWM_NODE_COUNT)
    {
        uPwmNodeIndex = uTimeNode;
    }
    else if (uTimeNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT))
    {
        uPwmNodeIndex = (uTimeNode - HAL_LPC17XX_MCPWM_NODE_COUNT);
    }
    else if (uTimeNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT + HAL_LPC17XX_PWM_NODE_COUNT))
    {
        uPwmNodeIndex = (uTimeNode - (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT));
    }
    
    return uPwmNodeIndex;
}


/**
  * @brief  PWM 模块索引获取
  * @param  uTimeNode PWM节点
  * @retval PWM 模块索引
  */
static uBit8 HAL_PWM_GetModuleIndex(uBit8 uTimeNode)
{
    uBit8 uModuleIndex = 0;
    
    if (uTimeNode < HAL_LPC17XX_MCPWM_NODE_COUNT)
    {
        uModuleIndex = HAL_LPC17XX_MCPWM_MODULE;
    }
    else if (uTimeNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT))
    {
        uModuleIndex = HAL_LPC17XX_TIMER_MODULE;
    }
    else if (uTimeNode < (HAL_LPC17XX_MCPWM_NODE_COUNT + HAL_LPC17XX_TIMER_NODE_COUNT + HAL_LPC17XX_PWM_NODE_COUNT))
    {
        uModuleIndex = HAL_LPC17XX_PWM_MODULE;
    }
    else 
    {
        uModuleIndex = HAL_LPC17XX_RESERVER_MODULE;
    }
    
    return uModuleIndex;
}
#endif


/*****************************************************************************
 * 定时器捕获模式相关控制接口
 ****************************************************************************/

/**
  * @brief  捕获模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_TIME_InitCapture(uBit8 uTimeNode, uBit8 uChannelMask, uBit8 uTrgSource)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    uBit8 uChannelNum = 0;
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        
        for (int i = 0; i < 8; i++)
        {
            if (uChannelMask & (0x1<<i))
            {
                uChannelNum = i;
                break;
            }
        }
        
        HW_TIM_InitCapture(uPwmNodeIndex, uChannelNum, uTrgSource);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIM_InitInputMode(uTimeNode, uChannelMask, TIM_INPUT_MODE_NORMAL);
#endif
    
    return 0;
}


/**
  * @brief  捕获模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uTrgSource 触发源(1-上升沿捕获 2-下降沿捕获 3-双边沿捕获)
  * @retval None
  */
void HAL_TIME_SetCaptureEdge(uBit8 uTimeNode, uBit8 uChannelNum, uBit8 uTrgSource)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        
        HW_TIM_SetCaptureEdge(uPwmNodeIndex, uChannelNum, uTrgSource);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    
#endif
    
}






/**
  * @brief  定时器输入使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HAL_TIME_EnableCapture(uBit8 uTimeNode, bool bIsEnable)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_EnableCapture(uPwmNodeIndex, bIsEnable);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIM_EnableInput(uTimeNode, bIsEnable);
#endif
    
}


/**
  * @brief  定时器输入捕获值复位
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIME_ResetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum)
{

#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_ResetCounter(uPwmNodeIndex);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
    
#elif (defined(STM32F10X)||defined(STM32F0XX))

#endif
    
}


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uBit32 HAL_TIME_GetCaptureValue(uBit8 uTimeNode, uBit8 uChannelNum)
{
    uBit32 ulCaptureValue = 0;
    
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        ulCaptureValue = HW_TIM_GetCaptureValue(uPwmNodeIndex, uChannelNum);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    ulCaptureValue = HW_TIM_GetCaptureValue(uTimeNode, uChannelNum);
#endif
    
    return ulCaptureValue;
}


/*****************************************************************************
 * 定时器捕获模式中断相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HAL_TIME_EnableCaptureIRQ(uBit8 uTimeNode, uBit8 uChannelNum, bool bIsEnable)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_EnableIRQ(uPwmNodeIndex, uChannelNum, HW_TIM_INT_CAPTURE, bIsEnable);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIM_EnableInputIRQ(uTimeNode, uChannelMask, bIsEnable);
#endif
    
}


/**
  * @brief  定时器捕获中断标志获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起标志
  */
bool HAL_TIME_GetCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum)
{
    bool bFlag = false;
    
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        bFlag = HW_TIM_GetITStatus(uPwmNodeIndex, uChannelNum, HW_TIM_INT_CAPTURE);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    bFlag = HW_TIME_GetCaptureIRQFlag(uTimeNode, uChannelNum);
#endif
    
    return bFlag;
}


/**
  * @brief  定时器捕获中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HAL_TIME_ClearCaptureIRQFlag(uBit8 uTimeNode, uBit8 uChannelNum)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    
    //获取索引
    uBit8 uModuleIndex = HAL_PWM_GetModuleIndex(uTimeNode);
    uBit8 uPwmNodeIndex = HAL_PWM_GetNodeIndex(uTimeNode);
    
    switch (uModuleIndex)
    {
    case HAL_LPC17XX_MCPWM_MODULE: 
        
        break;
    case HAL_LPC17XX_TIMER_MODULE: 
        HW_TIM_ClearITStatus(uPwmNodeIndex, uChannelNum, HW_TIM_INT_CAPTURE);
        break;
    case HAL_LPC17XX_PWM_MODULE: 
        
        break;
    case HAL_LPC17XX_RESERVER_MODULE: break;
    }
    
#elif (defined(STM32F10X)||defined(STM32F0XX))
    HW_TIME_ClearCaptureIRQFlag(uTimeNode, uChannelNum);
#endif
    
}
