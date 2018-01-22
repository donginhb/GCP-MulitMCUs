/**
  ******************************************************************************
  * @file    HW_Tim.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   Time output drivers
  ******************************************************************************
  * @attention
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
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Tim.h"
#include "chip.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define TIM_GET_PR_BY_CNT_FRE(CntFre)            (Chip_TIME_GetClock(TIM[uTimeNode])/(CntFre) - 1)   //通过计数频率计算预分频值
#define TIM_GET_PR_BY_OP_FRE(OutFre, AutoLoad)   (TIM_GET_PR_BY_CNT_FRE((OutFre) * (AutoLoad))) //通过输出频率计算预分频值(计数频率=输出频率*自动重装载值)

//定时器数量定义
#define HW_TIME_COUNT               (4)             //定时器数量

#define HW_TIME_CH_COUNT            (4)             //通道计数
#define HW_TIME_CH_MASK(n)          ((n)&0xF)       //通道掩码
#define HW_TIME_DEFAULT_COUNT_RANG  (2)             //默认定时器计数范围

//串口相关变量定义
static LPC_TIMER_T * const TIM[HW_TIME_COUNT] = {LPC_TIMER0, LPC_TIMER1, LPC_TIMER2, LPC_TIMER3};
static const IRQn_Type TIM_IRQn[HW_TIME_COUNT] = {TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn};   //中断向量


//PWM1 IO 引脚定义
static const PINMUX_GRP_T m_TimeOutputPinMux[HW_TIME_COUNT][HW_TIME_CH_COUNT] = 
{
    //TIME0
    {
        {0x01, 28,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x01, 29,  IOCON_MODE_INACT | IOCON_FUNC3},
    },
    
    //TIME1
    {
        {0x01, 22,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x01, 25,  IOCON_MODE_INACT | IOCON_FUNC2},
    },
    
    //TIME2
    {
        {0x04, 28,  IOCON_MODE_INACT | IOCON_FUNC2},
        {0x04, 29,  IOCON_MODE_INACT | IOCON_FUNC2},
        {0x00,  8,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x00,  9,  IOCON_MODE_INACT | IOCON_FUNC3},
    },
    
    //TIME3
    {
        {0x00, 10,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x00, 11,  IOCON_MODE_INACT | IOCON_FUNC3},
    },

};


//PWM1 IO 引脚定义
static const PINMUX_GRP_T m_TimeInputPinMux[HW_TIME_COUNT][HW_TIME_CH_COUNT] = 
{
    //TIME0
    {
        {0x01, 26,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x01, 27,  IOCON_MODE_INACT | IOCON_FUNC3},
    },
    
    //TIME1
    {
        {0x01, 18,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x01, 19,  IOCON_MODE_INACT | IOCON_FUNC3},
    },
    
    //TIME2
    {
        {0x00,  4,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x00,  5,  IOCON_MODE_INACT | IOCON_FUNC3},
    },
    
    //TIME3
    {
        {0x00, 23,  IOCON_MODE_INACT | IOCON_FUNC3},
        {0x00, 24,  IOCON_MODE_INACT | IOCON_FUNC3},
    },

};


/**
  * @brief  定时器输出IO配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @retval None
  */
static void HW_TIM_ConfigOutputIO(uint8_t uTimeNode, uint8_t uChannelMask)
{
    for (int i = 0; i < HW_TIME_CH_COUNT; i++)
    {
        if (HW_TIME_CH_MASK(uChannelMask) & (0x1<<i))
        {
            Chip_IOCON_PinMuxSet(LPC_IOCON, m_TimeOutputPinMux[uTimeNode][i].pingrp, m_TimeOutputPinMux[uTimeNode][i].pinnum, m_TimeOutputPinMux[uTimeNode][i].modefunc);
        }
    }

}


/**
  * @brief  定时器输入IO配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @retval None
  */
