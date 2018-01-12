/**
  ******************************************************************************
  * @file    MD_MotorCtrl.c
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
#include "MD_MotorCtrl.h"
#include "MD_HardwareDef.h"
#include "DataType/DataType.h"

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <string.h>


/*****************************************************************************
 * 电机控制线程接口
 ****************************************************************************/
static  SYS_TIME_DATA m_MotorCtrlTimer = {1};   //按键控制定时器

static uBit32 m_md_ulMotorTrgTime = 0;

/**
  * @brief  电机驱动管理
  * @param  None
  * @retval None
  */
void MD_MotorDriverHandler(void)
{
    static uBit8 s_uCurSpeed = 0;
    
    if (SysTime_CheckExpiredState(&m_MotorCtrlTimer))
    {
        SysTime_StartOneShot(&m_MotorCtrlTimer, m_md_ulMotorTrgTime);   //设置下一次执行的时间
        
        switch (s_uCurSpeed)
        {
        case 0: break;
        default: break;
        }
        
    }
      
}

