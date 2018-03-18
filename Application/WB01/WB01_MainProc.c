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
#include "WB01_MainProc.h"
#include "WB01_HwCtrl.h"
#include "WB01_HardwareDef.h"
#include "WB01_ComMan.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"



/**
  * @brief  相关资源初始化
  * @param  None
  * @retval None
  */
void WB01_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚(关闭调试引脚)
    CoreCtrl_JTAGConfig(2);
    
    //硬件初始化
    WB01_HwInit();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void WB01_MainProc(void)
{
#if 0
    //LED显示
    WB01_MainWorkLedShow();
    
    //霍尔传感器识别
    WB01_HallSensorProc();
    
    //出货流程处理
    WB01_OutGoodsHandler();
    
    //测试处理
    WB01_TestHandler();
#endif
    //通信设置
    WB01_ComHandler();
    
}