static void HW_TIM_ConfigInputIO(uint8_t uTimeNode, uint8_t uChannelMask)
{
    for (int i = 0; i < HW_TIME_CH_COUNT; i++)
    {
        if (HW_TIME_CH_MASK(uChannelMask) & (0x1<<i))
        {
            Chip_IOCON_PinMuxSet(LPC_IOCON, m_TimeInputPinMux[uTimeNode][i].pingrp, m_TimeInputPinMux[uTimeNode][i].pinnum, m_TimeInputPinMux[uTimeNode][i].modefunc);
        }
    }

}


/**
  * @brief  定时器外设时钟获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器时钟
  */
static uint32_t HW_TIM_GetPeripheralClock(uint8_t uTimeNode)
{
    uint32_t ulTimeClock = 0;
    
    switch (uTimeNode)
    {
    case HW_TIME_NODE_0: ulTimeClock = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER0); break;
    case HW_TIME_NODE_1: ulTimeClock = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER1); break;
    case HW_TIME_NODE_2: ulTimeClock = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER2); break;
    case HW_TIME_NODE_3: ulTimeClock = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER3); break;
    default: break; //不应该到这里
    }
    
    return ulTimeClock;
}


/*****************************************************************************
 * 定时器输出相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输出模式配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uOutputCfgCode 工作模式
  * @retval None
  */
static void HW_TIM_ConfigOutputMode(uint8_t uTimeNode, uint8_t uChannelMask)
{
    //初始化模块
    Chip_TIMER_Init(TIM[uTimeNode]);
    
    //复位计数器
    Chip_TIMER_Reset(TIM[uTimeNode]);
    
    //设置PWM模式(计时)
    Chip_TIMER_TIMER_SetCountClockSrc(TIM[uTimeNode], TIMER_CAPSRC_RISING_PCLK, 0);
    
    //设置PWM触发操作(复位计数)
    Chip_TIMER_ResetOnMatchEnable(TIM[uTimeNode], 3);
    
    //设置PWM时序
    Chip_TIMER_SetMatch(TIM[uTimeNode], 0, HW_TIME_DEFAULT_COUNT_RANG-1);
    Chip_TIMER_SetMatch(TIM[uTimeNode], 1, HW_TIME_DEFAULT_COUNT_RANG-1);
    Chip_TIMER_SetMatch(TIM[uTimeNode], 3, HW_TIME_DEFAULT_COUNT_RANG-1);
    
    //外部引脚状态设置(翻转)
    Chip_TIMER_ExtMatchControlSet(TIM[uTimeNode], 0, TIMER_EXTMATCH_TOGGLE, 0);
    Chip_TIMER_ExtMatchControlSet(TIM[uTimeNode], 0, TIMER_EXTMATCH_TOGGLE, 1);
    
}


/**
  * @brief  定时器输出模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uOutputCfgCode 工作模式
  * @retval None
  * @note   初始化后,默认已经打开定时器通道,但是尚未启动定时器
  */
void HW_TIM_InitOutput(uint8_t uTimeNode, uint8_t uChannelMask)
{
    //引脚配置
    HW_TIM_ConfigOutputIO(uTimeNode, uChannelMask);
    
    //模式配置
    HW_TIM_ConfigOutputMode(uTimeNode, uChannelMask);
    
    //时序配置
    HW_TIM_SetOutputPwmDutyRatio(uTimeNode, uChannelMask, 50);
    HW_TIM_SetOutputPwmFrq(uTimeNode, 1000);
    
    //开启定时器
    //HW_TIM_OutputEnable(uTimeNode, 1);
    
}


