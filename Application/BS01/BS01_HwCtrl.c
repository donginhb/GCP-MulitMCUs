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
#include "BS01_HwCtrl.h"
#include "BS01_HardwareDef.h"
#include "BS01_WifiCom.h"      

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
static void BS01_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D0, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D1, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D2, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D3, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D4, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D5, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D6, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_D7, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_EN, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_RW, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LCD_RS, false);

}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS01_HwInit(void)
{
    uBit32 ulRet = 0;
    
    //初始化IO
    BS01_IOConfig();
    
    //初始化LCD
    uBit32 ulPortNoGroup[] = {OUTPUT_IO_LCD_D0, OUTPUT_IO_LCD_D1, OUTPUT_IO_LCD_D2, OUTPUT_IO_LCD_D3,
                              OUTPUT_IO_LCD_D4, OUTPUT_IO_LCD_D5, OUTPUT_IO_LCD_D6, OUTPUT_IO_LCD_D7};
    
    LCD_Init(ulPortNoGroup, OUTPUT_IO_LCD_EN, OUTPUT_IO_LCD_RW, OUTPUT_IO_LCD_RS);
    LCD_WriteStr(0, 0, "                ");
    LCD_WriteStr(1, 0, "                ");
    LCD_WriteStr(0, 0, "WIFI CLOCK INIT");
    
#if 0
    //初始化调试串口
    UART_Init(BS01_DEBUG_UART_NODE, 115200);
#endif
    
    //初始化ESP8266
    ulRet = BS01_InitWifi(BS01_ESP_UART_NODE);
    LCD_WriteStr(0, 0, "                ");
    LCD_WriteStr(1, 0, "                ");
    LCD_WriteStr(0, 0, "ESP82XX:");
    
    switch (ulRet)
    {
    case BS01_WIFI_ERR_SUCCESS: LCD_WriteStr(1, 0, "Success!"); break;
    case BS01_WIFI_ERR_COM    : LCD_WriteStr(1, 0, "Comunication Err!"); break;
    case BS01_WIFI_ERR_SET    : LCD_WriteStr(1, 0, "Set Config Err!"); break;
    case BS01_WIFI_RER_CONNET : LCD_WriteStr(1, 0, "WIFI Connect Err!"); break;
    default:break;
    }
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS01_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS01_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS01_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
    }

}

