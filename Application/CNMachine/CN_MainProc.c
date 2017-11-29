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
#include "CN_MainProc.h"
#include "CN_HwCtrl.h"
#include "CN_HardwareDef.h"
#include "CN_ComMan.h"

#include "DataType/DataType.h"


/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void CN_Init(void)
{
    //硬件初始化
    CN_HwInit();
    
}


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void CN_MainProc(void)
{
    //LED显示
    CN_MainWorkLedShow();
    
    //通信处理
    //CN_ComHandler();
    
    //按键处理
    //CN_KeyProc();
    
    //传感器采样处理
    //CN_SensorSampleProc();
    
}