/**
  * @brief  定时器通道使能设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HW_TIM_EnableChannel(uint8_t uTimeNode, uint8_t uChannelMask, bool bIsEnble)
{
    if (bIsEnble)
    {
        //使能定时器通道
        HW_TIM_SetOutputPwmDutyRatio(uTimeNode, uChannelMask, 50);
    }
    else 
    {
        //禁止定时器通道
        HW_TIM_SetOutputPwmDutyRatio(uTimeNode, uChannelMask, 0);
    }
    
}


/**
  * @brief  定时器输出中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableOutputIRQ(uint8_t uTimeNode, uint8_t uChannelMask, bool bIsEnable)
{
    if (bIsEnable)
    {
        NVIC_DisableIRQ(TIM_IRQn[uTimeNode]);
        NVIC_SetPriority(TIM_IRQn[uTimeNode], ((0x01 << 3) | 0x01));
        NVIC_EnableIRQ(TIM_IRQn[uTimeNode]);
    }
    else 
    {
        NVIC_DisableIRQ(TIM_IRQn[uTimeNode]);
    }
    
}


/**
  * @brief  定时器输出PWM占空比设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 定时器通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_TIM_SetOutputPwmDutyRatio(uint8_t uTimeNode, uint8_t uChannelMask, float fDutyRatio)
{
    uint32_t ulCmpVal = TIM[uTimeNode]->MR[3];
    uint16_t nRegValue = (uint16_t)((fDutyRatio * ulCmpVal) / 100.0);
    
    for (int i = 0; i < HW_TIME_CH_COUNT; i++)
    {
        if (HW_TIME_CH_MASK(uChannelMask) & (0x01<<i))
        {
            Chip_TIMER_SetMatch(TIM[uTimeNode], i, nRegValue);
        }
    }
    
}


/**
  * @brief  定时器输出比较值设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @param  nCompareVal 比较值
  * @retval None
  */
void HW_TIM_SetOutputCmpVal(uint8_t uTimeNode, uint8_t uChannelMask, uint16_t nCompareVal)
{
    for (int i = 0; i < HW_TIME_CH_COUNT; i++)
    {
        if (HW_TIME_CH_MASK(uChannelMask) & (0x01<<i))
        {
            Chip_TIMER_SetMatch(TIM[uTimeNode], i, nCompareVal);
        }
    }
    
}


/**
  * @brief  定时器输出PWM频率设置
  * @param  uTimeNode 定时器节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HW_TIM_SetOutputPwmFrq(uint8_t uTimeNode, uint32_t ulFrequency)
{
    uint32_t ulTimeClock = HW_TIM_GetPeripheralClock(uTimeNode);
    uint32_t ulPrescale = ((ulTimeClock / (ulFrequency*HW_TIME_DEFAULT_COUNT_RANG*2)))-1;
    Chip_TIMER_PrescaleSet(TIM[uTimeNode], ulPrescale);
        
}


/**
  * @brief  定时器输出自动重加载值设置
  * @param  nReloadVal 重加载值
  * @retval None
  */
void HW_TIM_SetOutputAutoReloadValue(uint8_t uTimeNode, uint16_t nReloadVal)
{
    Chip_TIMER_SetMatch(TIM[uTimeNode], 3, nReloadVal);
    
}


/**
  * @brief  定时器输出自动重加载值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器中的重加载值
  */
uint16_t HW_TIM_GetOutputAutoReloadValue(uint8_t uTimeNode)
{
    
    return TIM[uTimeNode]->MR[3];
}


/**
  * @brief  定时器输出使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 定时器使能位
  * @retval None
  */
void HW_TIM_OutputEnable(uint8_t uTimeNode, bool bIsEnable)
{
    if (bIsEnable)
    {
        Chip_TIMER_Enable(TIM[uTimeNode]);
    }
    else 
    {
        Chip_TIMER_Disable(TIM[uTimeNode]);
    }
    
}


/*****************************************************************************
 * 定时器输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @retval None
  */
static void HW_TIM_ConfigInputCountMode(uint8_t uTimeNode, uint8_t uChannelNum)
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
  * @brief  定时器输入计数初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HW_TIM_InitInputCount(uint8_t uTimeNode, uint8_t uChannelMask)
{
    //引脚配置
    HW_TIM_ConfigInputIO(uTimeNode, uChannelMask);
    
    //模式配置
    uint8_t uChannelNum = (uChannelMask & 0x1) ? 0 : 1;
    HW_TIM_ConfigInputCountMode(uTimeNode, uChannelNum);
    
}


