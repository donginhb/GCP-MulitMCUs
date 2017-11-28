/**
  ******************************************************************************
  * @file    HW_Watchdog.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.07.21
  * @brief   C_CAN drivers
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *     CAN1_TD  --- P0_1(Fun1),  P0_22(Fun3),
  *     CAN1_RD  --- P0_0(Fun1),  P0_21(Fun3),
  *                  
  *     CAN2_TD  --- P0_5(Fun2),  P2_8(Fun1),
  *     CAN2_RD  --- P0_4(Fun2),  P2_7(Fun1),
  * 
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Watchdog.h"
#include "chip.h"


/*****************************************************************************
 * WatchDog 基本功能接口函数
 ****************************************************************************/


/**
  * @brief  看门狗启动
  * @param  ulTimeOutMs 超时时间
  * @retval None
  */
void HW_WWDT_Start(uint32_t ulTimeOutMs)
{
    uint32_t wdtFreq = 0;
    
    //初始化看门狗
    Chip_WWDT_Init(LPC_WWDT);
    
    //设置时钟
#if defined(WATCHDOG_CLKSEL_SUPPORT)
    Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_WATCHDOG_PCLK);
    wdtFreq = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_WDT) / 4;
#else
    wdtFreq = WDT_OSC;
#endif
    
    //设置超时时间
    Chip_WWDT_SetTimeOut(LPC_WWDT, wdtFreq * ulTimeOutMs / 1000);
    
    //设置看门狗模式:超时复位
    Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);

    //清除标志位
    Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);

    //启动看门狗
    Chip_WWDT_Start(LPC_WWDT);
    
}


/**
  * @brief  喂狗
  * @param  None
  * @retval None
  */
void HW_WWDT_Feed(void)
{
    Chip_WWDT_Feed(LPC_WWDT);
    
}

