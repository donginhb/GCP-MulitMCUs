/**
  ******************************************************************************
  * @file    WFC_WifiClock.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.05
  * @brief   Wifi Clock Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "WFC_WifiClock.h"
#include "WFC_WifiCom.h"
#include "WFC_HardwareDef.h"
#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/RTC/RTC.h"
#include "Algorithm/Json/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * WIFI CLOCK相关控制接口
 ****************************************************************************/

/**
  * @brief  天气获取
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_GetWeatherParmForJson(char const *pJsonPack, char *pKey, char *pValue)
{
    uBit32 ulRet = 0;
    cJSON *pWeatherJson = NULL;
    
    //将字符串转Json结构
    pWeatherJson = cJSON_Parse(pJsonPack);
    
    //判断是否解析成功
    if (pWeatherJson == NULL)
    {
        return 1;
    }
    
    //数据提取
    do 
    {
        //提取"results" 对象
        cJSON *pResultsJson = cJSON_GetObjectItem(pWeatherJson, "results");
        
        //数据校验
        if (pResultsJson == NULL) 
        { 
            ulRet = 1; 
            break;
        }
        
        //提取"location"对象
        cJSON *pLocationJson = cJSON_GetObjectItem(pResultsJson->child, "location");
        
        //提取"now"对象
        cJSON *pNowJson = cJSON_GetObjectItem(pResultsJson->child, "now");
        
        //校验数据
        if ((pLocationJson == NULL) || (pNowJson == NULL))
        {
            ulRet = 1; 
            break;
        }
        
        //检测KEY是否在当前对象中
        if (cJSON_HasObjectItem(pLocationJson, pKey))
        {
            //数据出参
            uBit32 ulStrLen = 0;
            
            //校验VALUE是否为字符串
            if (cJSON_GetObjectItem(pLocationJson, pKey)->type == cJSON_String)
            {
                char *pStrine = cJSON_GetObjectItem(pLocationJson, pKey)->valuestring;
                ulStrLen = strlen(pStrine);
                
                //数据出参
                memcpy(pValue, pStrine, ulStrLen);
                
                ulRet = 0;
                
                break;
            }
        }
        
        //检测KEY是否在当前对象中
        if (cJSON_HasObjectItem(pNowJson, pKey))
        {
            //数据出参
            uBit32 ulStrLen = 0;
            
            //校验VALUE是否为字符串
            if (cJSON_GetObjectItem(pNowJson, pKey)->type & cJSON_String)
            {
                char *pStrine = cJSON_GetObjectItem(pNowJson, pKey)->valuestring;
                ulStrLen = strlen(pStrine);
                
                //数据出参
                memcpy(pValue, pStrine, ulStrLen);
                
                ulRet = 0;
                
                break;
            }
        }
        
    }while(0);
    
    
    cJSON_Delete(pWeatherJson);
    return ulRet;
}



/**
  * @brief  天气获取
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_GetBaseClockForJson(char const *pJsonPack, char *pKey, char *pValue)
{
    uBit32 ulRet = 0;
    cJSON *pClockJson = NULL;
    
    //将字符串转Json结构
    pClockJson = cJSON_Parse(pJsonPack);
    
    //判断是否解析成功
    if (pClockJson == NULL)
    {
        return 1;
    }
    
    //数据提取
    do 
    {
        //提取"results" 对象
        cJSON *pResultsJson = cJSON_GetObjectItem(pClockJson, "result");
        
        //数据校验
        if (pResultsJson == NULL) 
        { 
            ulRet = 1; 
            break;
        }
        
        //检测KEY是否在当前对象中
        if (cJSON_HasObjectItem(pResultsJson, pKey))
        {
            //数据出参
            uBit32 ulStrLen = 0;
            
            //校验VALUE是否为字符串
            if (cJSON_GetObjectItem(pResultsJson, pKey)->type & cJSON_String)
            {
                char *pStrine = cJSON_GetObjectItem(pResultsJson, pKey)->valuestring;
                ulStrLen = strlen(pStrine);
                
                //数据出参
                memcpy(pValue, pStrine, ulStrLen);
                
                ulRet = 0;
                
                break;
            }
        }
        
    }while(0);
    
    
    cJSON_Delete(pClockJson);
    return ulRet;
}


static uBit8 m_uWeatherJsonPack[512] = {0}; //天气JSON包(字符串)
static uBit32 m_ulWeatherJsonLen = 0;       //天气JSON包长度

static uBit8 m_uClockJsonPack[512] = {0};   //北京时间JSON包(字符串)
static uBit32 m_ulClockJsonLen = 0;         //北京时间JSON包长度




/**
  * @brief  天气更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateWeather(void)
{
    
    return WFC_RecvWeatherPack(m_uWeatherJsonPack, &m_ulWeatherJsonLen);
}


/**
  * @brief  时间更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateClock(void)
{
    
    return WFC_RecvClockPack(m_uClockJsonPack, &m_ulClockJsonLen);
}


/**
  * @brief  天气显示
  * @param  None
  * @retval None
  */
uBit32 WFC_DisplayWeather(void)
{
    uBit8 uName[64] = {0};
    uBit8 uText[64] = {0};
    uBit8 uTemperature[64] = {0};
    uBit8 uDisplayBuff[256] = {0};
    
    //解析数据包
    if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "name", (char *)&uName))
    {
        return 1;
    }
    
    if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "text", (char *)&uText))
    {
        return 1;
    }
    
    if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "temperature", (char *)&uTemperature))
    {
        return 1;
    }
    
    //拼装数据
    sprintf((char *)uDisplayBuff, "%d-%.2d-%.2d %.2d:%.2d:%.2d\r\nname: %s\r\nweather: %s\r\ntemp: %s\r\n\r\n", 
                                   RTC_GetTimeAddr()->year, RTC_GetTimeAddr()->month, RTC_GetTimeAddr()->day,
                                   RTC_GetTimeAddr()->hour, RTC_GetTimeAddr()->min, RTC_GetTimeAddr()->sec,
                                   uName, uText, uTemperature);
    UART_BlockSendStr(WFC_DEBUG_UART_NODE, uDisplayBuff);
    
    return 0;
}


/**
  * @brief  基时钟获取
  * @param  None
  * @retval None
  */
uBit32 WFC_GetBaseClock(void)
{
    uBit8 uBaseClock[64] = {0};
    uBit32 ulBaseClock = 0;
    
    //解析数据包
    if (WFC_GetBaseClockForJson((char const *)m_uClockJsonPack, "timestamp", (char *)&uBaseClock))
    {
        return 0;
    }
    
    ulBaseClock = atol((char const *)uBaseClock);
    
    RTC_SetCurClock(ulBaseClock, 8);
    
    UART_BlockSendStr(0, uBaseClock);
    
    return ulBaseClock;
}




/*****************************************************************************
 * JSON解包测试相关线程接口
 ****************************************************************************/
#define WFC_WIFI_TOGGLE_TIME          (1000)       //LED翻转时间(MS)
static SYS_TIME_DATA m_WifiCtrlTimer  = {1};     //LED控定时器


//解包测试
//长时间测试,以确定是否有内存溢出
void WFC_UnpackTest(void)
{
    if (SysTime_CheckExpiredState(&m_WifiCtrlTimer))
    {
        SysTime_StartOneShot(&m_WifiCtrlTimer, WFC_WIFI_TOGGLE_TIME); //设置下一次执行的时间
        
        RTC_Update();
        
        WFC_DisplayWeather();
    }
    
}

