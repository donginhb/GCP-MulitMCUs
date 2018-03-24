/**
  ******************************************************************************
  * @file    BS03_MotorCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.05
  * @brief   VM Motor Ctrl Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "BS03_MotorCtrl.h"
#include "BS03_HardwareDef.h"
#include "DataType/DataType.h"

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <string.h>


/*****************************************************************************
 * 电机相关控制接口
 ****************************************************************************/
static  SYS_TIME_DATA m_MotorCtrlTimer = {1};   //电机控制定时器
static uBit32 m_md_ulMotorSpeedGroup[] = {2, 4, 6};//电机速度组
static uBit32 m_md_ulSpeedIndex = 5;            //电机速度索引
static uBit32 m_md_ulMotorDir = 1;              //电机方向控制
static uBit32 m_md_ulMotorEnable = 0;           //电机使能


/**
  * @brief  电机使能
  * @param  bIsEnable 电机使能位
  * @retval None
  */
void BS03_EnableMotor(bool bIsEnable)
{
    m_md_ulMotorEnable = bIsEnable;
    
}


/**
  * @brief  电机使能状态获取
  * @param  None
  * @retval 电机使能状态
  */
bool BS03_GetMotorEnableStatue(void)
{
    
    return m_md_ulMotorEnable;
}


/**
  * @brief  电机方向设置
  * @param  bDirect 电机方向位
  * @retval None
  */
void BS03_SetMotorDirect(bool bDirect)
{
    m_md_ulMotorDir = bDirect;
    
}


/**
  * @brief  电机方向获取
  * @param  None
  * @retval 电机方向
  */
bool BS03_GetMotorDirect(void)
{
    
    return m_md_ulMotorDir;
}


/**
  * @brief  电机速度设置
  * @param  ulSpeedIndex 速度索引
  * @retval 0-成功 非0-失败
  */
uBit32 BS03_SetMotorSpeed(uBit32 ulSpeedIndex)
{
    //校验数据
    if (ulSpeedIndex >= sizeof(m_md_ulMotorSpeedGroup))
    {
        return 1;
    }
    
    m_md_ulSpeedIndex = ulSpeedIndex;
    
    return 0;
}


/**
  * @brief  电机速度索引获取
  * @param  None
  * @retval 速度索引
  */
uBit32 BS03_GetMotorSpeedIndex(void)
{
    
    return m_md_ulSpeedIndex;
}


/**
  * @brief  电机最大速度索引获取
  * @param  None
  * @retval 最大速度索引
  */
uBit32 BS03_GetMaxMotorSpeedIndex(void)
{
    
    return (sizeof(m_md_ulMotorSpeedGroup)/sizeof(m_md_ulMotorSpeedGroup[0])) - 1;
}


/**
  * @brief  电机驱动管理
  * @param  None
  * @retval None
  */
void BS03_MotorDriverHandler(void)
{
    static uBit8 s_uCurStep = 0;
    
    if (SysTime_CheckExpiredState(&m_MotorCtrlTimer))
    {
        SysTime_StartOneShot(&m_MotorCtrlTimer, m_md_ulMotorSpeedGroup[m_md_ulSpeedIndex]);   //设置下一次执行的时间
        
        if (m_md_ulMotorEnable)
        {
            //电机驱动
            switch (s_uCurStep%4)
            {
            case 0: GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_A, true);  GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_B, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_C, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_D, false); break;
            case 1: GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_A, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_B, true);  GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_C, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_D, false); break;
            case 2: GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_A, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_B, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_C, true);  GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_D, false); break;
            case 3: GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_A, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_B, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_C, false); GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_D, true);  break;
            default: break;
            }
            
            //方向控制
            if (m_md_ulMotorDir)
            {
                s_uCurStep++;
            }
            else 
            {
                s_uCurStep--;
            }
        }
        else 
        {
            GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_A, false); 
            GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_B, false); 
            GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_C, false); 
            GPIO_SetOutputState(OUTPUT_IO_MOTOR_SIGNAL_D, false); 
        }
    }
    
}

