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
#include "IRB_MainProc.h"
#include "IRB_HwCtrl.h"
#include "IRB_HardwareDef.h"
#include "IRB_RfCom.h"
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
void IRB_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //初始化硬件
    IRB_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void IRB_MainProc(void)
{
    //LED显示
    IRB_ShowMainWorkLed();
    
    
}
