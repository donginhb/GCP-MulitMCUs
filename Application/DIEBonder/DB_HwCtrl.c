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
#include "DB_HwCtrl.h"
#include "DB_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/PWM/PWM_MotorCtrl.h"

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
static void DB_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
#if 0
    //翻转按键的电平逻辑
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY0, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY1, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY2, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY3, 1);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_KEY4, 1);
#endif
    
}



/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

void HAL_PWM_CountInit(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit8 uPwmCapNode, uBit8 uCapChNum);
void HAL_PWM_SendCount(uBit8 uPwmOuputNode, uBit8 uOuputChNum, uBit32 ulPwmCount);


/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void DB_HwInit(void)
{
    //初始化IO
    DB_IOConfig();
    
#if 0
    //初始化按键
    uBit32 ulKeyPinGourp[] = {INPUT_IO_KEY2};
    KEY_SetScanPinGroup(ulKeyPinGourp, sizeof(ulKeyPinGourp)/sizeof(ulKeyPinGourp[0]));
#endif
    
    //UART_Init(0, 115200);
    
#if 0
    PWM_Init(3, 2);
    PWM_OutputEnable(3, true);
    PWM_SetOutputPwmFrq(3, 20*1000);
#endif
    
#if 0
    PWM_MotorPulseInit(0, 0, 1, 0);
#else
    PWM_MotorPulseInit(3, 1, 0, 0);
#endif
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define DB_LED_TOGGLE_TIME          (1)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void DB_MainWorkLedShow(void)
{
    static uBit32 s_ulMotorPulse = 0;
    
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, DB_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
#if 0
        PWM_SendPulse(0,  10);
#else
        PWM_SendPulseLimitMs(3, 10);
#endif
        s_ulMotorPulse += 1;
    }

}


/*****************************************************************************
 * 按键处理线程接口
 ****************************************************************************/
#define DB_KEY_SCAN_INTERVAL       (50)        //按键扫描间隔(MS)
static  SYS_TIME_DATA m_ScanTimer = {1};        //扫描定时器


/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void DB_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_ScanTimer))
    {
        SysTime_StartOneShot(&m_ScanTimer, DB_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
#if 0
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        if (ulCurTrg)
        {
            //GPIO_ToggleOutputState(OUTPUT_IO_LED1);
        }
#endif
        
    }
      
}

/*****************************************************************************
 * 按键处理线程接口
 ****************************************************************************/

//电机控制接口
static void DB_MotorCtrl(void)
{
    static uBit32 ulPulse = 0;
    
    
        
}

