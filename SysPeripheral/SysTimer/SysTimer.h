#ifndef  __SYS_TIMER_H
#define  __SYS_TIMER_H

#include "DataType/DataType.h"

//系统定时器数据结构
typedef struct
{
    uBit32  ulEnable;     //使能标志
    uBit32  ulOneshot;    //单次计时标志
    uBit32  ulMsDelay;    //延时时间
    uBit32  ulMsStart;    //开始执行的时刻

}SYS_TIME_DATA;


#ifdef __cplusplus
extern "C"
{
#endif


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
void SysTime_Init(uBit32 ulTickPeriod, bool bEnableUsDelay, uBit8 uUsTimeNode);


/*****************************************************************************
 * 延时功能
 ****************************************************************************/

/**
  * @brief  系统滴答时基获取
  * @param  None
  * @retval 系统滴答时基
  */
uBit32 SysTime_GetTickCount(void);


/**
  * @brief  简单的延时函数
  * @param  ulDelay 延时参数,无标准单位,入参范围[1-0xFFFFFFFF]
  * @retval None
  * @note   在96MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  *         在204MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  */
void SysTime_SimpleDelay(uBit32 ulDelay);


/**
  * @brief  微秒级延时
  * @param  ulUsDelay 微秒数,入参范围[1-0xFFFFFFFF](若使用硬件定时器,需考虑实际定时器位数)
  * @retval None
  */
void SysTime_DelayUs(uBit32 ulUsDelay);


/**
  * @brief  毫秒级延时
  * @param  ulMsDelay 毫秒数,入参范围[1-0xFFFFFFFF](49.7天)
  * @retval None
  */
void SysTime_DelayMs(uBit32 ulMsDelay);


/*****************************************************************************
 * 系统定时器实现
 ****************************************************************************/

/**
  * @brief  系统定时器启动
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_Start(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay);


/**
  * @brief  系统定时器启动(单次)
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_StartOneShot(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay);


/**
  * @brief  系统定时器取消
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_Cancel(SYS_TIME_DATA *pTimeData);


/**
  * @brief  系统定时器超时状态检测
  * @param  pTimeData 系统定时器指针
  * @retval 0-未超时  1-超时
  */
uBit8 SysTime_CheckExpiredState(SYS_TIME_DATA *pTimeData);


/*****************************************************************************
 * 计时功能
 ****************************************************************************/

/**
  * @brief  计时启动
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_StartReckon(SYS_TIME_DATA *pTimeData);


/**
  * @brief  计时值获取
  * @param  pTimeData 系统定时器指针
  * @retval 计时数(毫秒)
  */
uBit32 SysTime_GetReckonValue(SYS_TIME_DATA *pTimeData);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_TIMER_H */
