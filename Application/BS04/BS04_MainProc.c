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
#include "BS04_MainProc.h"
#include "BS04_HwCtrl.h"
#include "BS04_HardwareDef.h"
//#include "BS04_RfCom.h"
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
void BS04_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //初始化硬件
    BS04_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BS04_MainProc(void)
{
    //LED显示
    BS04_ShowMainWorkLed();
    
    //人体检测处理
    BS04_HumanDetectHandler();

    //红外遥控接收处理
    BS04_IRHandler();

}
