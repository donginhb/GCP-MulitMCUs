/**
  ******************************************************************************
  * @file    GCIOCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "WFC_HwCtrl.h"
#include "WFC_HardwareDef.h"
#include "WFC_WifiCom.h"      

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/EXTI/EXTI.h"
#include "SysPeripheral/UART/UART.h"

//#include "ExtPeripheral/EEPROM/EEPROM.h"
#include "ExtPeripheral/TempSensor/TempSensor.h"
#include "ExtPeripheral/WIFI/ESP82XX.h"
      
#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
static void WFC_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
#if 0
    
    //初始化IIC引脚
    //GPIO_InitIOGroup(&g_GcpIOTable.pOutputGroup[OUTPUT_IO_SCL], 2, GPIO_MODE_OD);  //开漏模式
    
    //初始化温度传感器应交
    //GPIO_InitIOGroup(&g_GcpIOTable.pOutputGroup[OUTPUT_IO_DS18B20_BUS], 1, GPIO_MODE_OD);  //开漏模式
    
    //翻转按键的电平逻辑
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY0, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY1, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY2, 1);
#endif
    
}



/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void WFC_HwInit(void)
{
    uBit32 ulRet = 0;
    
    //初始化IO
    WFC_IOConfig();
    
    //初始化串口
    UART_Init(WFC_UART_NODE, 115200);
    
    //初始化ESP8266
    ulRet = WFC_InitWifi(1);
    
    switch (ulRet)
    {
    case WFC_WIFI_ERR_SUCCESS: UART_SendStr(WFC_UART_NODE, "ESP82XX: Success!\r\n"); break;
    case WFC_WIFI_ERR_COM    : UART_SendStr(WFC_UART_NODE, "ESP82XX: Comunication Err!\r\n"); break;
    case WFC_WIFI_ERR_SET    : UART_SendStr(WFC_UART_NODE, "ESP82XX: Set Config Err!\r\n"); break;
    case WFC_WIFI_RER_CONNET : UART_SendStr(WFC_UART_NODE, "ESP82XX: WIFI Connect Err!\r\n"); break;
    default:break;
    }
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define WFC_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void WFC_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, WFC_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
    }

}


/*****************************************************************************
 * 按键处理线程接口
 ****************************************************************************/
#define WFC_KEY_SCAN_INTERVAL       (50)        //按键扫描间隔(MS)
static  SYS_TIME_DATA m_ScanTimer = {1};        //扫描定时器


/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void WFC_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_ScanTimer))
    {
        SysTime_StartOneShot(&m_ScanTimer, WFC_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        if (ulCurTrg)
        {
            GPIO_ToggleOutputState(OUTPUT_IO_LED1);
        }
        
    }
      
}


/*****************************************************************************
 * 传感器数据处理线程接口
 ****************************************************************************/
#define WFC_SENSOR_SAMPLE_INTERVAL          (500)        //温度采样间隔间隔(MS)
static  SYS_TIME_DATA m_SensorSampleTimer = {1};        //扫描定时器

/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void WFC_SensorSampleProc(void)
{
    static uBit8 s_uProStep = 0;    //工作步骤
    
    if (SysTime_CheckExpiredState(&m_SensorSampleTimer))
    {
        SysTime_StartOneShot(&m_SensorSampleTimer, WFC_SENSOR_SAMPLE_INTERVAL);   //设置下一次执行的时间
        
#if 1
        switch (s_uProStep)
        {
        case 0: //启动温度转换
            {
                TEMP_StartConvert(); 
                s_uProStep++;
                break;
            }
        case 1: //获取温度值
            {
                Bit32 TempValue = (Bit32)(TEMP_GetValue() * 10); 
                uBit8 uDisBuff[128] = {0};
                sprintf((char *)uDisBuff, (const char *)"TempValue: %d\r\n", TempValue);
                UART_SendBuff(WFC_UART_NODE, uDisBuff, strlen((const char *)uDisBuff));
                
                s_uProStep++;
                break;     
            }
        case 2: 
            {
                s_uProStep++;   
                break;
            }
        case 3:
            {
                s_uProStep++;   
                break;
            }
        default: 
            {
                s_uProStep = 0;   
                break;
            }
        }
#endif
    }
    
}


