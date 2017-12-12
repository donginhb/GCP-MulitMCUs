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
#include "VM_MainProc.h"
#include "VM_HwCtrl.h"
#include "VM_HardwareDef.h"
#include "VM_MotorCtrl.h"
#include "DataType/DataType.h"
#include "SysCtrl/SysConfig.h"

/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void VM_Init(void)
{
    //初始化硬件
    VM_HwInit();
    
    //初始化坐标轴映射
#if SYS_USING_CNC
    VM_SetConfig();
#endif
    
    VM_MoveLiftMotor(0, 1000, 0);
    
}


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void VM_MainProc(void)
{
    //LED显示
    VM_MainWorkLedShow();
    
    //升降电机测试
#if SYS_USING_CNC
    VM_MoveLiftMotorTest();
#endif
    
}
