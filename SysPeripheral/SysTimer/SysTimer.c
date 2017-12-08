/**
  ******************************************************************************
  * @file    SysTimer.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   SysTimer API
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysTimer.h"
#include "DataType/DataType.h"
#include "HAL/HAL_SysTick.h"
#include "HAL/HAL_Delay.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define CLOCK_TIME_MAX  (0xFFFFFFFF)    //32位,时钟变量位数

static bool bUsEnableFlag = false;        

/**
  * @brief  时间差获取
  * @param  ulStartTime 开始时间
  * @param  ulFinalTime 结束时间
  * @retval None
  */
static uBit32 SysTime_GetTimeDiffMs(uBit32 ulStartTime, uBit32 ulFinalTime)
{
  if (ulFinalTime >= ulStartTime)
  {
    return (ulFinalTime - ulStartTime);
  }

  return ((CLOCK_TIME_MAX - ulStartTime + 1) + ulFinalTime);  // 补码处理
}


/*****************************************************************************
 * 系统定时器配置接口
 ****************************************************************************/

/**
  * @brief  精确延时使能
  * @param  ulTickPeriod 滴答周期(单位:MS)
  * @param  bEnableUsDelay 是否使能US级延时
  * @param  uUsTimeNode  用于US级延时的定时器节点号
  * @retval None
  * @note   如果使能精确的微秒级延时,则需要启用一个硬件定时器,
  *         根据实际需求来决定是否启用硬件定时器;
  */
void SysTime_Init(uBit32 ulTickPeriod, bool bEnableUsDelay, uBit8 uUsTimeNode)
{
    HAL_SysTick_Init(1);
    
    if (bEnableUsDelay)
    {
        bUsEnableFlag = bEnableUsDelay;
        
        HAL_DelayInit(uUsTimeNode);
    }
    
}


/*****************************************************************************
 * 延时功能
 ****************************************************************************/

/**
  * @brief  系统滴答时基获取
  * @param  None
  * @retval 系统滴答时基
  */
uBit32 SysTime_GetTickCount(void)
{
    
    return HAL_SysTick_GetCount();
}


/**
  * @brief  简单的延时函数
  * @param  ulDelay 延时参数,无标准单位,入参范围[1-0xFFFFFFFF]
  * @retval None
  * @note   在96MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  *         在204MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  */
void SysTime_SimpleDelay(uBit32 ulDelay)
{
    for(volatile int i = 0; i < ulDelay; i++);
    
}


/**
  * @brief  微秒级延时
  * @param  ulUsDelay 微秒数,入参范围[1-0xFFFFFFFF](若使用硬件定时器,需考虑实际定时器位数)
  * @retval None
  */
void SysTime_DelayUs(uBit32 ulUsDelay)
{
    if (bUsEnableFlag)
    {
        HAL_DelayUs(ulUsDelay);
    }
    else 
    {
        SysTime_SimpleDelay(ulUsDelay*10);
    }
    
}


/**
  * @brief  毫秒级延时
  * @param  ulMsDelay 毫秒数,入参范围[1-0xFFFFFFFF](49.7天)
  * @retval None
  */
void SysTime_DelayMs(uBit32 ulMsDelay)
{
    //获取当前的滴答值
    uBit32 ulMsStart = SysTime_GetTickCount();
    
    //等待超时
    while (!(SysTime_GetTimeDiffMs(ulMsStart, SysTime_GetTickCount()) >= ulMsDelay));
    
}


/*****************************************************************************
 * 系统定时器实现
 ****************************************************************************/

/**
  * @brief  系统定时器启动
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_Start(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay)
{
    pTimeData->ulEnable  = 1;
    pTimeData->ulOneshot = 0;
    pTimeData->ulMsDelay = ulMsDelay;
    pTimeData->ulMsStart = SysTime_GetTickCount();
    
}


/**
  * @brief  系统定时器启动(单次)
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_StartOneShot(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay)
{
    pTimeData->ulEnable  = 1;
    pTimeData->ulOneshot = 1;
    pTimeData->ulMsDelay = ulMsDelay;
    pTimeData->ulMsStart = SysTime_GetTickCount();
    
}


/**
  * @brief  系统定时器取消
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_Cancel(SYS_TIME_DATA *pTimeData)
{
    pTimeData->ulEnable  = 0;

}


/**
  * @brief  系统定时器超时状态检测
  * @param  pTimeData 系统定时器指针
  * @retval 0-未超时  1-超时
  */
uBit8 SysTime_CheckExpiredState(SYS_TIME_DATA *pTimeData)
{
    if (!pTimeData->ulEnable)
    {
        return 0;
    }
    
    if (SysTime_GetTimeDiffMs(pTimeData->ulMsStart, SysTime_GetTickCount()) >= pTimeData->ulMsDelay)
    {
        if (pTimeData->ulOneshot)
        {
            pTimeData->ulEnable = 0;
        }
        return 1;
    }
    
    return 0;
}


/*****************************************************************************
 * 计时功能
 ****************************************************************************/

/**
  * @brief  计时启动
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_StartReckon(SYS_TIME_DATA *pTimeData)
{
    pTimeData->ulEnable  = 1;
    pTimeData->ulOneshot = 0;
    pTimeData->ulMsDelay = 0;
    pTimeData->ulMsStart = SysTime_GetTickCount();

}


/**
  * @brief  计时值获取
  * @param  pTimeData 系统定时器指针
  * @retval 计时数(毫秒)
  */
uBit32 SysTime_GetReckonValue(SYS_TIME_DATA *pTimeData)
{
    
    return SysTime_GetTimeDiffMs(pTimeData->ulMsStart, SysTime_GetTickCount());
}

