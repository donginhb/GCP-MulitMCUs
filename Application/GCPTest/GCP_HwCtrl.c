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
#include "GCP_HwCtrl.h"
#include "GCP_StateMan.h"
#include "GCP_ErrorDef.h"
#include "GCP_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysCtrl/SysUpdate/SysUpdate.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/TIME/TimeCapture.h"

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
static void GCP_IOConfig(void)
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
void GCP_HwInit(void)
{
    //初始化IO
    GCP_IOConfig();
    
    //初始化串口
    UART_Init(0, 115200);
    
    //初始化输出端口
    PWM_Init(PWM_NODE_0, true);
    PWM_OutputEnable(PWM_NODE_0, true);
    PWM_SetOutputPwmFrq(PWM_NODE_0, 10);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define GCP_LED_TOGGLE_TIME             (100)           //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void GCP_ShowMainWorkLed(void)
{
    static bool s_bLedState = false;
    
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, s_bLedState ? GCP_LED_TOGGLE_TIME : (GCP_LED_TOGGLE_TIME*5)); //设置下一次执行的时间
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
        
        s_bLedState = !s_bLedState;
    }

}

