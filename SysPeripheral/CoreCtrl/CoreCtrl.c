/**
  ******************************************************************************
  * @file    CoreCtrl.c
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
#include "CoreCtrl.h"
#include "HAL/HAL_SysCtrl.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
static SYS_TIME_DATA m_ResetTimer = {0};    //复位管理定时器
static bool bResetState = false;            //复位标志

/*****************************************************************************
 * 系统复位相关接口函数
 ****************************************************************************/

/**
  * @brief  立即复位
  * @param  None
  * @retval None
  */
void CoreCtrl_ResetSystemNow(void)
{
    HAL_SystemReset();
    
}


/**
  * @brief  延时复位
  * @param  ulDelayMs 延时的时间(单位:MS)
  * @retval None
  */
void CoreCtrl_ResetSystemDelay(uBit32 ulDelayMs)
{
    SysTime_Start(&m_ResetTimer, ulDelayMs);
    bResetState = true;
    
}


/**
  * @brief  系统复位状态获取
  * @param  None
  * @retval true-正在复位(延时执行中) false-正常
  */
bool CoreCtrl_GetResetState(void)
{
    
    return bResetState;
}


/**
  * @brief  系统复位处理(放在while循环中处理)
  * @param  None
  * @retval None
  */
void CoreCtrl_ResetHandler(void)
{
    if (SysTime_CheckExpiredState(&m_ResetTimer))
    {
        CoreCtrl_ResetSystemNow();
    }
    
}
