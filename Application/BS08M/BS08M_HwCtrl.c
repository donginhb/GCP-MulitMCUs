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
#include "BS08M_HwCtrl.h"
#include "BS08M_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"

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
static void BS08M_IOConfig(void)
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
void BS08M_HwInit(void)
{
    //初始化IO
    BS08M_IOConfig();
    
    //初始化LCD
    uBit32 ulPortNoGroup[] = {OUTPUT_IO_LCD_D0, OUTPUT_IO_LCD_D1, OUTPUT_IO_LCD_D2, OUTPUT_IO_LCD_D3,
                              OUTPUT_IO_LCD_D4, OUTPUT_IO_LCD_D5, OUTPUT_IO_LCD_D6, OUTPUT_IO_LCD_D7};
    
    LCD_Init(ulPortNoGroup, OUTPUT_IO_LCD_EN, OUTPUT_IO_LCD_RW, OUTPUT_IO_LCD_RS);
    LCD_WriteStr(0, 0, "                ");
    LCD_WriteStr(1, 0, "                ");
    LCD_WriteStr(0, 0, "NRF COM...");
    
    //初始化串口
    
    UART_Init(BS08M_COM_UART_NODE, 9600);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS08M_LED_TOGGLE_TIME          (100)    //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS08M_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS08M_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
    }

}


/*****************************************************************************
 * 温度控制接口接口
 ****************************************************************************/
#define BS08M_MAX_TEMP_VALUE        (6000)  //60度(放大因子:100)

/**
  * @brief  温度值设置
  * @param  ulTempValue 温度值(放大因子:100)
  * @retval 0-成功 非0-失败
  */
uBit32 BS08M_SetTempValue(uBit32 ulTempValue)
{
    uBit8 uDisplayBuff[32] = {0};
    
    sprintf((char *)uDisplayBuff, "Temp: %d.%.2d", ulTempValue/100, ulTempValue%100);
    
    //输出温度值
    LCD_WriteStr(0, 0, "                ");
    LCD_WriteStr(1, 0, "                ");
    LCD_WriteStr(0, 0, uDisplayBuff);
    
    if (ulTempValue >= BS08M_MAX_TEMP_VALUE)
    {
        LCD_WriteStr(1, 0, "Status: Over!");
    }
    else 
    {
        LCD_WriteStr(1, 0, "Status: Normal");
    }
    
    return 0;
}



