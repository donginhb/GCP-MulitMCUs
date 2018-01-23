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
#include "IRB_HwCtrl.h"
#include "IRB_HardwareDef.h"
#include "IRB_MotorCtrl.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/KEY/KEY.h"

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
static void IRB_IOConfig(void)
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
void IRB_HwInit(void)
{
    //初始化IO
    IRB_IOConfig();
    
    //初始化串口
    UART_Init(IRB_COM_UART_NODE, 115200);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define IRB_LED_TOGGLE_TIME             (100)            //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void IRB_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, IRB_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
    }

}
