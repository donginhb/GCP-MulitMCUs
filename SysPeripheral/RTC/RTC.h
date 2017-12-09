#ifndef  __RTC_H
#define  __RTC_H

#include "DataType/DataType.h"


typedef struct
{
    uBit32  sec;
    uBit32  min;
    uBit32  hour;
    uBit32  week;
    uBit32  day;
    uBit32  month;
    uBit32  year;

}RTC_TIME;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CLOCK相关控制接口
 ****************************************************************************/


//当前时间设置(从1970到现在的秒数)
void RTC_SetCurClock(uBit32 ulSec);


//系统时间更新
void RTC_Update(void);


//系统时钟地址获取
RTC_TIME *RTC_GetTimeAddr(void);


#ifdef __cplusplus
}
#endif

#endif /* __RTC_H */
