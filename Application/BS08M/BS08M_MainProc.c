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
#include "BS08M_MainProc.h"
#include "BS08M_HwCtrl.h"
#include "BS08M_HardwareDef.h"
#include "BS08M_ComMan.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"



/**
  * @brief  相关资源初始化
  * @param  None
  * @retval None
  */
void BS08M_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚
    CoreCtrl_JTAGConfig();
    
    //硬件初始化
    BS08M_HwInit();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void BS08M_MainProc(void)
{
    //LED显示
    BS08M_MainWorkLedShow();
    
    //蜂鸣器控制
    BS08M_BeepHanlder();
    
    //通信管理
    BS08M_ComHandler();
    
}
