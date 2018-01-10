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
#include "ExtPeripheral/LCD/LCD.h"
#include "Algorithm/Json/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * JSON数据包相关解析接口
 ****************************************************************************/

/**
  * @brief  天气获取(从JSON数据包中获取想要的数据)
  * @param  pJsonPack JSON数据包字符串
  * @param  pKey 要提取的秘钥
  * @param  pValue 提取到的数据
  * @retval 0-成功 非0-失败
  */
static uBit32 WFC_GetWeatherParmForJson(char const *pJsonPack, char *pKey, char *pValue)
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
  * @brief  基北京时间获取(从JSON数据包中获取想要的数据)
  * @param  pJsonPack JSON数据包字符串
  * @param  pKey 要提取的秘钥
  * @param  pValue 提取到的数据
  * @retval 0-成功 非0-失败
  */
static uBit32 WFC_GetBaseClockForJson(char const *pJsonPack, char *pKey, char *pValue)
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


/*****************************************************************************
 * 网络参数接口获取解析相关接口
 ****************************************************************************/
#define WFC_CLOCK_UPDATE_TIME          (24 * 60 * 60 * 1000)    //更新时间(MS)
static SYS_TIME_DATA m_UpdateClockTimer  = {0};    //网络更新定时器

static uBit8 m_uWeatherJsonPack[512] = {0}; //天气JSON包(字符串)
static uBit32 m_ulWeatherJsonLen = 0;       //天气JSON包长度

static uBit8 m_uClockJsonPack[512] = {0};   //北京时间JSON包(字符串)
static uBit32 m_ulClockJsonLen = 0;         //北京时间JSON包长度


static uBit8 m_uCityName[64] = {0};         //城市名
static uBit8 m_uWeather[64] = {0};          //天气
static uBit8 m_uTemp[64] = {0};             //温度值
static uBit8 m_uBaseClock[64] = {0};        //基时钟


/**
  * @brief  天气更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateWeather(void)
{
    //从网络中获取最新的JSON包
    uBit32 ulRet = WFC_RecvWeatherPack(m_uWeatherJsonPack, &m_ulWeatherJsonLen);
    
    //判断获取结果
    if (ulRet == 0)
    {
        //解析数据包并提取参数
        if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "name", (char *)&m_uCityName))
        {
            return 1;
        }
        
        if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "text", (char *)&m_uWeather))
        {
            return 1;
        }
        
        if (WFC_GetWeatherParmForJson((char const *)m_uWeatherJsonPack, "temperature", (char *)&m_uTemp))
        {
            return 1;
        }
    }
    
    return ulRet;
}


/**
  * @brief  时间更新
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 WFC_UpdateClock(void)
{
    //从网络中获取最新的JSON包
    uBit32 ulRet = WFC_RecvClockPack(m_uClockJsonPack, &m_ulClockJsonLen);
    
    //判断获取结果
    if (ulRet == 0)
    {
        //解析数据包
        if (WFC_GetBaseClockForJson((char const *)m_uClockJsonPack, "timestamp", (char *)&m_uBaseClock))
        {
            return 1;
        }
        
        //字符串转整型
        uBit32 ulBaseClock = atol((char const *)m_uBaseClock);
        
        //设置RTC
        RTC_SetCurClock(ulBaseClock, 8);
        
        //设置下次网络同步的时间
        SysTime_StartOneShot(&m_UpdateClockTimer, WFC_CLOCK_UPDATE_TIME); 
        
    }
    
    return ulRet;
}


/**
  * @brief  时间更新管理
  * @param  None
  * @retval None
  */
void WFC_UpdateClockHandler(void)
{
    //计算下次网络时间同步的时间,当时间到达,则自动从网络更新时间和天气
    if (SysTime_CheckExpiredState(&m_UpdateClockTimer))
    {
        //更新网络时间
        WFC_UpdateClock();
        
        //更新网络天气
        WFC_UpdateWeather();
    }
    
}


/*****************************************************************************
 * WIFI时钟显示相关接口
 ****************************************************************************/
#define WFC_WIFI_TOGGLE_TIME          (1000)    //翻转时间(MS)
static SYS_TIME_DATA m_DisplayTimer  = {1};    //定时器


/**
  * @brief  WIFI 时钟参数显示
  * @param  None
  * @retval None
  */
static void WFC_DisplayParm(void)
{
    static uBit8 uDisplayStep = 0;
    uBit8 uDisplayBuff[128] = {0};
    
    //显示时钟 格式: 20:54:00  
    sprintf((char *)uDisplayBuff, "%.2d:%.2d:%.2d", RTC_GetTimeAddr()->hour, RTC_GetTimeAddr()->min, RTC_GetTimeAddr()->sec);
    LCD_WriteStr(1, 0, uDisplayBuff);
    
    switch (uDisplayStep%4)
    {
    case 0: //显示日期
        memset(uDisplayBuff, 0, sizeof(uDisplayBuff));
        sprintf((char *)uDisplayBuff, "%d-%.2d-%.2d", RTC_GetTimeAddr()->year, RTC_GetTimeAddr()->month, RTC_GetTimeAddr()->day);
        LCD_WriteStr(0, 0, uDisplayBuff);
        break;
        
    case 1: //显示城市
        memset(uDisplayBuff, 0, sizeof(uDisplayBuff));
        sprintf((char *)uDisplayBuff, "City: %s",  m_uCityName);
        LCD_WriteStr(0, 0, uDisplayBuff);
        break;
        
    case 2: //显示天气
        memset(uDisplayBuff, 0, sizeof(uDisplayBuff));
        sprintf((char *)uDisplayBuff, "Weather: %s",  m_uWeather);
        LCD_WriteStr(0, 0, uDisplayBuff);
        break;
        
    case 3: //显示温度
        memset(uDisplayBuff, 0, sizeof(uDisplayBuff));
        sprintf((char *)uDisplayBuff, "Temp: %s",  m_uTemp);
        LCD_WriteStr(0, 0, uDisplayBuff);
        break;
        
    default: uDisplayStep = 0; break;
    }
    
    uDisplayStep++;
    
}


/**
  * @brief  更新显示数据
  * @param  None
  * @retval None
  */
void WFC_UpdateDisplay(void)
{
    if (SysTime_CheckExpiredState(&m_DisplayTimer))
    {
        //设置下一次执行的时间
        SysTime_StartOneShot(&m_DisplayTimer, WFC_WIFI_TOGGLE_TIME); 
        
        //更新RTC时钟
        RTC_Update();
        
        //显示
        WFC_DisplayParm();
    }
    
}


