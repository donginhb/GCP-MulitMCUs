#ifndef  __CLOCK_H
#define  __CLOCK_H

#include "DataType/DataType.h"


typedef struct
{
    uBit8   sec;
    uBit8   min;
    uBit8   hour;
    uBit8   week;
    uBit8   day;
    uBit8   month;
    uBit32  year;

}CLOCK_TYPE;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CLOCK相关控制接口
 ****************************************************************************/


//当前时间设置(从1970到现在的秒数)
void CLOCK_SetCurClock(uBit32 ulSec);


//系统时间更新
void CLOCK_Update(void);


//系统时钟地址获取
CLOCK_TYPE *CLOCK_GetSysClockAddr(void);


#ifdef __cplusplus
}
#endif

#endif /* __CLOCK_H */
