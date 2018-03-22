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
    
    //上电复位
    //WB01_StartOutGoodsResetTask();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void WB01_MainProc(void)
{
    //LED显示
    WB01_MainWorkLedShow();
    
    //霍尔识别管理
    WB01_HallSensorProc();
    
    //出货复位管理
    WB01_OutGoodsResetHandler();
    
    //自学习管理
    WB01_SelfLearnHandler();
    
    //出货流程管理
    WB01_OutGoodsHandler();
    
    //通信管理
    WB01_ComHandler();
    
    //货道电机管理
    WB01_AisleMotorHandler();
    
    //入货门电机管理
    WB01_IndoorHandler();
    
    //出货门电机管理
    WB01_OutdoorHandler();
    
    //测试处理
    WB01_TestHandler();
    
}
