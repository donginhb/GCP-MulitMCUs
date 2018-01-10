#ifndef  __WIFI_CLOCK_H
#define  __WIFI_CLOCK_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

/*****************************************************************************
 * 网络参数接口获取解析相关接口
 ****************************************************************************/

/**
  * @brief  天气更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateWeather(void);


/**
  * @brief  时间更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateClock(void);


/**
  * @brief  时间更新管理
  * @param  None
  * @retval None
  */
void WFC_UpdateClockHandler(void);


/*****************************************************************************
 * WIFI时钟显示相关接口
 ****************************************************************************/

/**
  * @brief  更新显示数据
  * @param  None
  * @retval None
  */
void WFC_UpdateDisplay(void);


#ifdef __cplusplus
}
#endif

#endif /* __WIFI_CLOCK_H */
