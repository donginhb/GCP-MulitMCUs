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
#include "CN_HwCtrl.h"
#include "CN_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/ADC/ADC.h"
#include "SysPeripheral/EXTI/EXTI.h"

#include "HwDrivers/HW_Dac.h"

#include "ExtPeripheral/TempSensor/TempSensor.h"
      
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
static void CN_IOConfig(void)
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
void CN_HwInit(void)
{
    //初始化IO
    CN_IOConfig();
    
    //初始化串口
    UART_Init(0, 115200);
    
    //初始化AD采样值
    //ADC_Init(0, CN_TEMP_SAMPLING_CH_MASK, true);
    //ADC_Start(0);
    
    //初始化外部中断
    //EXTI_Init(0, EXTI_TRG_FALLING);
    
    //初始化DAC
    HW_DAC_Init();
    HW_DAC_UpdateByAnalogValue(1000);
    HW_DAC_UpdateByAnalogValue(2000);
    HW_DAC_UpdateByAnalogValue(3000);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define CN_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void CN_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, CN_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
        
    }

}


/*****************************************************************************
 * 温度采集线程接口
 ****************************************************************************/
#define CN_TEMP_SAMPLING_TIME               (100)           //温度采集间隔
static SYS_TIME_DATA m_SamplingCtrlTimer  = {1};            //温度采样控制定时器

static uBit16 m_vm_mTempValue1 = 0;                         //温度采样值
static uBit16 m_vm_mTempValue2 = 0;                         //温度采样值


/**
  * @brief  温度数据采集
  * @param  None
  * @retval None
  */
void CN_SamplingTempValue(void)
{
    if (SysTime_CheckExpiredState(&m_SamplingCtrlTimer))
    {
        uBit16 nTempValue = 0;
        if (ADC_RecvValueNoBlock(0, CN_TEMP_SAMPLING_CH1, &nTempValue) == 0)
        {
            //存储当前温度采集值
            m_vm_mTempValue1 = nTempValue;
        }
        if (ADC_RecvValueNoBlock(0, CN_TEMP_SAMPLING_CH2, &nTempValue) == 0)
        {
            //存储当前温度采集值
            m_vm_mTempValue2 = nTempValue;
        }
        
        //发送ADC值
        uBit8 uSBuff[64] = {0};
        sprintf((char *)uSBuff, "ADC:%u  %u\r\n", m_vm_mTempValue1, m_vm_mTempValue2);
        UART_SendStr(0, uSBuff);
    
        //设置下一次执行的时间
        SysTime_Start(&m_SamplingCtrlTimer, CN_TEMP_SAMPLING_TIME);
    }

}


/**
  * @brief  温度采集数据获取
  * @param  None
  * @retval 采集到的温度数据(原始AD值,未作转换)
  */
uBit32 CN_GetTempSamplingValue(void)
{
    
    return m_vm_mTempValue1;
}

