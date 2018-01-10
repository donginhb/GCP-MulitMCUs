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

#include "ExtPeripheral/WIFI/ESP82XX.h"
#include "ExtPeripheral/LCD/LCD.h"
      
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
    
    //初始化LCD
    uBit32 ulPortNoGroup[] = {OUTPUT_IO_LCD_D0, OUTPUT_IO_LCD_D1, OUTPUT_IO_LCD_D2, OUTPUT_IO_LCD_D3,
                              OUTPUT_IO_LCD_D4, OUTPUT_IO_LCD_D5, OUTPUT_IO_LCD_D6, OUTPUT_IO_LCD_D7};
    
    LCD_Init(ulPortNoGroup, OUTPUT_IO_LCD_EN, OUTPUT_IO_LCD_RW, OUTPUT_IO_LCD_RS);
    
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

