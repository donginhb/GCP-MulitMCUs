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
#include "BMC_HwCtrl.h"
#include "BMC_HardwareDef.h"
#include "BMC_MotorCtrl.h"

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
static void BMC_IOConfig(void)
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
void BMC_HwInit(void)
{
    //初始化IO
    BMC_IOConfig();
    
#if 0
    //初始化按键
    uBit32 ulKeyPinGourp[] = {INPUT_IO_KEY1, INPUT_IO_KEY2};
    KEY_SetScanPinGroup(ulKeyPinGourp, sizeof(ulKeyPinGourp)/sizeof(ulKeyPinGourp[0]));
#endif
    
    //初始化串口
    UART_Init(BMC_COM_UART_NODE, 115200);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BMC_LED_TOGGLE_TIME             (100)            //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BMC_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BMC_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
    }

}


/*****************************************************************************
 * 按键扫描线程接口
 ****************************************************************************/
#if 0
#define BMC_KEY_SCAN_INTERVAL       (50)         //按键扫描间隔(MS)
static  SYS_TIME_DATA m_KeyScanTimer = {1};     //扫描定时器

/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void BMC_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_KeyScanTimer))
    {
        SysTime_StartOneShot(&m_KeyScanTimer, BMC_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        if (ulCurTrg == 1)
        {
            BMC_SetMotorDirect(!BMC_GetMotorDirect());
        }
        else if (ulCurTrg == 2)
        {
            BMC_EnableMotor(!BMC_GetMotorEnableStatue());
        }
    }
      
}
#endif
