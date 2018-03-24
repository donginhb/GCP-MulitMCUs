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
#include "BS03_MainProc.h"
#include "BS03_HwCtrl.h"
#include "BS03_HardwareDef.h"
#include "BS03_MotorCtrl.h"
#include "BS03_Comman.h"
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
void BS03_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //初始化硬件
    BS03_HwInit();
    
    //电机控制
    BS03_EnableMotor(false);
    BS03_SetMotorSpeed(0);
    BS03_SetMotorDirect(BS03_MOTOR_DIRECT_CW);
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BS03_MainProc(void)
{
    //LED显示
    BS03_ShowMainWorkLed();
    
#if 0
    //按键扫描
    BS03_KeyProc();
#endif
    
    //电机驱动管理
    BS03_MotorDriverHandler();
    
    //通信管理
    BS03_ComHandler();
    
}
