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
#include "GCP_MainProc.h"
#include "GCP_HwCtrl.h"
#include "GCP_HardwareDef.h"
#include "GCP_ComMan.h"

#include "DataType/DataType.h"



/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void GCP_Init(void)
{
    //硬件初始化
    GCP_HwInit();
    
}


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void GCP_MainProc(void)
{
    //LED显示
    GCP_MainWorkLedShow();
    
    //通信处理
    //GCP_ComHandler();
    
    //按键处理
    //GCP_KeyProc();
    
    //传感器采样处理
    //GCP_SensorSampleProc();
    
}
