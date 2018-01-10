/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "WFC_WifiCom.h"
#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "ExtPeripheral/WIFI/ESP82XX.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
      
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


#define WIFI_DEF_SSID                   "Duhanfeng"         //默认WIFI名
#define WIFI_DEF_PASSWORD               "stm32f10x.h"       //默认密码
      
//天气相关API接口
#define WIFI_WEATHER_CONNECT_TYPE       "TCP"               //天气API 连接类型
#define WIFI_WEATHER_IP                 "api.seniverse.com" //天气API 服务器地址
#define WIFI_WEATHER_PORT               (80)                //天气API 服务器端口
#define WIFI_WEATHER_API                "GET https://api.seniverse.com/v3/weather/now.json?key=7owcharxbd93kg9u&location=dongguan&language=en&unit=c\r\n\r\n\r\n"


//天气相关API接口
#define WIFI_CLOCK_CONNECT_TYPE         "TCP"               //时间API 连接类型
#define WIFI_CLOCK_IP                   "api.k780.com"      //时间API 服务器地址
#define WIFI_CLOCK_PORT                 (80)                //时间API 服务器端口
#define WIFI_CLOCK_API                  "GET http://api.k780.com/?app=life.time&appkey=29832&sign=4fea48c37f30b1ebc71afd09eaee9b1a&format=json\r\n\r\n\r\n"



#define WIFI_DEF_BUADRATE               (115200)            //默认串口波特率
      
static uBit32 WFC_GetJson(uBit8 *RecvPack, uBit32 ulRecvLen, uBit8 *JsonPack, uBit32 *pJsonLen)
{
    uBit32 ulTempJsonLen = 0;
    
    //入参判断
    if (ulRecvLen == 0)
    {
        return 1;
    }
    
    //寻找包头
    for (int i = 0; i < ulRecvLen; i++)
    {
        //找到包头
        if (RecvPack[i] == '{')
        {
            ulTempJsonLen = ulRecvLen - i;
            
            //内存复制
            memcpy(JsonPack, &RecvPack[i], ulTempJsonLen);
            
            break;
        }
    }
    
    //寻找包尾
    for (int i = 0; i < ulTempJsonLen; i++)
    {
        if (JsonPack[ulTempJsonLen - i -1] == '}')
        {
            *pJsonLen = ulTempJsonLen - i;
            
            return 0;
        }
        
        JsonPack[ulTempJsonLen - i -1] = 0;
    }
    
    return 1;
}



/*****************************************************************************
 * WIFI相关控制接口
 ****************************************************************************/

/**
  * @brief  WIFI模块初始化
  * @param  uUartNode 通信串口节点
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_InitWifi(uBit8 uUartNode)
{
    uBit32 ulRet = WFC_WIFI_ERR_SUCCESS;
    uBit32 ulEnetStatus = 0;
    
    //初始化ESP8266
    UART_Init(uUartNode, 115200);
    ESP82XX_InitInterface(uUartNode, UART_BlockSendBuff, UART_RecvBuff);
    
    //检测WIFI是否连接
    do 
    {
        //检测连接
        if (ESP82XX_CheckDeviceConnect())   
        {
            ulRet = WFC_WIFI_ERR_COM;
            break;
        }
        //设置工作模式
        if (ESP82XX_SetWifiMode(ESP82XX_WIFI_MODE_STA))
        {
            ulRet = WFC_WIFI_ERR_SET;
            break;
        }
        
        //获取网络状态
        if (ESP82XX_GetEnetStatus(&ulEnetStatus)) 
        {
            ulRet = 1;
            break;
        };
        
        //校验网络状态
        if (ulEnetStatus == ESP82XX_ENET_STATUS_DISCONNECT_AP)
        {
            //连接WIFI
            if (ESP82XX_ConnectWiFi(WIFI_DEF_SSID, WIFI_DEF_PASSWORD))
            {
                ulRet = WFC_WIFI_RER_CONNET;
                break;
            }
        }
        
    }while (0);
    
    return ulRet;
}


/**
  * @brief  天气信息包获取
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 WFC_RecvWeatherPack(uBit8 *pJsonPack, uBit32 *pJsonLen)
{
    uBit32 ulEnetStatus = 0;
    uBit32 ulRet = 0;
    uBit8  uRecvBuff[512] = {0};
    uBit32 ulRecvLen = 0;
    
    do 
    {
        //获取网络状态
        if (ESP82XX_GetEnetStatus(&ulEnetStatus)) 
        {
            ulRet = 1;
            break;
        };
        
        //校验网络状态
        if (ulEnetStatus == ESP82XX_ENET_STATUS_DISCONNECT_AP)
        {
            ulRet = 1;
            break;
        }
        
        //连接网络
        if (ESP82XX_ConnectEnet(WIFI_WEATHER_CONNECT_TYPE, WIFI_WEATHER_IP, WIFI_WEATHER_PORT))
        {
            ulRet = 1;
            break;
        }
        
        //获取网络状态
        if (ESP82XX_GetEnetStatus(&ulEnetStatus)) 
        {
            ulRet = 1;
            break;
        };
        
        //校验网络状态
        if (ulEnetStatus != ESP82XX_ENET_STATUS_CONNECT_TCP_UDP)
        {
            ulRet = 1;
            break;
        }
        
        //发送请求包
        if (ESP82XX_SendEnetPack(WIFI_WEATHER_API))
        {
            ulRet = 1;
            break;
        }
        
        //延时等待数据返回
        ulRecvLen = ESP82XX_RecvEnetPack(uRecvBuff, 512, 500);
        
        if (WFC_GetJson(uRecvBuff, ulRecvLen, pJsonPack, pJsonLen))
        {
            ulRet = 1;
            break;
        }
        
        
    }while (0);
    
    
    
    return ulRet;
}


/**
  * @brief  北京时间信息包获取
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 WFC_RecvClockPack(uBit8 *pJsonPack, uBit32 *pJsonLen)
{
    uBit32 ulEnetStatus = 0;
    uBit32 ulRet = 0;
    uBit8  uRecvBuff[512] = {0};
    uBit32 ulRecvLen = 0;
    
    do 
    {
        //获取网络状态
        if (ESP82XX_GetEnetStatus(&ulEnetStatus)) 
        {
            ulRet = 1;
            break;
        };
        
        //校验网络状态
        if (ulEnetStatus == ESP82XX_ENET_STATUS_DISCONNECT_AP)
        {
            ulRet = 1;
            break;
        }
        
        //连接网络
        if (ESP82XX_ConnectEnet(WIFI_CLOCK_CONNECT_TYPE, WIFI_CLOCK_IP, WIFI_CLOCK_PORT))
        {
            ulRet = 1;
            break;
        }
        
        //获取网络状态
        if (ESP82XX_GetEnetStatus(&ulEnetStatus)) 
        {
            ulRet = 1;
            break;
        };
        
        //校验网络状态
        if (ulEnetStatus != ESP82XX_ENET_STATUS_CONNECT_TCP_UDP)
        {
            ulRet = 1;
            break;
        }
        
        //发送请求包
        if (ESP82XX_SendEnetPack(WIFI_CLOCK_API))
        {
            ulRet = 1;
            break;
        }
        
        //延时等待数据返回
        ulRecvLen = ESP82XX_RecvEnetPack(uRecvBuff, 512, 500);
        
        if (WFC_GetJson(uRecvBuff, ulRecvLen, pJsonPack, pJsonLen))
        {
            ulRet = 1;
            break;
        }
        
        
    }while (0);
    
    return ulRet;
}





