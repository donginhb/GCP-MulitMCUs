#ifndef  __WIFI_CLOCK_H
#define  __WIFI_CLOCK_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

/**
  * @brief  天气更新
  * @param  None
  * @retval None
  */
uBit32 WFC_UpdateWeather(void);


/**
  * @brief  天气显示
  * @param  None
  * @retval None
  */
uBit32 WFC_DisplayWeather(void);


/**
  * @brief  天气更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateClock(void);

/**
  * @brief  时钟获取
  * @param  None
  * @retval None
  */
uBit32 WFC_GetBaseClock(void);




//解包测试
//长时间测试,以确定是否有内存溢出
void WFC_UnpackTest(void);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_CLOCK_H */
