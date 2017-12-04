/**
  ******************************************************************************
  * @file    ESP82XX.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   ESP82XX Driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "ESP82XX.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//系统控制
#define ESP82XX_AT_ECHO_OFF                 "ATE0\r\n"          //关闭回显
#define ESP82XX_AT_ECHO_ON                  "ATE1\r\n"          //打开回显
#define ESP82XX_AT_TEST                     "AT\r\n"            //测试是否启动

//WIFI模式设置
#define ESP82XX_AT_SET_CUR_WIFI_STA_MODE        "AT+CWMODE_CUR=1\r\n"   //设置为客户机(Station)模式
#define ESP82XX_AT_SET_CUR_WIFI_AP_MODE         "AT+CWMODE_CUR=2\r\n"   //设置为AP模式
#define ESP82XX_AT_SET_CUR_WIFI_STA_AP_MODE     "AT+CWMODE_CUR=3\r\n"   //设置为Station+AP模式
#define ESP82XX_AT_SET_DEF_WIFI_STA_MODE        "AT+CWMODE_DEF=1\r\n"   //设置为客户机(Station)模式
#define ESP82XX_AT_SET_DEF_WIFI_AP_MODE         "AT+CWMODE_DEF=2\r\n"   //设置为AP模式
#define ESP82XX_AT_SET_DEF_WIFI_STA_AP_MODE     "AT+CWMODE_DEF=3\r\n"   //设置为Station+AP模式

//WIFI连接
#define ESP82XX_AT_SET_CUR_WIFI_CONNECT         "AT+CWJAP_CUR="         //设置WIFI连接(当前)
#define ESP82XX_AT_SET_DEF_WIFI_CONNECT         "AT+CWJAP_DEF="         //设置WIFI连接(默认)
#define ESP82XX_AT_SET_WIFI_DISCONNECT          "AT+CWQAP\r\n"          //断开连接

//网络状态查询
#define ESP82XX_AT_GET_ENET_STATUS              "AT+CIPSTATUS\r\n"      //查询网络状态

//TCP连接设置
#define ESP82XX_AT_SET_TCP_CONNECT              "AT+CIPSTART=\"TCP\","  //连接TCP服务器连接

//网络数据长度设置
#define ESP82XX_AT_SET_TCP_PACK_LEN             "AT+CIPSENDEX=113"      //设置TCP数据包长度


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define ESP82XX_MAX_RECV_BUFF_LEN               (512)   //最大接收缓冲区长度
#define ESP82XX_MAX_RECV_CMD_LEN                (512)   //最大接收指令长度

static uBit32 (*pf_ESP82XX_SendBuff)(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize) = NULL;     //发送函数指针
static uBit32 (*pf_ESP82XX_RecvBuff)(uBit8 uUartNode, uBit8 *pRBuff, uBit32 ulSize) = NULL;     //接收函数指针
static uBit32 m_uUartNode = 0;                                                                  //串口节点

static uBit8  m_uRecvBuff[ESP82XX_MAX_RECV_BUFF_LEN] = {0};     //接收缓冲区
static uBit8  m_uRecvCmd[ESP82XX_MAX_RECV_CMD_LEN] = {0};       //接收指令

static uBit32 m_ulRecvIndex = 0;                                //接收缓冲区索引


/**
  * @brief  串口数据发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval 0-成功  非0-失败
  */
static uBit32 ESP82XX_SendBuff(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize)
{
    if (pf_ESP82XX_SendBuff)
    {
        return pf_ESP82XX_SendBuff(uUartNode, pSBuff, ulSize);
    }
    
    return 1;
}


/**
  * @brief  串口接收(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uBit32 实际上接收到的数据长度
  */
static uBit32 ESP82XX_RecvBuff(uBit8 uUartNode, uBit8 *pRBuff, uBit32 ulSize)
{
    if (pf_ESP82XX_RecvBuff)
    {
        return pf_ESP82XX_RecvBuff(uUartNode, pRBuff, ulSize);
    }
    
    return 0;
}


/**
  * @brief  清空接收缓冲区
  * @param  None
  * @retval None
  */
static void ESP82XX_ClearRecvBuff(void)
{
    //获取所有的数据
    while (ESP82XX_RecvBuff(m_uUartNode, m_uRecvBuff, ESP82XX_MAX_RECV_BUFF_LEN));
    
    //清空缓冲区
    memset(m_uRecvBuff, 0, ESP82XX_MAX_RECV_BUFF_LEN);
    
}


/**
  * @brief  ESP82XX返回指令获取
  * @param  uUartNode 串口节点号
  * @param  uUartNode 串口节点号
  * @retval 0-成功  非0-失败
  */
static uBit32 ESP82XX_GetReturnCmd(uBit8 *pRecvCmd, uBit32 *pCmdLen)
{
    if (ESP82XX_RecvBuff(m_uUartNode, &m_uRecvBuff[m_ulRecvIndex], 1))
    {
        m_ulRecvIndex++;
        
        if ((m_ulRecvIndex >= 2) && (m_uRecvBuff[m_ulRecvIndex-1] == '\n') && (m_uRecvBuff[m_ulRecvIndex-2] == '\r'))
        {
            if (m_ulRecvIndex == 2)
            {
                m_ulRecvIndex = 0;
                return 1;
            }
            memcpy(pRecvCmd, m_uRecvBuff, m_ulRecvIndex-2);
            *pCmdLen = m_ulRecvIndex-2;
            m_ulRecvIndex = 0;
            return 0;
        }
    }
    
    return 1;
}


