#ifndef  __ESP82XX_H
#define  __ESP82XX_H

#include "DataType/DataType.h"

//WIFI模式定义
#define ESP82XX_WIFI_MODE_STA                   (1) //设置为客户机(Station)模式
#define ESP82XX_WIFI_MODE_AP                    (2) //设置为AP模式
#define ESP82XX_WIFI_MODE_STA_AP                (3) //设置为Station+AP模式

//网络状态定义
#define ESP82XX_ENET_STATUS_ERR                 (0) //无效查询
#define ESP82XX_ENET_STATUS_CONNECT_AP          (2) //已连接AP
#define ESP82XX_ENET_STATUS_CONNECT_TCP_UDP     (3) //已建立TCP/UDP连接
#define ESP82XX_ENET_STATUS_DISCONNECT_ENET     (4) //断开网络连接
#define ESP82XX_ENET_STATUS_DISCONNECT_AP       (5) //未连接AO

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
  *   @arg ESP82XX_WIFI_MODE_STA    客户机(Station)模式
  *   @arg ESP82XX_WIFI_MODE_AP     AP模式
  *   @arg ESP82XX_WIFI_MODE_STA_AP Station+AP模式
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_SetWifiMode(uBit32 ulWifiMode);


/**
  * @brief  ESP82XX WIFI连接
  * @param  pSSID WIFI名称
  * @param  pPassWord WIFI 密码
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_ConnectWiFi(char *pSSID, char *pPassWord);


/**
  * @brief  ESP82XX 获取网络状态
  * @param  pEnetStatus 网络状态
  *   @arg  SP82XX_ENET_STATUS_ERR              无效查询
  *   @arg  SP82XX_ENET_STATUS_CONNECT_AP       已连接AP
  *   @arg  SP82XX_ENET_STATUS_CONNECT_TCP_UDP  已建立TCP/UDP连接
  *   @arg  SP82XX_ENET_STATUS_DISCONNECT_ENET  断开网络连接
  *   @arg  SP82XX_ENET_STATUS_DISCONNECT_AP    未连接AP
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_GetEnetStatus(uBit32 *pEnetStatus);

      
/**
  * @brief  ESP82XX 网络连接
  * @param  pType   连接类型
  *   @arg      "TCP" TCP方式连接
  *   @arg      "UDP" UDP方式连接
  * @param  pDstIP  目标IP/域名
  * @param  ulPort  目标端口
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_ConnectEnet(uBit8 *pType, uBit8 *pDstIP, uBit32 ulPort);


/**
  * @brief  ESP82XX 网络数据包长度设置
  * @param  ulPackLen  包长度
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_SetSendPackLen(uBit32 ulPackLen);


/**
  * @brief  ESP82XX 网络数据包发送
  * @param  pSendBuff  要发送的数据
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_SendEnetPack(uBit8 *pSendBuff);


/**
  * @brief  ESP82XX 网络数据包接收
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @param  ulOverTime 超时时间
  * @retval uBit32 实际上接收到的数据长度
  */
uBit32 ESP82XX_RecvEnetPack(uBit8 *pRBuff, uBit32 ulSize, uBit32 ulOverTime);


#ifdef __cplusplus
}
#endif

#endif /* __ESP82XX_H */
