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
#include "VM_HwCtrl.h"
#include "VM_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/PWM/PWM_MotorCtrl.h"
#include "MotorCtrl/MotorCtrl.h"

#include "ExtPeripheral/NixieTube/NixieTube.h"

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
static void VM_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //先单独初始化IO使能脚
    GPIO_InitIOGroup(&g_GcpIOTable.pOutputGroup[OUTPUT_IO_OE1_N], 4, GPIO_MODE_OUPUT);  //模式配置
    
    //设置IO使能脚逻辑翻转
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE4_N, true);
    
    //在初始化其他IO之前,禁止IO输出
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, false);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //对输出IO进行逻辑翻转
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT10, true);
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT11, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT12, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT13, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT14, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT15, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT16, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT17, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT18, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT19, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT20, true);
    //7个通用OUTPUT
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT21, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT22, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT23, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT24, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT25, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT26, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT27, true);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT10, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT12, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT13, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT14, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT15, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT16, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT17, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT18, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT19, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT20, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT21, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT22, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT23, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT24, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT25, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT26, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT27, false);
    
    //使能IO
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, true);
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void VM_HwInit(void)
{
    //初始化IO
    VM_IOConfig();
    
    //初始化轴
    AXIS_Init(0, 3, 1, 0, 0, OUTPUT_IO_MOTOR_DIR);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define VM_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void VM_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, VM_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
        
#if 0
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT1);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT2);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT3);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT4);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT5);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT6);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT7);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT8);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT9);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_ROW_OUTPUT10);
        
        
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT11);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT12);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT13);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT14);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT15);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT16);
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT17);   //P1.22
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT18);   //P1.21
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT19);   //P1.19
        GPIO_ToggleOutputState(OUTPUT_IO_MOTRO_COL_OUTPUT20);   //P1.18
        
        
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT21);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT22);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT23);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT24);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT25);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT26);
        GPIO_ToggleOutputState(OUTPUT_IO_OUTPUT27);
#endif
    }

}


