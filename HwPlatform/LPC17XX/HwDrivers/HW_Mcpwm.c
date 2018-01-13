/**
  ******************************************************************************
  * @file    HW_McPwm.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.12
  * @brief   motor control pwm drivers
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *     MCABORT --- P1[21]  FUNC1
  *     MCOA[0] --- P1[19]  FUNC1
  *     MCOB[0] --- P1[22]  FUNC1
  *     MCOA[1] --- P1[25]  FUNC1
  *     MCOB[1] --- P1[26]  FUNC1
  *     MCOA[2] --- P1[28]  FUNC1
  *     MCOB[2] --- P1[29]  FUNC1
  *     
  *     MCI[0]  --- P1[20]  FUNC1
  *     MCI[1]  --- P1[23]  FUNC1
  *     MCI[2]  --- P1[24]  FUNC1
  *     
  * 由于MCPWM的三个通道的定时器都是互相独立的,可以做到不同的通道的频率、占空比等
  * 参数互相独立,所以在接口之中,将三个通道定义为三个节点;
  * 
  * 由于MCPWM没有时钟预分频机制,输入时钟固定为PCLK,所以为了输出不同的频率而牺牲分
  * 辩率的固定性.也就是说,通过修改LIM寄存器来适配不同的频率,同时分辨率也会随之改
  * 变.频率越高,分辨率越小,反之亦然.当分辨率为2时,输出的频率为最高输出频率PCLK/2;
  * 
  * 在本应用之中,将LIM寄存器作为周期寄存器,MAT寄存器作为脉冲宽度寄存器;
  * 
  * 注: 由于影子寄存器的关系,当对LIM/MAT等寄存器进行写入操作后,随即读取寄存器的
  *     数值得不到想要的结果,导致在连续配置模块的情况下会出错.所以当对LIM和MATH
  *     寄存器进行写入操作时,会先关闭模块,让配置直接写入操作寄存器而非影子寄存器;
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_McPwm.h"
#include "chip.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define HW_MAX_MCPWM_COUNT          (3)

#define HW_MCPWM_DEF_OUT_FRE        (10*1000)       //默认输出频率
#define HW_MCPWM_DEF_OUT_DUTY_RATE  (50)            //默认输出占空比


//MCPWM输出引脚定义
static const PINMUX_GRP_T m_McpwmOutputPinMux[] = 
{
    {0x1, 19,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOA[0]
    {0x1, 22,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOB[0]
    {0x1, 25,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOA[1]
    {0x1, 26,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOB[1]
    {0x1, 28,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOA[2]
    {0x1, 29,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCOB[2]

};


//MCPWM捕获引脚定义
static const PINMUX_GRP_T m_McpwmInputPinMux[] = 
{
    {0x1, 20,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCI[0]
    {0x1, 23,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCI[1]
    {0x1, 24,  IOCON_MODE_INACT | IOCON_FUNC1},    //MCI[2]

};


/**
  * @brief  MCPWM 输出IO配置
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnableMCOB 是否使能MCOB引脚
  * @retval None
  */
static void HW_MCPWM_ConfigOutputIO(uint8_t uMcpwmNode, bool bOppositeOutput)
{
    //使能MCOA[x]
    Chip_IOCON_PinMuxSet(LPC_IOCON, m_McpwmOutputPinMux[uMcpwmNode*2].pingrp, 
                         m_McpwmOutputPinMux[uMcpwmNode*2].pinnum, 
                         m_McpwmOutputPinMux[uMcpwmNode*2].modefunc);

    if (bOppositeOutput)
    {
        //使能MCOB[x]
        Chip_IOCON_PinMuxSet(LPC_IOCON, m_McpwmOutputPinMux[uMcpwmNode*2 + 1].pingrp, 
                             m_McpwmOutputPinMux[uMcpwmNode*2 + 1].pinnum, 
                             m_McpwmOutputPinMux[uMcpwmNode*2 + 1].modefunc);
    }
    
    
}


/**
  * @brief  MCPWM 输入IO配置
  * @param  uMcpwmNode MCPWM节点
  * @retval None
  */
