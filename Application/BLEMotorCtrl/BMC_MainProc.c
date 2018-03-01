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
#include "BMC_MainProc.h"
#include "BMC_HwCtrl.h"
#include "BMC_HardwareDef.h"
#include "BMC_MotorCtrl.h"
#include "BMC_Comman.h"
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
void BMC_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //初始化硬件
    BMC_HwInit();
    
    //电机控制
    BMC_EnableMotor(false);
    BMC_SetMotorSpeed(0);
    BMC_SetMotorDirect(BMC_MOTOR_DIRECT_CW);
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BMC_MainProc(void)
{
    //LED显示
    BMC_ShowMainWorkLed();
    
#if 0
    //按键扫描
    BMC_KeyProc();
#endif
    
    //电机驱动管理
    BMC_MotorDriverHandler();
    
    //通信管理
    BMC_ComHandler();
    
}
