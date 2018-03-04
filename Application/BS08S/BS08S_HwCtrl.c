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
#include "BS08S_HwCtrl.h"
#include "BS08S_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"

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
static void BS08S_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //配置温度传感器引脚为开漏引脚
    GPIO_InitIOGroup(&g_GcpIOTable.pOutputGroup[OUTPUT_IO_DS18B20_BUS], 1, GPIO_MODE_OD);  //开漏模式

}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS08S_HwInit(void)
{
    //初始化IO
    BS08S_IOConfig();

    //初始化串口
    UART_Init(BS08S_COM_UART_NODE, 9600);
    
    //初始化DS18B20
    SysTime_InitHwTimer(BS08S_DELAY_TIIME_NODE, true);
    TEMP_InitInterface(OUTPUT_IO_DS18B20_BUS, SysTime_DelayUs);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS08S_LED_TOGGLE_TIME          (100)    //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS08S_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS08S_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
        
    }

}


/*****************************************************************************
 * 温度传感器数据处理线程接口
 ****************************************************************************/
#define BS08S_SENSOR_SAMPLE_INTERVAL          (500)     //温度采样间隔间隔(MS)
static  SYS_TIME_DATA m_SensorSampleTimer = {1};        //扫描定时器

/**
  * @brief  温度传感器数据处理线程
  * @param  None
  * @retval None
  */
void BS08S_SensorSampleProc(void)
{
    static uBit8 s_uProStep = 0;    //工作步骤
    
    if (SysTime_CheckExpiredState(&m_SensorSampleTimer))
    {
        SysTime_StartOneShot(&m_SensorSampleTimer, BS08S_SENSOR_SAMPLE_INTERVAL);   //设置下一次执行的时间
        
#if 1
        switch (s_uProStep)
        {
        case 0: //启动温度转换
            {
                TEMP_StartConvert(); 
                s_uProStep++;
                break;
            }
        case 1: 
            {
                s_uProStep++;
                break;     
            }
        case 2: //获取温度值
            {
                Bit32 cTempValue = (Bit32)(TEMP_GetValue() * 100); 
                
                if ((cTempValue >= 0) && (cTempValue <= 125*100))
                {
                    uBit8 uDisBuff[128] = {0};
                    sprintf((char *)uDisBuff, (const char *)"TEMP:%d.", cTempValue);
                    UART_SendBuff(BS08S_COM_UART_NODE, uDisBuff, strlen((const char *)uDisBuff));
                    
                }
                
                s_uProStep++;   
                break;
            }
        case 3:
            {
                s_uProStep++;   
                break;
            }
        default: 
            {
                s_uProStep = 0;   
                break;
            }
        }
#endif
    }
    
}

