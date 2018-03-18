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
#include "BS05M_HwCtrl.h"
#include "BS05M_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/ADC/ADC.h"
#include "SysPeripheral/PWM/PWM.h"

      
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
static void BS05M_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //设置IO逻辑翻转
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_LED_SUM, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_LED_HUMAN, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_LED_AUTO, true);
    //GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_LED_OUT, true);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_SUM, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_HUMAN, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_AUTO, false);
    //GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_OUT, false);
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void BS05M_HwInit(void)
{
    //初始化IO
    BS05M_IOConfig();
    
    //初始化串口
    UART_Init(BS05M_COM_UART_NODE, 9600);
    
    //设置ADC采样通道
    ADC_Init(BS05M_LIGHT_ADC_NODE, 0x1<<BS05M_LIGHT_ADC_CH, true);
    ADC_Start(BS05M_LIGHT_ADC_NODE);
    
    //初始化步进电机PWM输出通道
    PWM_Init(BS05M_MOTOR_PWM_NODE, BS05M_MOTOR_PWM_CH_MASK);
    PWM_SetOutputPwmDutyRatio(BS05M_MOTOR_PWM_NODE, BS05M_MOTOR_PWM_CH_MASK, 100);
    PWM_SetOutputPwmFrq(BS05M_MOTOR_PWM_NODE, 10000);
    PWM_OutputEnable(BS05M_MOTOR_PWM_NODE, true);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define BS05M_LED_TOGGLE_TIME          (100)     //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void BS05M_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, BS05M_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
    }

}


/*****************************************************************************
 * 光照强度检测处理线程接口
 ****************************************************************************/
#define BS05M_ADC_SAMPLE_TIME        (100)       //ADC采样间隔时间
#define BS05M_ADC_TRIGGER_VALUE      (2000)      //ADC触发值
static SYS_TIME_DATA m_AdcCtrlTimer  = {1};     //ADC控定时器

static bool m_SumLightValidFalg = false;        //光照强度有效标志(低光照时有效)

/**
  * @brief  光照强度检测线程接口
  * @param  None
  * @retval None
  */
void BS05M_SumLightDetectHandler(void)
{
    if (SysTime_CheckExpiredState(&m_AdcCtrlTimer))
    {
        uBit16 nAdcValue = 0;
        
        //获取上一次采样的结果
        if (ADC_RecvValueNoBlock(BS05M_LIGHT_ADC_NODE, BS05M_LIGHT_ADC_CH, &nAdcValue) == 0)
        {
            //设置下一次执行的时间
            SysTime_StartOneShot(&m_AdcCtrlTimer, BS05M_ADC_SAMPLE_TIME);
            
#if 0
            //发送数据
            uBit8 uDisBuff[64] = {0};
            sprintf((char *)uDisBuff, "ADC:%d\r\n", nAdcValue);
            UART_SendStr(BS05M_COM_UART_NODE, uDisBuff);
#endif 
            
            //亮度监控
            if (nAdcValue > BS05M_ADC_TRIGGER_VALUE)
            {
                m_SumLightValidFalg = false;
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_SUM, false);
            }
            else 
            {
                m_SumLightValidFalg = true;
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_SUM, true);
                
            }
            
            //亮度控制
            if (nAdcValue < 1000)
            {
                BS05M_CtrlLightStrength(95);
            }
            else if (nAdcValue < 1500)
            {
                BS05M_CtrlLightStrength(50);
            }
            else if (nAdcValue < 2000)
            {
                BS05M_CtrlLightStrength(40);
            }
            else 
            {
                BS05M_CtrlLightStrength(10);
            }
        }
    }

}


/*****************************************************************************
 * 人体红外检测处理线程接口
 ****************************************************************************/
#define BS05M_HUMAN_SAMPLE_TIME        (100)     //人体红外采样间隔时间
static SYS_TIME_DATA m_HumanCtrlTimer  = {1};   //人体红外检测控制定时器

static bool m_HumanDetectValidFlag = false;     //人体检测有效标志(检测到有人时触发)

