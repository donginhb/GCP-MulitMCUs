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
#include "WFC_MainProc.h"
#include "WFC_HwCtrl.h"
#include "WFC_HardwareDef.h"
#include "WFC_WifiCom.h"
#include "WFC_WifiClock.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"



/**
  * @brief  相关资源初始化
  * @param  None
  * @retval None
  */
void WFC_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚
    CoreCtrl_JTAGConfig();
    
    //硬件初始化
    WFC_HwInit();
    
    //获取天气信息
    WFC_UpdateWeather();
    
    //获取北京时间数据包
    WFC_UpdateClock();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void WFC_MainProc(void)
{
    //LED显示
    WFC_MainWorkLedShow();
    
    //更新WIFI时钟
     WFC_UpdateDisplay();
    
    //网络时间更新管理
    WFC_UpdateClockHandler();
}
