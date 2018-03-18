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
#include "BS04_HwCtrl.h"
#include "BS04_HardwareDef.h"
//#include "BS04_RfCom.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/TIME/TimeCapture.h"
#include "SysPeripheral/IRQ/IRQ_Man.h"

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
static void BS04_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //输出逻辑翻转
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUT, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUT, false);
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS04_HwInit(void)
{
    //初始化IO
    BS04_IOConfig();
    
#if 0
    //初始化RF通信
    RF_InitCom();
#endif
    
    //使能IR捕获
    BS04_InitIR();
    
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS04_LED_TOGGLE_TIME             (500)           //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer = {1};              //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS04_ShowMainWorkLed(void)
{
    //static bool s_bBeepStatus = false;
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS04_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
    }

}

/*****************************************************************************
 * 人体红外探测线程接口
 ****************************************************************************/

#define BS04_HD_TOGGLE_TIME             (100)               //LED翻转时间(MS)
static SYS_TIME_DATA m_HumanDetectCtrlTimer = {1};          //LED控定时器

/**
  * @brief  人体检测线程
  * @param  None
  * @retval None
  */
void BS04_HumanDetectHandler(void)
{
    if (SysTime_CheckExpiredState(&m_HumanDetectCtrlTimer))
    {
        SysTime_StartOneShot(&m_HumanDetectCtrlTimer, BS04_HD_TOGGLE_TIME); //设置下一次执行的时间
        
        if (GPIO_MAN_GetInputPinState(INPUT_IO_HUMAN_DETECT))
        {
            GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUT, true);
        }
        else 
        {
            GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUT, false);
        }
        
    }
    
    
}


/*****************************************************************************
 * 红外遥控接收线程接口
 ****************************************************************************/

#define TIME_CAP_NODE       (2)             //捕获定时器节点
#define TIME_CAP_CHANNEL    (0)             //捕获定时器通道

static uBit32 m_ulCaptureValue = 0;         //捕获值
static bool m_bCaptureFinishFlag = false;   //捕获完成标志


//捕获中断回调(高电平时间捕获测试)
//由软件导致的延迟,大约在10us左右
static void BS04_CAP_CallbackHandler(void)
{
    static bool s_bCapRisingEdge = true;    //判断当前是由哪种边沿触发的捕获 true为上升沿 false为下降沿
    
    //判断中断入口
    if (TIME_CAP_GetCaptureIRQFlag(TIME_CAP_NODE, TIME_CAP_CHANNEL))
    {
        //清标志位
        TIME_CAP_ClearCaptureIRQFlag(TIME_CAP_NODE, TIME_CAP_CHANNEL);
        
        if (s_bCapRisingEdge == false)
        {
            //下降沿获取捕获值
            m_ulCaptureValue = TIME_CAP_GetCaptureValue(TIME_CAP_NODE, TIME_CAP_CHANNEL);
            m_bCaptureFinishFlag = true;
        }
        else 
        {
            //上升沿复位捕获值
            TIME_CAP_ResetCaptureValue(TIME_CAP_NODE, TIME_CAP_CHANNEL);
        }
        
        //设置下次的捕获边缘
        TIME_CAP_SetCaptureEdge(TIME_CAP_NODE, TIME_CAP_CHANNEL, 
                                s_bCapRisingEdge ? TIM_CAP_EDGE_FALLING : TIM_CAP_EDGE_RISING);
        
        s_bCapRisingEdge = !s_bCapRisingEdge;
    }
    
}


/**
  * @brief  红外遥控接收初始化
  * @param  None
  * @retval None
  */
void BS04_InitIR(void)
{
    //设置中断回调
    IRQ_SetTrgCallback(BS04_CAP_CallbackHandler, IRQ_TRG_TIM3);
    
    //初始化捕获端口
    TIME_CAP_InitCapture(TIME_CAP_NODE, 0x1<<TIME_CAP_CHANNEL, TIM_CAP_EDGE_RISING);    //初始化捕获(捕获上升沿)
    TIME_CAP_EnableCaptureIRQ(TIME_CAP_NODE, TIME_CAP_CHANNEL, true);                   //使能捕获中断
    TIME_CAP_EnableCapture(TIME_CAP_NODE, true);                                        //使能捕获
    
}


/**
  * @brief  红外遥控接收处理
  * @param  None
  * @retval None
  */
void BS04_IRHandler(void)
{
    if (m_bCaptureFinishFlag)
    {
        (void)m_ulCaptureValue;
    }
    
    
}

