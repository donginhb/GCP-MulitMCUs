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
#include "BS02_MainProc.h"
#include "BS02_HwCtrl.h"
#include "BS02_HardwareDef.h"
#include "BS02_MotorCtrl.h"
#include "BS02_Comman.h"
#include "DataType/DataType.h"
#include "SysCtrl/SysConfig.h"

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"

/*****************************************************************************
 * 售货机主任务相关控制接口
 ****************************************************************************/

/**
  * @brief  售货机初始化
  * @param  None
  * @retval None
  */
void BS02_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚
    CoreCtrl_JTAGConfig(1);
    
    //初始化硬件
    BS02_HwInit();
    
    //电机控制
    BS02_EnableMotor(0);
    BS02_SetMotorSpeed(7);
    BS02_SetMotorDirect(BS02_MOTOR_DIRECT_CW);
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BS02_MainProc(void)
{
    //LED显示
    BS02_ShowMainWorkLed();
    
    //按键扫描
    //BS02_KeyProc();
    
    //电机驱动管理
    BS02_MotorDriverHandler();
    
    //通信管理
    BS02_ComHandler();
    
}
