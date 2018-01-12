/**
  ******************************************************************************
  * @file    HW_McPwm.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
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
  * 
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
#define HW_MCPWM_DEF_OUT_DUTY_RATE  (50)


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
static void HW_MCPWM_ConfigOutputIO(uint8_t uMcpwmNode, bool bIsEnableMCOB)
{
    //使能MCOA[x]
    Chip_IOCON_PinMuxSet(LPC_IOCON, m_McpwmOutputPinMux[uMcpwmNode*2].pingrp, 
                         m_McpwmOutputPinMux[uMcpwmNode*2].pinnum, 
                         m_McpwmOutputPinMux[uMcpwmNode*2].modefunc);

    if (bIsEnableMCOB)
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


/*****************************************************************************
 * MCPWM 输出相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM初始化
  * @param  uMcpwmNode MCPWM节点
  * @param  bIsEnableMCOB 是否使能MCOB引脚
  * @retval None
  */
void HW_MCPWM_InitOutput(uint8_t uMcpwmNode, bool bIsEnableMCOB)
{
    uint32_t ulMcpwmClockRate = 0;
    
    //参数判断
    if (uMcpwmNode >= HW_MAX_MCPWM_COUNT)
    {
        return 1;
    }

    //使能引脚
    HW_MCPWM_ConfigOutputIO(uMcpwmNode, bIsEnableMCOB);
    
    //开时钟
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_MCPWM);
    
    //获取外设时钟频率
    ulMcpwmClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_MCPWM);
    
    //设置输出模式
    //边沿对齐模式,正态电平,禁止死区,使能更新
    LPC_MCPWM->CON_CLR = (0x1 << (1 + uMcpwmNode*8)) | (0x1 << (2 + uMcpwmNode*8)) | 
                         (0x1 << (3 + uMcpwmNode*8)) | (0x1 << (4 + uMcpwmNode*8));  
    
    //设置计数器源
    LPC_MCPWM->CNTCON_CLR = (0x1<<(29+uMcpwmNode)); //设置PLCK作为时钟源
    
    //设置PWM波时序
    LPC_MCPWM->LIM[uMcpwmNode] = ulMcpwmClockRate/HW_MCPWM_DEF_OUT_FRE - 1;
    LPC_MCPWM->MAT[uMcpwmNode] = (uint32_t)(ulMcpwmClockRate/HW_MCPWM_DEF_OUT_FRE * ((float)HW_MCPWM_DEF_OUT_DUTY_RATE / 100));
    
    //启动定时器
    LPC_MCPWM->CON_SET = (0x1 << (0 + uMcpwmNode*8));
    
}


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HAL_PWM_SetOutputPwmDutyRatio(uint8_t uPwmNode, uint8_t uChannelMask, float fDutyRatio)
{

    
}


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HAL_PWM_SetOutputPwmFrq(uint8_t uPwmNode, uBit32 ulFrequency)
{

    
}


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void HAL_PWM_OutputEnable(uint8_t uPwmNode, bool bIsEnablle)
{

    
}



