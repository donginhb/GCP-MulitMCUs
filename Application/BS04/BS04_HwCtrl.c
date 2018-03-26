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
#include "SysPeripheral/TIME/TimeReckon.h"
#include "SysPeripheral/IRQ/IRQ_Man.h"
#include "SysPeripheral/EXTI/EXTI.h"


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

#define BS04_HD_OVER_TIME               (10000)             //人体红外超时时间
static SYS_TIME_DATA m_HumanDetectCtrlTimer = {1};          //人体红外检测定时器

/**
  * @brief  人体检测线程
  * @param  None
  * @retval None
  */
void BS04_HumanDetectHandler(void)
{
    
    if (GPIO_MAN_GetInputPinState(INPUT_IO_HUMAN_DETECT))
    {
        //在定时器尚未启动的时候启动计时
        if (!m_HumanDetectCtrlTimer.ulEnable)
        {
            SysTime_StartOneShot(&m_HumanDetectCtrlTimer, BS04_HD_OVER_TIME); //设置下一次执行的时间
        }
    }
    else 
    {
        //低电平清除定时器
        SysTime_Cancel(&m_HumanDetectCtrlTimer);
    }
    
    if (SysTime_CheckExpiredState(&m_HumanDetectCtrlTimer))
    {
        //启动定时器
        BS04_BeepEnable();
        
        //清除定时器
        SysTime_Cancel(&m_HumanDetectCtrlTimer);
    }
    
}


/*****************************************************************************
 * 蜂鸣器管理线程接口
 ****************************************************************************/

#define BS04_BEEP_OVER_TIME               (3000)            //蜂鸣器超时时间
static SYS_TIME_DATA m_BeepCtrlTimer = {0};                 //蜂鸣器检测定时器


/**
  * @brief  蜂鸣器使能
  * @param  None
  * @retval None
  */
void BS04_BeepEnable(void)
{
    //使能蜂鸣器
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_BEEP, true);
    
    //启动超时定时器
    SysTime_StartOneShot(&m_BeepCtrlTimer, BS04_BEEP_OVER_TIME); 
    
}


/**
  * @brief  蜂鸣器管理
  * @param  None
  * @retval None
  */
void BS04_Handler(void)
{
    if (SysTime_CheckExpiredState(&m_BeepCtrlTimer))
    {
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_BEEP, false);
    }
    
}


/*****************************************************************************
 * 红外遥控接收线程接口
 ****************************************************************************/

#define TIME_REK_NODE       (1)             //捕获定时器节点

//红外编码逻辑
#define GUIDANCE_CODE       (4500)          //引导码:4.50ms高电平
#define LOGIC_1             (1680)          //逻辑1: 1.68ms高电平
#define LOGIC_0             ( 560)          //逻辑0: 0.56ms高电平
#define CONTINUOUS_CODE     (2200)          //连发码:2.20ms高电平


static struct
{
  uBit8 m_Status;
  uBit32 m_Code;
}IrRemote = {0};


//捕获中断回调(高电平时间捕获测试)
//由软件导致的延迟,大约在10us左右
static void BS04_EXTI_CallbackHandler(void)
{
    static bool s_bCapRisingEdge = true;    //判断当前是由哪种边沿触发的捕获 true为上升沿 false为下降沿
    
    static uBit8 cIrRmtSta = 0;   //红外遥控状态(0XAA/0)
    static uBit8 cCodeCount = 0;  //脉冲计数
    static uBit32 iIrRmtCode = 0; //数据: 数据位+数据反码+地址位+地址反码
    uBit16 nIrTime = 0;
    
    //获取当前的IO状态
    s_bCapRisingEdge = GPIO_GetInputState(INPUT_IO_IR_DETECT) ? true : false;
    
    if (s_bCapRisingEdge == false)
    {
        //下降沿停止计时
        TIME_REK_Stop(TIME_REK_NODE);
        nIrTime = TIME_REK_GetValue(TIME_REK_NODE);
        TIME_REK_ClearValue(TIME_REK_NODE);
        
        //是否引导码
        if ((nIrTime > GUIDANCE_CODE-500) && (nIrTime < GUIDANCE_CODE+500))
        {
            cIrRmtSta = 0xAA;
            
        }
        else
        {
            if (cIrRmtSta == 0xAA)  //已接收到引导码
            {
                if ((nIrTime > LOGIC_1-300) && (nIrTime < LOGIC_1+300)) //逻辑1
                {
                    iIrRmtCode |= (0x1<<(cCodeCount));
                    cCodeCount++;
                }
                else if ((nIrTime > LOGIC_0-300) && (nIrTime < LOGIC_0+300))  //逻辑0
                {
                    iIrRmtCode &= ~(0x1<<(cCodeCount));
                    cCodeCount++;
                }
                else ;
            }
            
            if (cCodeCount == 32)
            {
                cIrRmtSta = 0;
                cCodeCount = 0;
                
                IrRemote.m_Status = 1;
                IrRemote.m_Code = (iIrRmtCode>>16) & 0x00FF;
            }
        }
        
    }
    else 
    {
        //上升沿开始计时
        TIME_REK_Start(TIME_REK_NODE);
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
    IRQ_SetTrgCallback(BS04_EXTI_CallbackHandler, IRQ_TRG_EXTI6);
    
    //设置中断捕获模式
    EXTI_Init(INPUT_IO_IR_DETECT, EXTI_TRG_RISING|EXTI_TRG_FALLING);
    
    //设置计时定时器
    TIME_REK_Init(TIME_REK_NODE);
    
}


/**
  * @brief  红外遥控接收处理
  * @param  None
  * @retval None
  */
void BS04_IRHandler(void)
{
    if (IrRemote.m_Status)
    {
        IrRemote.m_Status = 0;
        
        switch (IrRemote.m_Code)
        {
        case 64:    //开灯/关灯
            GPIO_ToggleOutputState(OUTPUT_IO_OUT);
            break;
        default: break;
        }
    }
    
    
}