/**
  * @brief  ESP82XX回显设置(设备默认开启)
  * @param  bIsEnable 使能 true-开启回显 false-关闭回显
  * @retval 0-成功  非0-失败
  */
static uBit32 ESP82XX_SetEchoEnable(bool bIsEnable)
{
    SYS_TIME_DATA RecvTimer = {0};
    uBit8 *pCmd = NULL;
    uBit32 ulRecvCmdLen = 0;
    
    if (bIsEnable)
    {
        pCmd = ESP82XX_AT_ECHO_ON;
    }
    else 
    {
        pCmd = ESP82XX_AT_ECHO_OFF;
    }
    
    //清空之前接收到的数据
    ESP82XX_ClearRecvBuff();
    
    //发送数据
    ESP82XX_SendBuff(m_uUartNode, pCmd, strlen((char const *)pCmd));
    
    //等待数据接收
    SysTime_Start(&RecvTimer, 500);
    
    while (!SysTime_CheckExpiredState(&RecvTimer))
    {
        if (ESP82XX_GetReturnCmd(m_uRecvCmd, &ulRecvCmdLen) == 0)
        {
            SysTime_Start(&RecvTimer, 500);
            
            if (memcmp(m_uRecvCmd, "OK", strlen("OK")) == 0)
            {
                return 0;
            }
        }
    }
    
    return 1;
}


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
                             uBit32 (*pf_ESP_RecvBuff)(uBit8 , uBit8 *, uBit32 ))
{
    do 
    {
        //数据校验
        if (!pf_ESP_SendBuff)  break;
        if (!pf_ESP_RecvBuff)  break;
        
        //存储指针
        pf_ESP82XX_SendBuff = pf_ESP_SendBuff;
        pf_ESP82XX_RecvBuff = pf_ESP_RecvBuff;
        
        m_uUartNode = uUartNode;
        
        //关闭回显
        ESP82XX_SetEchoEnable(false);
        
        return 0;
    }while (0);
    
    return 1;
}


/**
  * @brief  ESP82XX 设备连接有效检测
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_CheckDeviceConnect(void)
{
    SYS_TIME_DATA RecvTimer = {0};
    uBit8 *pCmd = ESP82XX_AT_TEST;
    uBit32 ulRecvCmdLen = 0;
    
    //清空之前接收到的数据
    ESP82XX_ClearRecvBuff();
    
    //发送数据
    ESP82XX_SendBuff(m_uUartNode, pCmd, strlen((char const *)pCmd));
    
    //等待数据接收
    SysTime_Start(&RecvTimer, 500);
    
    while (!SysTime_CheckExpiredState(&RecvTimer))
    {
        if (ESP82XX_GetReturnCmd(m_uRecvCmd, &ulRecvCmdLen) == 0)
        {
            SysTime_Start(&RecvTimer, 500);
            
            if (memcmp(m_uRecvCmd, "OK", strlen("OK")) == 0)
            {
                return 0;
            }
        }
    }
    
    return 1;
}


/**
  * @brief  ESP82XX WIFI模式设置
  * @param  ulWifiMode wifi 模式
  *   @arg ESP82XX_WIFI_STA_MODE    客户机(Station)模式
  *   @arg ESP82XX_WIFI_AP_MODE     AP模式
  *   @arg ESP82XX_WIFI_STA_AP_MODE Station+AP模式
  * @retval 0-成功  非0-失败
  */
uBit32 ESP82XX_SetWifiMode(uBit32 ulWifiMode)
{
    SYS_TIME_DATA RecvTimer = {0};
    uBit8 *pCmd = NULL;
    uBit32 ulRecvCmdLen = 0;
    
    switch (ulWifiMode)
    {
    case ESP82XX_WIFI_STA_MODE   : pCmd = ESP82XX_AT_SET_CUR_WIFI_STA_MODE   ; break;
    case ESP82XX_WIFI_AP_MODE    : pCmd = ESP82XX_AT_SET_CUR_WIFI_AP_MODE    ; break;
    case ESP82XX_WIFI_STA_AP_MODE: pCmd = ESP82XX_AT_SET_CUR_WIFI_STA_AP_MODE; break;
    default: break;
    }
    
    //清空之前接收到的数据
    ESP82XX_ClearRecvBuff();
    
    //发送数据
    ESP82XX_SendBuff(m_uUartNode, pCmd, strlen((char const *)pCmd));
    
    //等待数据接收
    SysTime_Start(&RecvTimer, 500);
    
    while (!SysTime_CheckExpiredState(&RecvTimer))
    {
        if (ESP82XX_GetReturnCmd(m_uRecvCmd, &ulRecvCmdLen) == 0)
        {
            SysTime_Start(&RecvTimer, 500);
            
            if (memcmp(m_uRecvCmd, "OK", strlen("OK")) == 0)
            {
                return 0;
            }
        }
    }
    
    return 1;
}



