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
#include "BS01_MainProc.h"
#include "BS01_HwCtrl.h"
#include "BS01_HardwareDef.h"
#include "BS01_WifiCom.h"
#include "BS01_WifiClock.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"



/**
  * @brief  相关资源初始化
  * @param  None
  * @retval None
  */
void BS01_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚
    CoreCtrl_JTAGConfig(1);
    
    //硬件初始化
    BS01_HwInit();
    
    //获取天气信息
    BS01_UpdateWeather();
    
    //获取北京时间数据包
    BS01_UpdateClock();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void BS01_MainProc(void)
{
    //LED显示
    BS01_MainWorkLedShow();
    
    //更新WIFI时钟
     BS01_UpdateDisplay();
    
    //网络时间更新管理
    BS01_UpdateClockHandler();
}
