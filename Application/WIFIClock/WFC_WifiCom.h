#ifndef  __DEMO_H
#define  __DEMO_H

#include "DataType/DataType.h"

//WIFI 错误码定义
#define WFC_WIFI_ERR_SUCCESS            (0)             //成功
#define WFC_WIFI_ERR_COM                (1)             //通信失败
#define WFC_WIFI_ERR_SET                (2)             //指令设置失败
#define WFC_WIFI_RER_CONNET             (3)             //WIFI连接失败

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * WIFI相关控制接口
 ****************************************************************************/

/**
  * @brief  WIFI模块初始化
  * @param  uUartNode 通信串口节点
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_InitWifi(uBit8 uUartNode);


/**
  * @brief  天气信息包获取
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 WFC_RecvWeatherPack(uBit8 *pJsonPack, uBit32 *pJsonLen);


/**
  * @brief  北京时间信息包获取
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 WFC_RecvClockPack(uBit8 *pJsonPack, uBit32 *pJsonLen);

    

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_H */
