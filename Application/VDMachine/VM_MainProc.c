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

#include "CMU/CMU_Interface.h"
#include "CMU/CMU_DataStructDef.h"
#include "CMU/ProtocolLayer/CMU_ExApi.h"

#if !SYS_USING_CNC
#error 售货机必须使用CNC模块
#endif


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  外部自定义CMU接口初始化
  * @param  None
  * @retval None
  */
static void VM_InitExCmu(void)
{
    //初始化外部自定义接口
    CMU_EXTERNAL_FUN_TEBLE CMUFunTable = {0};
    CMUFunTable.pf_VM_EnabletAisleMotor = VM_EnableAisleMotor;
    CMUFunTable.pf_VM_GetAisleMotorRunningState = VM_GetAisleMotorRunningState;
    CMUFunTable.pf_VM_GetTempSamplingValue = VM_GetTempSamplingValue;
    CMUFunTable.pf_CSM_SetMotorPosCtrlMotion = VM_MoveLiftMotor;
    CMUFunTable.pf_PAX_SetPulseRate = VM_SetPulseRate;
    
    CMUFunTable.pf_CSM_StartLiftPlatformHomeScan = VM_StartLiftPlatformHomeScan;
    CMUFunTable.pf_CSM_StopLiftPlatformHomeScan  = VM_StopLiftPlatformHomeScan ;
    CMUFunTable.pf_CSM_GetHomeScanStatus = VM_GetHomeScanStatus;
    
    //设置外部自定义接口
    CMU_SetExternCustomFun(&CMUFunTable);
    
}


/*****************************************************************************
 * 售货机主任务相关控制接口
 ****************************************************************************/

/**
  * @brief  售货机初始化
  * @param  None
  * @retval None
  */
void VM_Init(void)
{
    //初始化硬件
    VM_HwInit();
        
    //初始化外部自定义CMU接口
    VM_InitExCmu();
    
    //初始化升降电机
    VM_InitLiftMotor();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void VM_MainProc(void)
{
    //LED显示
    VM_ShowMainWorkLed();
    
    //货道电机控制检测
    VM_AisleMotorHandler();
    
    //升降平台限位检测
    VM_LiftPlatformLimitHandler();
    
    //升降平台回零处理
    VM_LiftPlatformHomeHandler();
    
    //温度数据采集处理
    VM_SamplingTempValue();
    
    //取货门超时检测处理
    VM_DoorLockHandler();
    
}
