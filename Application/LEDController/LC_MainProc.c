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
#include "LC_MainProc.h"
#include "LC_HwCtrl.h"
#include "LC_HardwareDef.h"
#include "LC_RfCom.h"
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
void LC_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //初始化硬件
    LC_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void LC_MainProc(void)
{
    //LED显示
    LC_ShowMainWorkLed();
    
    //RF接收处理
    RF_ComHandler();
    
}
