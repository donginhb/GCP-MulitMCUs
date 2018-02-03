/**
  ******************************************************************************
  * @file    GCMachine.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.23
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
#include "GCP_MainProc.h"
#include "GCP_HwCtrl.h"
#include "GCP_HardwareDef.h"
#include "GCP_MotorCtrl.h"
#include "DataType/DataType.h"
#include "SysCtrl/SysConfig.h"
#include "SysCtrl/SYS_CmuConfig.h"

#include "SysPeripheral/CAN/Can.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/IRQ/IRQ_Man.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/TIME/TimeCapture.h"

#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define TIME_CAP_NODE       (PWM_NODE_3)    //捕获定时器节点
#define TIME_CAP_CHANNEL    (0)             //捕获定时器通道

static uBit32 m_ulCaptureValue = 0;         //捕获值
static bool m_bCaptureFinishFlag = false;   //捕获完成标志


//捕获中断回调(高电平时间捕获测试)
//由软件导致的延迟,大约在10us左右
static void GCP_CAP_Test(void)
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


//捕获初始化
static void GCP_CAP_Init(void)
{
    //设置中断回调
    IRQ_SetTrgCallback(GCP_CAP_Test, IRQ_TRG_TIME0);
    
    //初始化捕获端口
    TIME_CAP_InitCapture(TIME_CAP_NODE, 0x1<<TIME_CAP_CHANNEL, TIM_CAP_EDGE_RISING);    //初始化捕获(捕获上升沿)
    TIME_CAP_EnableCaptureIRQ(TIME_CAP_NODE, TIME_CAP_CHANNEL, true);                   //使能捕获中断
    TIME_CAP_EnableCapture(TIME_CAP_NODE, true);                                        //使能捕获
    
}


/*****************************************************************************
 * 售货机主任务相关控制接口
 ****************************************************************************/

/**
  * @brief  售货机初始化
  * @param  None
  * @retval None
  */
void GCP_Init(void)
{
    //初始化定时器捕获中断回调
    GCP_CAP_Init();
    
    //初始化硬件
    GCP_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void GCP_MainProc(void)
{
    //LED显示
    GCP_ShowMainWorkLed();
    
    if (m_bCaptureFinishFlag)
    {
        m_bCaptureFinishFlag = false;
        
        //发送数据
        uBit8 ulSBuff[64] = {0};
        sprintf((char *)ulSBuff, "Cap:%d\r\n", m_ulCaptureValue);
        UART_BlockSendStr(0, ulSBuff);
    }
}
