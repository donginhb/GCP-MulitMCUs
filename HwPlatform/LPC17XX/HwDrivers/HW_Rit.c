/**
  ******************************************************************************
  * @file    HW_TimeDelay.c
  * @author  Duhanfneg
  * @version V1.0 
  * @date    2017.08.17
  * @brief   UART delay drivers
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Rit.h"
#include "chip.h"

static uint32_t m_ulRitClockRate = 0;   //RIT模块的时钟频率


/*****************************************************************************
 * RIT延时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器初始化
  * @param  None
  * @retval None
  */
void HW_RIT_InitDelayMode(void)
{
    //初始化外设
    Chip_RIT_Init(LPC_RITIMER);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //使能计数清零
    Chip_RIT_EnableCTRL(LPC_RITIMER, RIT_CTRL_ENCLR);
    
    //获取RIT时钟频率(1us的计数间隔)
    m_ulRitClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_RIT)/(1000000);
  
}


/**
  * @brief  us级延时
  * @param  ulUs 要延时的us数,取值范围为: 0xFFFF FFFF/0x18 ==> 约178S
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_RIT_DelayUs(uint32_t ulUs)
{
    if (!ulUs) return;
    
    //设置比较值
    Chip_RIT_SetCOMPVAL(LPC_RITIMER, m_ulRitClockRate * ulUs);
    
    //使能定时器
    Chip_RIT_Enable(LPC_RITIMER);
    
    //等待计时完成
    while (Chip_RIT_GetIntStatus(LPC_RITIMER) != SET);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //清空比较器
    LPC_RITIMER->COUNTER = 0;
    
}


/**
  * @brief  ms级延时
  * @param  ulMs 要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_RIT_DelayMs(uint32_t ulMs)
{
    if (!ulMs) return;
    
    //设置比较值
    Chip_RIT_SetCOMPVAL(LPC_RITIMER, m_ulRitClockRate * 1000 * ulMs);
    
    //使能定时器
    Chip_RIT_Enable(LPC_RITIMER);
    
    //等待计时完成
    while (Chip_RIT_GetIntStatus(LPC_RITIMER) != SET);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //清空比较器
    LPC_RITIMER->COUNTER = 0;
  
}


/*****************************************************************************
 * RIT计时模式相关控制接口
 ****************************************************************************/

/**
  * @brief  计时模式初始化
  * @param  None
  * @retval None
  */
void HW_RIT_InitReckonMode(void)
{
    //初始化外设
    Chip_RIT_Init(LPC_RITIMER);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //使能计数清零
    Chip_RIT_EnableCTRL(LPC_RITIMER, RIT_CTRL_ENCLR);
    
    //获取RIT时钟频率(1us的计数间隔)
    m_ulRitClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_RIT)/(1000000);
    
    //设置比较值(最大)
    Chip_RIT_SetCOMPVAL(LPC_RITIMER, 0xFFFFFFFF);
    
}


/**
  * @brief  计时开始
  * @param  None
  * @retval None
  */
void HW_RIT_StartReckon(void)
{
    //使能定时器
    Chip_RIT_Enable(LPC_RITIMER);
    
}


/**
  * @brief  计时停止
  * @param  None
  * @retval None
  */
void HW_RIT_StopReckon(void)
{
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
}


/**
  * @brief  计时值获取
  * @param  None
  * @retval 计时值(单位: us)
  */
uint32_t HW_RIT_GetReckonValue(void)
{
    //获取计数值
    uint32_t ulReckonValue = Chip_RIT_GetCounter(LPC_RITIMER) / m_ulRitClockRate;
    
    return ulReckonValue;
}


/**
  * @brief  计时值清零
  * @param  None
  * @retval None
  */
void HW_RIT_ClearRecvkonValue(void)
{
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //清空计数器
    LPC_RITIMER->COUNTER = 0;
    
}
