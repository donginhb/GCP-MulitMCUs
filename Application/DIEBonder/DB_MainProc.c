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
#include "DB_MainProc.h"
#include "DB_HwCtrl.h"
#include "DB_HardwareDef.h"
#include "DB_ComMan.h"

#include "DataType/DataType.h"



/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void DB_Init(void)
{
    //硬件初始化
    DB_HwInit();
    
    
    
    
}


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void DB_MainProc(void)
{
    //LED显示
    DB_MainWorkLedShow();
    
    //通信处理
    //DB_ComHandler();
    
    //按键处理
    DB_KeyProc();
    
    //传感器采样处理
    //DB_SensorSampleProc();
    
}