static void HW_MCPWM_ConfigInputIO(uint8_t uMcpwmNode)
{
    //使能MCI[x]
    Chip_IOCON_PinMuxSet(LPC_IOCON, m_McpwmInputPinMux[uMcpwmNode].pingrp, 
                         m_McpwmInputPinMux[uMcpwmNode].pinnum, 
                         m_McpwmInputPinMux[uMcpwmNode].modefunc);
    
}

#if 0

/**
  * @brief  MCPWM LIM寄存器写入
  * @param  uMcpwmNode MCPWM节点
  * @retval None
  */
static void HW_MCPWM_WriteLimitReg(uint8_t uMcpwmNode, uint32_t ulRegValue)
{
    //写入寄存器
    LPC_MCPWM->LIM[uMcpwmNode] = ulRegValue;
    
    //等待写入完成
    //while (LPC_MCPWM->LIM[uMcpwmNode] != ulRegValue);
    
}



/**
  * @brief  MCPWM MATH寄存器写入
  * @param  uMcpwmNode MCPWM节点
  * @retval None
  */
static void HW_MCPWM_WriteMatchReg(uint8_t uMcpwmNode, uint32_t ulRegValue)
{
    //写入寄存器
    LPC_MCPWM->MAT[uMcpwmNode] = ulRegValue;
    
    //等待写入完成
    //while (LPC_MCPWM->MAT[uMcpwmNode] != ulRegValue);
    
}
#endif

/*****************************************************************************
 * MCPWM 输出相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM输出模式初始化
  * @param  uMcpwmNode MCPWM节点
  * @param  bOppositeOutput 使能反相输出通道
  * @retval 0-成功 非0-失败
  * @note   调用此初始化函数不会立即使能输出,在调用使能接口才会产生输出
  */
uint32_t HW_MCPWM_InitOutput(uint8_t uMcpwmNode, bool bOppositeOutput)
{
    uint32_t ulMcpwmClockRate = 0;
    
    //参数判断
    if (uMcpwmNode >= HW_MAX_MCPWM_COUNT)
    {
        return 1;
    }

    //使能引脚
    HW_MCPWM_ConfigOutputIO(uMcpwmNode, bOppositeOutput);
    
    //开时钟
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_MCPWM);
    
    //获取外设时钟频率
    ulMcpwmClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_MCPWM);
    
    //设置输出模式
    //边沿对齐模式,正态电平,禁止死区,使能更新
    LPC_MCPWM->CON_CLR = (0x1 << (1 + uMcpwmNode*8)) | (0x1 << (2 + uMcpwmNode*8)) | 
                         (0x1 << (3 + uMcpwmNode*8)) | (0x1 << (4 + uMcpwmNode*8));  
    
    //设置计数器源
    LPC_MCPWM->CNTCON_CLR = (0x1<<(29+uMcpwmNode)); //设置为计数器模式(PLCK输入作为时钟源)
    
    //设置PWM波时序
    LPC_MCPWM->LIM[uMcpwmNode] = ulMcpwmClockRate/HW_MCPWM_DEF_OUT_FRE - 1;
    LPC_MCPWM->MAT[uMcpwmNode] = (uint32_t)((ulMcpwmClockRate/HW_MCPWM_DEF_OUT_FRE) * (1.0 - (HW_MCPWM_DEF_OUT_DUTY_RATE/100.0)));
    
    //关闭定时器
    LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    
    return 0;
}


