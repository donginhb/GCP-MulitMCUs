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
#include "WFC_ComMan.h"

#include "DataType/DataType.h"



/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void WFC_Init(void)
{
    //硬件初始化
    WFC_HwInit();
    
}


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void WFC_MainProc(void)
{
    //LED显示
    WFC_MainWorkLedShow();
    
    //通信处理
    //WFC_ComHandler();
    
    //按键处理
    //WFC_KeyProc();
    
    //传感器采样处理
    //WFC_SensorSampleProc();
    
}
