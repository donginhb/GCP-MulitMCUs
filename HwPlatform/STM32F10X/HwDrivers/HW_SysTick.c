/**
  ******************************************************************************
  * @file    HW_SysTick.h
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.06
  * @brief   SysTick Driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "HW_SysTick.h"
#include "HW_IRQHandler.h"
#include "chip.h"


/*****************************************************************************
 * SysTick 时间管理
 ****************************************************************************/

static uint32_t m_ulSysTickTime  = 1.0;   //系统滴答间隔(单位:MS)
static uint32_t m_ulSysTickCount = 0.0;   //系统滴答计数

/**
  * @brief  滴答时间设置
  * @param  ulTickTime 滴答时间
  * @retval None
  */
void HW_SysTick_SetTickTime(uint32_t ulTickTime)
{
    m_ulSysTickTime = ulTickTime;
  
}


/**
  * @brief  系统滴答计数更新
  * @param  None
  * @retval None
  * @note   此函数放在m_ulSysTickTime毫秒间隔的定时中断中执行
  */
void HW_SysTick_Update(void)
{
    m_ulSysTickCount += m_ulSysTickTime;

}


/**
  * @brief  系统滴答计数获取
  * @param  None
  * @retval 系统滴答计数值
  * @note   若1MS记一个数,约49.7天一个循环
  */
uint32_t HW_SysTick_GetCount(void)
{
    
    return m_ulSysTickCount;
}


/**
  * @brief  系统滴答初始化函数
  * @param  ulTickTime 中断周期(单位是毫秒)
  * @retval None
  */
void HW_SysTick_Init(uint32_t ulTickTime)
{
    //更新系统内核时钟
    SystemCoreClockUpdate();
    
    //配置系统滴答中断
    SysTick_Config(SystemCoreClock / 1000 * (uint32_t)ulTickTime);
    
    //配置软件定时器的滴答间隔(单位:MS)
    HW_SysTick_SetTickTime(ulTickTime);
    
    //设置系统滴答中断回调
    HW_IRQ_SetTrgCallback(HW_SysTick_Update, IRQ_TRG_SYSTICK_UPDATE);
 
}
