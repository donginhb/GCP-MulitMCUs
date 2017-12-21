/**
  ******************************************************************************
  * @file    HW_TimeInput.c
  * @author  Duhanfeng
  * @version V4.2 寄存器版本
  * @date    2017.12.04
  * @brief   TIM input drivers
  ******************************************************************************
  * @attention
  *
  *
  * 引脚,
  *     CAP0[0] --- P1[26]  FUNC3
  *     CAP0[1] --- P1[27]  FUNC3
  *     CAP1[0] --- P1[18]  FUNC3
  *     CAP1[1] --- P1[19]  FUNC3
  *     CAP2[0] --- P0[ 4]  FUNC3
  *     CAP2[1] --- P0[ 5]  FUNC3
  *     CAP3[0] --- P0[23]  FUNC3
  *     CAP3[1] --- P0[24]  FUNC3
  *             --- 
  *     MAT0[0] --- P1[28]  FUNC3
  *     MAT0[1] --- P1[29]  FUNC3
  *     MAT1[0] --- P1[22]  FUNC3
  *     MAT1[1] --- P1[25]  FUNC2
  *     MAT2[0] --- P4[28]  FUNC2
  *     MAT2[1] --- P4[29]  FUNC2
  *     MAT2[2] --- P0[ 8]  FUNC3
  *     MAT2[3] --- P0[ 9]  FUNC3
  *     MAT3[0] --- P0[10]  FUNC3
  *     MAT3[1] --- P0[11]  FUNC3
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "HW_TimeInput.h"
#include "chip.h"



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//定时器数量定义
#define HW_TIME_COUNT               (4)             //定时器数量定义

#define HW_TIME_CH_COUNT            (2)             //通道计数
#define HW_TIME_CH_MASK(n)          ((n)&0x3)       //通道掩码

//输入计数功能配置
static void HW_TIM_InputCountConfig(uint8_t uTimeNode, uint8_t uChannelNum);


//串口相关变量定义
static LPC_TIMER_T * const TIM[HW_TIME_COUNT] = {LPC_TIMER0, LPC_TIMER1, LPC_TIMER2, LPC_TIMER3};
static const IRQn_Type TIM_IRQn[HW_TIME_COUNT] = {TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn};   //中断向量

//PWM1 IO 引脚定义
static const PINMUX_GRP_T m_TimePinMux[HW_TIME_COUNT][HW_TIME_CH_COUNT] = 
{
    0
//    //TIME0
//    {
//        {0x01, 26,  IOCON_MODE_INACT | IOCON_FUNC3},
//        {0x01, 27,  IOCON_MODE_INACT | IOCON_FUNC3},
//    },
//    
//    //TIME1
//    {
//        {0x01, 18,  IOCON_MODE_INACT | IOCON_FUNC3},
//        {0x01, 19,  IOCON_MODE_INACT | IOCON_FUNC3},
//    },
//    
//    //TIME2
//    {
//        {0x00,  4,  IOCON_MODE_INACT | IOCON_FUNC3},
//        {0x00,  5,  IOCON_MODE_INACT | IOCON_FUNC3},
//    },
//    
//    //TIME3
//    {
//        {0x00, 23,  IOCON_MODE_INACT | IOCON_FUNC3},
//        {0x00, 24,  IOCON_MODE_INACT | IOCON_FUNC3},
//    },

};


/**
  * @brief  定时器引脚初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @retval None
  */
static void HW_TIM_PortConfig(uint8_t uTimeNode, uint8_t uChannelMask)
{
    for (int i = 0; i < HW_TIME_CH_COUNT; i++)
    {
        if (HW_TIME_CH_MASK(uChannelMask) & (0x1<<i))
        {
//            Chip_IOCON_PinMuxSet(LPC_IOCON, m_TimePinMux[uTimeNode][i].pingrp, m_TimePinMux[uTimeNode][i].pinnum, m_TimePinMux[uTimeNode][i].modefunc);
        }
    }

}