/**
  * @brief  MCPWM输出占空比设置
  * @param  uMcpwmNode MCPWM节点
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_MCPWM_SetOutputPwmDutyRatio(uint8_t uMcpwmNode, float fDutyRatio)
{
    //获取模块使能状态
    bool bDriverIsEnable = (LPC_MCPWM->CON & (0x1 << (0 + uMcpwmNode*8))) ? true : false;
    
    //关闭定时器
    LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    
    //清空计数器
    LPC_MCPWM->TC[uMcpwmNode] = 0;
    
    //写入匹配寄存器
    uint32_t ulMatchValue = (uint32_t)((LPC_MCPWM->LIM[uMcpwmNode] + 1) * (1.0 - (fDutyRatio/100.0)));
    LPC_MCPWM->MAT[uMcpwmNode] = ulMatchValue;
    
    //还原使能状态
    if (bDriverIsEnable)
    {
        //开启定时器
        LPC_MCPWM->CON_SET = (0x1 << (0 + uMcpwmNode*8));
    }
    
}


/**
  * @brief  MCPWM输出频率设置
  * @param  uMcpwmNode MCPWM节点
  * @param  ulFrequency  PWM频率(单位:Hz)
  * @retval None
  * @note   若没有使能更新,则在MCPWM运行期间,不会改变其频率
  */
void HW_MCPWM_SetOutputPwmFrq(uint8_t uMcpwmNode, uint32_t ulFrequency)
{
    //获取模块使能状态
    bool bDriverIsEnable = (LPC_MCPWM->CON & (0x1 << (0 + uMcpwmNode*8))) ? true : false;
    
    //关闭定时器
    LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    
    //清空计数器
    LPC_MCPWM->TC[uMcpwmNode] = 0;
    
    //计算当前的占空比
    float fDutyRatio = ((float)LPC_MCPWM->MAT[uMcpwmNode] / (LPC_MCPWM->LIM[uMcpwmNode] + 1)) * 100;
    
    //获取外设时钟频率
    uint32_t ulMcpwmClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_MCPWM);
    
    //设置频率
    LPC_MCPWM->LIM[uMcpwmNode] = ulMcpwmClockRate/ulFrequency - 1;
    
    //还原占空比
    uint32_t ulMatchValue = (uint32_t)((ulMcpwmClockRate/ulFrequency) * (1.0 - (fDutyRatio/100.0)));
    LPC_MCPWM->MAT[uMcpwmNode] = ulMatchValue;
    
    //还原使能状态
    if (bDriverIsEnable)
    {
        //开启定时器
        LPC_MCPWM->CON_SET = (0x1 << (0 + uMcpwmNode*8));
    }
    
}


/**
  * @brief  MCPWM输出使能
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableOutput(uint8_t uMcpwmNode, bool bIsEnablle)
{
    if (bIsEnablle)
    {
        //启动定时器
        LPC_MCPWM->CON_SET = (0x1 << (0 + uMcpwmNode*8));
        
        //清空计数器
        LPC_MCPWM->TC[uMcpwmNode] = 0;
    }
    else 
    {
        //关闭定时器
        LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    }
    
}


/*****************************************************************************
 * MCPWM 输入计数相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM输入计数模式初始化
  * @param  uMcpwmNode MCPWM节点
  * @param  uInputSource 输入源
  * @param  uTrgSource 触发源
  *   @arg  HW_MCPWM_INPUT_TRG_R  上升沿触发
  *   @arg  HW_MCPWM_INPUT_TRG_F  下降沿触发
  *   @arg  HW_MCPWM_INPUT_TRG_R_F 双边沿触发
  * @retval 0-成功 非0-失败
  * @note   调用此初始化函数不会立即使能计数,在调用使能接口才会使能计数
  */