/**
  * @brief  定时器计数使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableInputCount(uint8_t uTimeNode, bool bIsEnable)
{
    if (bIsEnable)
    {
        //启动定时器
        Chip_TIMER_Enable(TIM[uTimeNode]);
    }
    else 
    {
        //关闭定时器
        Chip_TIMER_Disable(TIM[uTimeNode]);
    }
    
}


/**
  * @brief  定时器最大输入计数设置
  * @param  uTimeNode 定时器节点
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位;若设置多个通道的"最大计数值",以最小的计数值为准;
  */
void HW_TIM_SetMaxInputCount(uint8_t uTimeNode, uint8_t uChannelNum, uint32_t ulMaxCount)
{
    //设置最大计数值
    Chip_TIMER_ResetOnMatchEnable(TIM[uTimeNode], uChannelNum);     //到达最大值时复位
    Chip_TIMER_SetMatch(TIM[uTimeNode], uChannelNum, ulMaxCount);   //设置最大计数值
    
}


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 当前计数值
  */
uint32_t HW_TIM_GetCurInputCount(uint8_t uTimeNode)
{
    
    return Chip_TIMER_ReadCount(TIM[uTimeNode]);
}


/*****************************************************************************
 * 定时器输入中断相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM计数使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelNum 通道编号(从0算起)
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableIRQ(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntModeMask, bool bIsEnable)
{
    if (bIsEnable)
    {
        //开外设中断
        if (uIntModeMask & HW_TIM_INT_MATCH)
        {
            Chip_TIMER_MatchEnableInt(TIM[uTimeNode], uChannelNum);
        }
        if (uIntModeMask & HW_TIM_INT_CAPTURE)
        {
            Chip_TIMER_CaptureEnableInt(TIM[uTimeNode], uChannelNum);
        }
        
        //开内核中断
        NVIC_EnableIRQ(MCPWM_IRQn);
    }
    else 
    {
        //关外设中断
        if (uIntModeMask & HW_TIM_INT_MATCH)
        {
            Chip_TIMER_MatchDisableInt(TIM[uTimeNode], uChannelNum);
        }
        if (uIntModeMask & HW_TIM_INT_CAPTURE)
        {
            Chip_TIMER_CaptureDisableInt(TIM[uTimeNode], uChannelNum);
        }
    }
    
}


/**
  * @brief  MCPWM中断状态获取
  * @param  uTimeNode 定时器节点
  * @param  uIntMode 中断模式(仅能获取一种中断的标志)
  * @retval None
  */
bool HW_TIM_GetITStatus(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntMode)
{
    bool bITStatus = false;
    
    if (uIntMode & HW_TIM_INT_MATCH)
    {
        bITStatus = Chip_TIMER_MatchPending(TIM[uTimeNode], uChannelNum);
    }
    else if (uIntMode & HW_TIM_INT_CAPTURE)
    {
        bITStatus = Chip_TIMER_CapturePending(TIM[uTimeNode], uChannelNum);
    }
    
    return bITStatus;
}


/**
  * @brief  MCPWM中断状态清除
  * @param  uTimeNode 定时器节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @retval None
  */
void HW_TIM_ClearITStatus(uint8_t uTimeNode, uint8_t uChannelNum, uint8_t uIntModeMask)
{
    if (uIntModeMask & HW_TIM_INT_MATCH)
    {
        Chip_TIMER_ClearMatch(TIM[uTimeNode], uChannelNum);
    }
    if (uIntModeMask & HW_TIM_INT_CAPTURE)
    {
        Chip_TIMER_ClearCapture(TIM[uTimeNode], uChannelNum);
    }
}

