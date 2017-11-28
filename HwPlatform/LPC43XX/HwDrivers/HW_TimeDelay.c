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
#include "HW_TimeDelay.h"
#include "chip.h"

static uint32_t s_RitClockRate = 0;


/*****************************************************************************
 * 延时功能定时器接口函数
 ****************************************************************************/


/**
  * @brief  定时器初始化
  * @param  None
  * @retval None
  */
void HW_TIM_DelayInit(void)
{
    //初始化外设
    Chip_RIT_Init(LPC_RITIMER);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //使能计数清零
    Chip_RIT_EnableCTRL(LPC_RITIMER, RIT_CTRL_ENCLR);
    
    //获取RIT时钟频率(1us的计数值)
    //s_RitClockRate = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_RIT)/(1000000);
  
}


/**
  * @brief  us级延时
  * @param  要延时的us数,取值范围为: 0xFFFF FFFF/0x18 ==> 约178S
  * @retval None
  * @note   调用本函数所延时的时间,会比预期的时间多3~4us,
  *         因为软件上的运算,寄存器读写等步骤占用了一定的时间
  */
void HW_TIM_DelayUs(uint32_t nus)
{
    if (!nus) return;
    
    //设置比较值
    Chip_RIT_SetCOMPVAL(LPC_RITIMER, s_RitClockRate * nus);
    
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
  * @param  要延时的ms数,取值范围为: 0xFFFF FFFF/24000
  * @retval None
  */
void HW_TIM_DelayMs(uint32_t nms)
{
    if (!nms) return;
    
    //设置比较值
    Chip_RIT_SetCOMPVAL(LPC_RITIMER, s_RitClockRate * 1000 * nms);
    
    //使能定时器
    Chip_RIT_Enable(LPC_RITIMER);
    
    //等待计时完成
    while (Chip_RIT_GetIntStatus(LPC_RITIMER) != SET);
    
    //关闭定时器
    Chip_RIT_Disable(LPC_RITIMER);
    
    //清空比较器
    LPC_RITIMER->COUNTER = 0;
  
}
