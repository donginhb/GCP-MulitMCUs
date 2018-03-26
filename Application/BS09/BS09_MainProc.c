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
#include "BS09_MainProc.h"
#include "BS09_HwCtrl.h"
#include "BS09_HardwareDef.h"
#include "BS09_ComMan.h"
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
void BS09_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //关闭JTAG接口
    CoreCtrl_JTAGConfig(1);
    
    //初始化硬件
    BS09_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BS09_MainProc(void)
{
    //LED显示
    BS09_ShowMainWorkLed();
    
    //按键扫描
    BS09_KeyProc();
    
    //通信管理
    BS09_ComHandler();
    
}
