/**
  ******************************************************************************
  * @file    Clock.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.07
  * @brief   Clock
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "RTC.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit32 m_ulBaseSysClock = 0;         //基系统时间
static uBit32 m_ulBaseClock = 0;            //从1970年至当前的秒数
static RTC_TIME m_RtcClock = {0};           //系统时钟接口
const static uBit8 m_uMonTable[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   //每月日期表


/**
  * @brief  闰年判断
  * @param  None
  * @retval 0-闰年 非0-非闰年
  */
static bool RTC_IsLeapYear(uBit32 ulYear)
{             
    if( ((ulYear%4==0) && (ulYear%100!=0)) || (ulYear%400==0) ) 
    {
        return true;
    }
    
    return false;
}


/*****************************************************************************
 * RTC相关控制接口
 ****************************************************************************/

/**
  * @brief  当前时间设置
  * @param  ulSec 当前秒数(从1970到现在的秒数)
  * @param  uGmtIndex 当前地区的市区偏移
  * @retval 0-成功 非0-失败
  */
uBit32 RTC_SetCurClock(uBit32 ulSec, uBit8 uGmtIndex)
{
    if (uGmtIndex >= 24)
    {
        return 1;
    }
    
    uBit32 ulBaseCloseIndex = uGmtIndex * 60 * 60;  //格林时间偏移,如果是计算北京时间,则入参8
    
    m_ulBaseClock = ulSec + ulBaseCloseIndex;       //获取当前时间(从1970到现在的秒数)
    m_ulBaseSysClock = SysTime_GetTickCount();      //获取滴答时间
    
    return 0;
}


/**
  * @brief  系统时间更新
  * @param  None
  * @retval None
  */
void RTC_Update(void)
{
    uBit32 ulDay;
    uBit32 ulTemp=1970;
    uBit32 ulSecIndex = 0;
    
    ulSecIndex = (SysTime_GetTickCount() - m_ulBaseSysClock)/1000;  //距离上次执行的秒数
    
    m_ulBaseClock += ulSecIndex;    //刷新基时钟(单位:S)
    m_ulBaseSysClock += ulSecIndex *1000;    //刷新系统基时钟(单位:MS)

    ulDay = m_ulBaseClock/(24*3600);//算出总的天数
    
    while(ulDay>=365)
    {
        if(RTC_IsLeapYear(ulTemp))
        {
            if(ulDay>=366)
            {
                ulDay = ulDay-366;
            }else break;
        }
        else ulDay = ulDay-365;
        ulTemp++;
    }
    m_RtcClock.year = ulTemp;
    
    ulTemp=0;
    
    while(ulDay>=28)
    {
        if(RTC_IsLeapYear(m_RtcClock.year)&&ulTemp==1)
        {
            if(ulDay>=29)
            {
                ulDay = ulDay-29;
            }else break;
        }
        else
        {
            if(ulDay>=m_uMonTable[ulTemp])
            {
                ulDay = ulDay-m_uMonTable[ulTemp];
            }else break;
        }
        ulTemp++;
    }
    
    //假如有32天 11:21:23 2月2号11:21:23
    m_RtcClock.month = ulTemp+1;    //为什么要+1？原因：比如temp=1，就证明day里面的数不止一个月，即已经到2月了
    m_RtcClock.day = ulDay+1;       //+1即表示当天的日期不加就表示前天的
    m_RtcClock.hour = m_ulBaseClock/3600%24;
    m_RtcClock.min = m_ulBaseClock/60%60;
    m_RtcClock.sec = m_ulBaseClock%60;
    
}


/**
  * @brief  系统时钟地址获取
  * @param  None
  * @retval Rtc时间结构指针
  */
RTC_TIME *RTC_GetTimeAddr(void)
{
    
    return &m_RtcClock;
}