/**
  * @brief  人体红外检测线程接口
  * @param  None
  * @retval None
  */
void BS05M_HumanDetectHandler(void)
{
    static uBit8 s_uOldHumanDetectState = 0xFF;        //人体红外触发状态
    
    if (SysTime_CheckExpiredState(&m_HumanCtrlTimer))
    {
        //设置下一次执行的时间
        SysTime_StartOneShot(&m_HumanCtrlTimer, BS05M_ADC_SAMPLE_TIME);
        
        //查询IO状态
        uBit8 uCurDetectState = GPIO_MAN_GetInputPinState(INPUT_IO_HUMAN_DETECT);
        
        if (uCurDetectState != s_uOldHumanDetectState)
        {
            if (uCurDetectState)    //检测到人体触发
            {
#if 0
                //发送数据
                uBit8 uDisBuff[64] = {0};
                sprintf((char *)uDisBuff, "Human Detect Trigger!\r\n");
                UART_SendStr(BS05M_COM_UART_NODE, uDisBuff);
#endif
                m_HumanDetectValidFlag = true;
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_HUMAN, true);
                
            }
            else    //检测不到人体触发
            {
#if 0
                //发送数据
                uBit8 uDisBuff[64] = {0};
                sprintf((char *)uDisBuff, "Human Detect UnTrigger!\r\n");
                UART_SendStr(BS05M_COM_UART_NODE, uDisBuff);
#endif
                m_HumanDetectValidFlag = false;
                GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_HUMAN, false);
            }
        }
        
        s_uOldHumanDetectState = uCurDetectState;
        
    }

}


/*****************************************************************************
 * 灯输出处理线程接口
 ****************************************************************************/

static bool m_bLightAutoOutputMode = true;               //灯光自动输出模式
static bool m_bLightEnableFlag = false;
static uBit8 m_uCurValue = 0;


/**
  * @brief  灯光输出控制接口
  * @param  None
  * @retval None
  */
void BS05M_CtrlLightStrength(uBit8 uDutyRatio)
{
    m_uCurValue = uDutyRatio;
    
    if (m_bLightEnableFlag)
    {
        PWM_SetOutputPwmDutyRatio(BS05M_MOTOR_PWM_NODE, BS05M_MOTOR_PWM_CH_MASK, 100-uDutyRatio);
    }
    
}


/**
  * @brief  灯光输出控制接口
  * @param  bIsEnable false-关闭 true-打开
  * @retval None
  */
void BS05M_EnableLignt(bool bIsEnable)
{
    m_bLightEnableFlag = bIsEnable;
    PWM_SetOutputPwmDutyRatio(BS05M_MOTOR_PWM_NODE, BS05M_MOTOR_PWM_CH_MASK, bIsEnable ? m_uCurValue : 100);
    
}


/**
  * @brief  灯光输出控制接口
  * @param  None
  * @retval None
  */
void BS05M_LightOutputHandler(void)
{
    if (m_bLightAutoOutputMode)
    {
        if (m_SumLightValidFalg && m_HumanDetectValidFlag)
        {
            //输出灯光
            BS05M_EnableLignt(true);
        }
        else 
        {
            //关闭灯光
            BS05M_EnableLignt(false);
        }
        
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_AUTO, true);
    }
    else 
    {
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_LED_AUTO, false);
    }
    
}


/**
  * @brief  灯光输出使能接口(仅在非自动模式下有效)
  * @param  bIsEnable false-关闭 true-使能
  * @retval None
  */
void BS05M_EnableLightOutput(bool bIsEnable)
{
    if (!m_bLightAutoOutputMode)
    {
        //控制灯光
        BS05M_EnableLignt(bIsEnable);
    }
    
}


/**
  * @brief  灯光输出自动模式使能
  * @param  bIsEnable false-关闭 true-使能
  * @retval None
  */
void BS05M_EnableAutoOutputMode(bool bIsEnable)
{
    m_bLightAutoOutputMode = bIsEnable;
    
}

