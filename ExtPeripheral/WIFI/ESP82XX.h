#ifndef  __ESP82XX_H
#define  __ESP82XX_H

#include "DataType/DataType.h"

#define ESP82XX_WIFI_STA_MODE           (1) //设置为客户机(Station)模式
#define ESP82XX_WIFI_AP_MODE            (2) //设置为AP模式
#define ESP82XX_WIFI_STA_AP_MODE        (3) //设置为Station+AP模式

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * ESP82XX相关控制接口
 ****************************************************************************/

/**
  * @brief  ESP82XX 接口初始化
  * @param  uUartNode 串口节点号
  * @param  pf_ESP_SendBuff 发送函数指针
  * @param  pf_ESP_RecvBuff 接收函数指针
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_InitInterface(uBit8 uUartNode,
                             uBit32 (*pf_ESP_SendBuff)(uBit8 , uBit8 *, uBit32 ),
                             uBit32 (*pf_ESP_RecvBuff)(uBit8 , uBit8 *, uBit32 ));


/**
  * @brief  ESP82XX 设备连接有效检测
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_CheckDeviceConnect(void);


/**
  * @brief  ESP82XX WIFI模式设置
  * @param  ulWifiMode wifi 模式
  *   @arg ESP82XX_WIFI_STA_MODE    客户机(Station)模式
  *   @arg ESP82XX_WIFI_AP_MODE     AP模式
  *   @arg ESP82XX_WIFI_STA_AP_MODE Station+AP模式
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_SetWifiMode(uBit32 ulWifiMode);


#ifdef __cplusplus
}
#endif

#endif /* __ESP82XX_H */
