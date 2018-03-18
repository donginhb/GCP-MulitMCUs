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
#include "BS05M_MainProc.h"
#include "BS05M_HwCtrl.h"
#include "BS05M_HardwareDef.h"
#include "BS05M_ComMan.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"



/**
  * @brief  相关资源初始化
  * @param  None
  * @retval None
  */
void BS05M_Init(void)
{
    //上电延时
    SysTime_DelayMs(200);
    
    //配置JTAG引脚
    CoreCtrl_JTAGConfig(1);
    
    //硬件初始化
    BS05M_HwInit();
    
}


/**
  * @brief  主任务处理
  * @param  None
  * @retval None
  */
void BS05M_MainProc(void)
{
    //LED显示
    BS05M_MainWorkLedShow();
    
    //通信管理
    BS05M_ComHandler();
    
    //光照检测
    BS05M_SumLightDetectHandler();
    
    //人体红外检测
    BS05M_HumanDetectHandler();
    
    //灯光输出控制接口
    BS05M_LightOutputHandler();
    
}