uint32_t HW_MCPWM_InitInputCount(uint8_t uMcpwmNode, uint8_t uInputSource, uint8_t uTrgSource)
{
    //参数判断
    if (uMcpwmNode >= HW_MAX_MCPWM_COUNT)
    {
        return 1;
    }

    //使能输入引脚
    HW_MCPWM_ConfigInputIO(uInputSource);
    
    //开时钟
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_MCPWM);
    
    //设置输出模式
    //边沿对齐模式,正态电平,禁止死区,使能更新
    LPC_MCPWM->CON_CLR = (0x1 << (1 + uMcpwmNode*8)) | (0x1 << (2 + uMcpwmNode*8)) | 
                         (0x1 << (3 + uMcpwmNode*8)) | (0x1 << (4 + uMcpwmNode*8));  
    
    //设置计数器源
    LPC_MCPWM->CNTCON_SET = (0x1<<(29+uMcpwmNode)); //设置为计数器模式(外界输入作为时钟源)
    
    //设置PWM波时序
    LPC_MCPWM->LIM[uMcpwmNode] = 0xFFFFFFFF;
    LPC_MCPWM->MAT[uMcpwmNode] = 0xFFFFFFFF;
    
    //设置触发信号
    if (uTrgSource & HW_MCPWM_INPUT_TRG_R)
    {
        LPC_MCPWM->CNTCON_SET = (0x1 << (0 + uMcpwmNode*6 + uInputSource*2));
    }
    else 
    {
        LPC_MCPWM->CNTCON_CLR = (0x1 << (0 + uMcpwmNode*6 + uInputSource*2));
    }
    
    if (uTrgSource & HW_MCPWM_INPUT_TRG_F)
    {
        LPC_MCPWM->CNTCON_SET = (0x1 << (1 + uMcpwmNode*6 + uInputSource*2));
    }
    else 
    {
        LPC_MCPWM->CNTCON_CLR = (0x1 << (1 + uMcpwmNode*6 + uInputSource*2));
    }
    
    //关闭定时器
    LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    
    return 0;
}


/**
  * @brief  MCPWM计数使能
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableInputCount(uint8_t uMcpwmNode, bool bIsEnablle)
{
    if (bIsEnablle)
    {
        //启动定时器
        LPC_MCPWM->CON_SET = (0x1 << (0 + uMcpwmNode*8));
    }
    else 
    {
        //关闭定时器
        LPC_MCPWM->CON_CLR = (0x1 << (0 + uMcpwmNode*8));
    }
    
}


/**
  * @brief  MCPWM最大输入计数设置
  * @param  uMcpwmNode MCPWM节点
  * @param  ulMaxCount 最大计数值
  * @retval None
  * @note   到达最大计数值后,计数寄存器会复位;
  */
void HW_MCPWM_SetMaxInputCount(uint8_t uMcpwmNode, uint32_t ulMaxCount)
{
    LPC_MCPWM->LIM[uMcpwmNode] = ulMaxCount - 1;
    
}


/**
  * @brief  MCPWM当前计数值获取
  * @param  uMcpwmNode MCPWM节点
  * @retval 当前计数值
  */
uint32_t HW_MCPWM_GetCurInputCount(uint8_t uMcpwmNode)
{
    
    return LPC_MCPWM->TC[uMcpwmNode];
}


/*****************************************************************************
 * MCPWM 中断相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM计数使能
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @param  bIsEnablle 使能位
  * @retval None
  */
void HW_MCPWM_EnableIRQ(uint8_t uMcpwmNode, uint8_t uIntModeMask, bool bIsEnable)
{
    if (bIsEnable)
    {
        //开外设中断
        LPC_MCPWM->INTEN_SET = uIntModeMask << (uMcpwmNode*4);
        
        //开内核中断
        NVIC_EnableIRQ(MCPWM_IRQn);
    }
    else 
    {
        //关外设中断
        LPC_MCPWM->INTEN_CLR = uIntModeMask << (uMcpwmNode*4);
    }
    
}


/**
  * @brief  MCPWM中断状态获取
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntMode 中断模式(仅能获取一种中断的标志)
  * @retval None
  */
bool HW_MCPWM_GetITStatus(uint8_t uMcpwmNode, uint8_t uIntMode)
{
    
    return (LPC_MCPWM->INTF & (uIntMode << (uMcpwmNode*4))) ? true : false;
}


/**
  * @brief  MCPWM中断状态清除
  * @param  uMcpwmNode MCPWM节点
  * @param  uIntModeMask 中断模式掩码(可以多种模式'与'后入参)
  * @retval None
  */
void HW_MCPWM_ClearITStatus(uint8_t uMcpwmNode, uint8_t uIntModeMask)
{
    LPC_MCPWM->INTF_CLR = (uIntModeMask << (uMcpwmNode*4));
    
}

