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
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/PWM/PWM_MotorCtrl.h"
#include "SysPeripheral/ADC/ADC.h"
#include "SysPeripheral/CAN/Can.h"
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
static void GCP_IOConfig(void)
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
void GCP_HwInit(void)
{
    //初始化IO
    GCP_IOConfig();
    
    //初始化轴
    AXIS_Init(0, 3, 1, 0, 0, OUTPUT_IO_MOTOR_DIR);
    
    //设置Bootloader LED灯
    SYS_UPDATE_SetMainWorkLed(g_GcpIOTable.pOutputGroup[OUTPUT_IO_LED_RUN1].nPort, 
                              g_GcpIOTable.pOutputGroup[OUTPUT_IO_LED_RUN1].nPin);
    
    //初始化温度数据采集
    ADC_Init(0, GCP_TEMP_SAMPLING_CH_MASK, false);
    ADC_Start(0);
    
    //初始化CAN
    CAN_Init(GCP_CAN_DEF_NODE, GCP_CAN_DEF_BAUDRATE);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define GCP_LED_TOGGLE_TIME                  (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer       = {1};     //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void GCP_ShowMainWorkLed(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, GCP_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
        
    }

}


/*****************************************************************************
 * 温度采集线程接口
 ****************************************************************************/
#define GCP_TEMP_SAMPLING_TIME               (100)           //温度采集间隔
static SYS_TIME_DATA m_SamplingCtrlTimer  = {1};            //温度采样控制定时器

static uBit16 m_vm_mTempValue = 0;                          //温度采样值

/**
  * @brief  温度数据采集
  * @param  None
  * @retval None
  */
void GCP_SamplingTempValue(void)
{
    if (SysTime_CheckExpiredState(&m_SamplingCtrlTimer))
    {
        uBit16 nTempValue = 0;
        if (ADC_RecvValueNoBlock(0, GCP_TEMP_SAMPLING_CH, &nTempValue) == 0)
        {
            //存储当前温度采集值
            m_vm_mTempValue = nTempValue;
            
            //设置下一次执行的时间
            SysTime_StartOneShot(&m_SamplingCtrlTimer, GCP_TEMP_SAMPLING_TIME);
            
            //开始采集
            ADC_Start(0);
        }
    }

}


/**
  * @brief  温度采集数据获取
  * @param  None
  * @retval 采集到的温度数据(原始AD值,未作转换)
  */
uBit32 GCP_GetTempSamplingValue(void)
{
    
    return m_vm_mTempValue;
}

