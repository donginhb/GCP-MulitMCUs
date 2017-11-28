/**
  ******************************************************************************
  * @file    SysCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.10
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysCtrl.h"
#include "HAL/HAL_SysCtrl.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
static SYS_TIME_DATA m_ResetTimer = {0};   //复位管理定时器


/*****************************************************************************
 * 系统复位相关接口函数
 ****************************************************************************/

/**
  * @brief  立即复位
  * @param  None
  * @retval None
  */
void SysCtrl_ResetSystemNow(void)
{
    HAL_SystemReset();
    
}


/**
  * @brief  延时复位
  * @param  ulDelayMs 延时的时间(单位:MS)
  * @retval None
  */
void SysCtrl_ResetSystemDelay(uBit32 ulDelayMs)
{
    SysTime_Start(&m_ResetTimer, ulDelayMs);
    
}


/**
  * @brief  系统复位处理(放在while循环中处理)
  * @param  None
  * @retval None
  */
void SysCtrl_ResetHandler(void)
{
    if (SysTime_CheckExpiredState(&m_ResetTimer))
    {
        SysCtrl_ResetSystemNow();
    }
    
}
