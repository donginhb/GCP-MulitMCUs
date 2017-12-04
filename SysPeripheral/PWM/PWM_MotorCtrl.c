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
#include "PWM_MotorCtrl.h"
#include "DataType/DataType.h"
#include "HAL/HAL_IRQHandler.h"
#include "HAL/HAL_PwmCount.h"
#include "HAL/HAL_Pwm.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define MAX_PWM_NODE                        (8)             //最大的PWM输出通道

#define FREQUENCY_OVERMEASURE   (1.20)                      //频率裕量,由于系统对定时器进行配置需要时间,
                                                            //所以需要稍微提升频率以腾出配置时间

static uBit8  m_uPwmCountNode[MAX_PWM_NODE] = {0};          //输出PWM对应的PWM计数模块
static uBit32 m_ulPwmCountChannel[MAX_PWM_NODE] = {0};      //输出PWM对应的PWM计数通道


/*****************************************************************************
 * 中断回调函数接口
 ****************************************************************************/


/**
  * @brief  PWM通道0计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count0_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[0], m_ulPwmCountChannel[0]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[0], m_ulPwmCountChannel[0]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(0, false);
    }
    
}


/**
  * @brief  PWM通道1计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count1_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[1], m_ulPwmCountChannel[1]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[1], m_ulPwmCountChannel[1]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(1, false);
    }
    
}


/**
  * @brief  PWM通道2计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count2_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[2], m_ulPwmCountChannel[2]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[2], m_ulPwmCountChannel[2]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(2, false);
    }
    
}


/**
  * @brief  PWM通道3计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count3_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[3], m_ulPwmCountChannel[3]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[3], m_ulPwmCountChannel[3]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(3, false);
    }
    
}


/**
  * @brief  PWM通道4计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count4_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[4], m_ulPwmCountChannel[4]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[4], m_ulPwmCountChannel[4]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(4, false);
    }
    
}


/**
  * @brief  PWM通道4计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count5_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[5], m_ulPwmCountChannel[5]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[5], m_ulPwmCountChannel[5]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(5, false);
    }
    
}


/**
  * @brief  PWM通道4计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count6_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[6], m_ulPwmCountChannel[6]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[6], m_ulPwmCountChannel[6]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(6, false);
    }
    
}


/**
  * @brief  PWM通道4计数中断处理
  * @param  None
  * @retval None
  */
static void PWM_Count7_IRQHandler(void)
{
    //判断中断入口
    if (HAL_TIM_GetInputCountITStatus(m_uPwmCountNode[7], m_ulPwmCountChannel[7]))   
    {
        //清标志位
        HAL_TIM_ClearInputCountITStatus(m_uPwmCountNode[7], m_ulPwmCountChannel[7]);  
        
        //关闭PWM输出
        HAL_PWM_OutputEnable(7, false);
    }
    
}


/**
  * @brief  PWM通道5计数中断处理
  * @param  None
  * @retval None
  */
static void (*pf_PwmCountIrqHandler[MAX_PWM_NODE])(void) = 
{
    PWM_Count0_IRQHandler,
    PWM_Count1_IRQHandler,
    PWM_Count2_IRQHandler,
    PWM_Count3_IRQHandler,
    PWM_Count4_IRQHandler,
    PWM_Count5_IRQHandler,
    PWM_Count6_IRQHandler,
    PWM_Count7_IRQHandler,
    
};


/*****************************************************************************
 * PWM输入相关控制接口
 ****************************************************************************/


/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @param  ulOutChNum 输出通道编号
  * @param  uPwmCountNode  PWM计数节点
  * @param  ulCountChNum 计数通道编号
  * @retval 0-成功 非0-失败
  */
uBit32 PWM_MotorPulseInit(uBit8 uPwmOutputNode, uBit32 ulOutChNum, 
                        uBit8 uPwmCountNode, uBit32 ulCountChNum)
{
    //数据校验
    if (uPwmOutputNode >= MAX_PWM_NODE)
    {
        return 1;
    }
    
    //存储映射
    m_uPwmCountNode[uPwmOutputNode] = uPwmCountNode;
    m_ulPwmCountChannel[uPwmOutputNode] = ulCountChNum;
    //m_ulPwmOutputChannel[uPwmOutputNode] = ulOutChNum;
    
    //初始化PWM输出模块
    HAL_PWM_Init(uPwmOutputNode, 0x1<<ulOutChNum);
    
    //初始化PWM计数模块
    HAL_TIM_InputCountInit(uPwmCountNode, 0x1<<ulCountChNum);
    
    //使能计数中断
    HAL_TIM_EnableInputCountIRQ(uPwmCountNode, ulCountChNum, true);
    
    //设置中断回调
    HAL_IRQ_SetTrgCallback(pf_PwmCountIrqHandler[uPwmOutputNode], PWM_COUNT_IRQ_BASE+uPwmCountNode);
    
    return 0;
}


/**
  * @brief  电机脉冲PWM初始化
  * @param  uPwmOutputNode PWM节点
  * @param  ulPulseCount 脉冲数量
  * @retval 0-成功 非0-失败
  */
void PWM_SendPulse(uBit8 uPwmOutputNode, uBit32 ulPulseCount)
{
    if (ulPulseCount == 0)
    {
        return;
    }
    
    //设置最大计数
    HAL_TIM_SetMaxInputCount(m_uPwmCountNode[uPwmOutputNode], m_ulPwmCountChannel[uPwmOutputNode], ulPulseCount);
    
    //使能计数
    HAL_TIM_InputCountEnable(m_uPwmCountNode[uPwmOutputNode], true);
    
    //开启PWM
    HAL_PWM_OutputEnable(uPwmOutputNode, true);
    
}



/**
  * @brief  在1MS内产生指定数量的脉冲
  * @param  uPwmOutputNode PWM节点
  * @param  ulPulseCount 脉冲数量
  * @retval 0-成功 非0-失败
  */
void PWM_SendPulseLimitMs(uBit8 uPwmOutputNode, uBit32 ulPulseCount)
{
    //计算频率
    uBit32 ulFrequency = (uBit32)(ulPulseCount * 1000 * FREQUENCY_OVERMEASURE);
    
    //设置频率
    HAL_PWM_SetOutputPwmFrq(uPwmOutputNode, ulFrequency);
    
    //发送指定数量脉冲
    PWM_SendPulse(uPwmOutputNode, ulPulseCount);
    
}

