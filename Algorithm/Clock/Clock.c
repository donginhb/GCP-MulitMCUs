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
#include "Clock.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define GMT_INDEX_8             (60*60*8)

static uBit32 m_ulBaseSysClock = 0; //基系统时间
static uBit32 m_ulBaseClock = 0;    //从1970年至当前的秒数

      
static CLOCK_TYPE m_SysClock = {0}; //系统时钟接口
      

//闰年判断
static bool Is_LeapYear(uBit16 year)
{             
    if(((year%4==0)&&(year%100!=0))||(year%400==0)) 
    {
        return true;
    }   
    else 
    {   
        return false;
    }
}


/*****************************************************************************
 * CLOCK相关控制接口
 ****************************************************************************/


//当前时间设置(从1970到现在的秒数)
void CLOCK_SetCurClock(uBit32 ulSec)
{
    m_ulBaseClock = ulSec;  //获取当前时间
    m_ulBaseSysClock = SysTime_GetTickCount();  //获取滴答时间
    
}


//系统时间更新
void CLOCK_Update(void)
{
    uBit32 day;
    uBit32 temp=1970;
    uBit32 RTC_time = 0;
    uBit8 mon_table[]={31,28,31,30,31,30,31,31,30,31,30,31};

    RTC_time = m_ulBaseClock +  ((SysTime_GetTickCount() - m_ulBaseSysClock)/1000) + GMT_INDEX_8;
    
    day = RTC_time/(24*3600);//算出总的天数
    while(day>=365)
    {
        if(Is_LeapYear(temp))
        {
            if(day>=366)
            {
                day = day-366;
            }else break;
        }
        else day = day-365;
        temp++;
    }
    m_SysClock.year = temp;
    temp=0;
    while(day>=28)
    {
        if(Is_LeapYear(m_SysClock.year)&&temp==1)
        {
            if(day>=29)
            {
                day = day-29;
            }else break;
        }
        else
        {
            if(day>=mon_table[temp])
            {
                day = day-mon_table[temp];
            }else break;
        }
        temp++;
    }
    //假如有32天 11:21:23 2月2号11:21:23
    m_SysClock.month = temp+1; //为什么要+1？原因：比如temp=1，就证明day里面的数不止一个月，即已经到2月了
    m_SysClock.day = day+1; //+1即表示当天的日期不加就表示前天的
    m_SysClock.hour = RTC_time/3600%24;
    m_SysClock.min = RTC_time/60%60;
    m_SysClock.sec = RTC_time%60;
    
}


//系统时钟地址获取
CLOCK_TYPE *CLOCK_GetSysClockAddr(void)
{
    
    return &m_SysClock;
}