/*****************************************************************************
 * 定时器输入相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式
  *   @arg HW_TIME_INPUT_COUNT_MODE   输入计数模式
  *   @arg HW_TIME_INPUT_CAPTURE_MODE 输入捕获模式
  * @retval None
  */
void HW_TIM_InputInit(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uInputMode)
{
    //引脚配置
    HW_TIM_PortConfig(uTimeNode, uChannelMask);
    
    //模式配置
    if (uInputMode == HW_TIME_INPUT_COUNT_MODE)
    {
        uint8_t uChannelNum = (uChannelMask & 0x1) ? 0 : 1;
        HW_TIM_InputCountConfig(uTimeNode, uChannelNum);
    }
    
}


/**
  * @brief  定时器使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnablle 定时器使能位
  * @retval None
  */
void HW_TIM_InputEnable(uint8_t uTimeNode, bool bIsEnablle)
{
    if (bIsEnablle)
    {
        Chip_TIMER_Enable(TIM[uTimeNode]);
    }
    else 
    {
        Chip_TIMER_Disable(TIM[uTimeNode]);
    }
    
}


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uint16_t HW_TIM_GetInputCount(uint8_t uTimeNode)
{
    
    return Chip_TIMER_ReadCount(TIM[uTimeNode]);
}


/*****************************************************************************
 * 定时器输入捕获模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 定时器通道捕获寄存器值
  */
uint16_t HW_TIM_GetInputCapValue(uint8_t uTimeNode, uint8_t uChannelNum)
{
    uint16_t nCapValue = 0;
    
    return nCapValue;
}


/*****************************************************************************
 * 定时器输入计数模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
static void HW_TIM_InputCountConfig(uint8_t uTimeNode, uint8_t uChannelNum)
{
    //初始化模块
    Chip_TIMER_Init(TIM[uTimeNode]);
    
    //复位计数器
    Chip_TIMER_Reset(TIM[uTimeNode]);
    
    //设置PWM模式(计时)
    Chip_TIMER_TIMER_SetCountClockSrc(TIM[uTimeNode], TIMER_CAPSRC_FALLING_CAPN, uChannelNum);
    
    //复位PWM触发操作
    TIM[uTimeNode]->MCR = 0;
    
}


/**
  * @brief  定时器最大输入计数设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位,若设置多个通道的"最大计数值",以最
  *         小的计数值为准;计数寄存器复位会导致
  */
void HW_TIM_SetMaxInputCount(uint8_t uTimeNode, uint8_t uChannelNum, uint32_t ulMaxCount)
{
    //设置最大计数值
    Chip_TIMER_ResetOnMatchEnable(TIM[uTimeNode], uChannelNum);     //到达最大值时复位
    Chip_TIMER_SetMatch(TIM[uTimeNode], uChannelNum, ulMaxCount);   //设置最大计数值
    
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
void HW_TIM_EnableInputIRQ(uint8_t uTimeNode, uint8_t uChannelNum, bool bIsEnable)
{
    if (bIsEnable)
    {
        //开外设中断
        Chip_TIMER_MatchEnableInt(TIM[uTimeNode], uChannelNum);
        
        //开内核中断
        NVIC_EnableIRQ(TIM_IRQn[uTimeNode]);
    }
    else 
    {
        NVIC_DisableIRQ(TIM_IRQn[uTimeNode]);
    }
    
}


/**
  * @brief  定时器输入中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval 中断挂起状态(true-挂起 flase-未挂起)
  */
bool HW_TIM_GetInputITStatus(uint8_t uTimeNode, uint8_t uChannelNum)
{
    
    return Chip_TIMER_MatchPending(TIM[uTimeNode], uChannelNum);
}


/**
  * @brief  定时器输入中断标志清除
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
void HW_TIM_ClearInputITStatus(uint8_t uTimeNode, uint8_t uChannelNum)
{
    Chip_TIMER_ClearMatch(TIM[uTimeNode], uChannelNum);
    
}

