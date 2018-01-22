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
#include "GCP_MotorCtrl.h"
#include "DataType/DataType.h"
#include "SysCtrl/SysConfig.h"

#include "SysCtrl/SYS_CmuConfig.h"

#include "CMU/CMU_Interface.h"
#include "CMU/CMU_DataStructDef.h"
#include "CMU/ProtocolLayer/CMU_ExApi.h"

#include "SysPeripheral/CAN/Can.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define GCP_CAN_MASTER_MODE                 (1)

#define GCP_CAN_COM_INTERVAL                (100)  //CAN通信间隔(MS)

void GCP_CanMainProc(void)
{
    static SYS_TIME_DATA m_CanCtrlTimer       = {1};    //CAN通信定时器
    
    if (SysTime_CheckExpiredState(&m_CanCtrlTimer))
    {
        static uBit8 uStatus = 0;
        
        uBit8 uSetCmdBuff[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
        uBit8 uResetCmdBuff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        SysTime_StartOneShot(&m_CanCtrlTimer, GCP_CAN_COM_INTERVAL); //设置下一次执行的时间
        
        CAN_FRAME_T CanSendFrame = {0};
        CanSendFrame.ulID = 0x110000C6 ;
        CanSendFrame.uDLC = 8;
        CanSendFrame.uType = CAN_TYPE_EXT_FRAME;
        
        if (uStatus)
        {
            memcpy(CanSendFrame.uData, uSetCmdBuff, 8);
        }
        else 
        {
            memcpy(CanSendFrame.uData, uResetCmdBuff, 8);
        }
        
        uStatus = !uStatus;
        
        CAN_Write(GCP_CAN_DEF_NODE, &CanSendFrame, 1, false);
    }
    
}


/*****************************************************************************
 * 售货机主任务相关控制接口
 ****************************************************************************/

/**
  * @brief  售货机初始化
  * @param  None
  * @retval None
  */
void GCP_Init(void)
{
    //SYS_SetCmuCom(1, 0, 500000);
    //SYS_SetCmuCom(4, 0, 115200);
    
    //初始化硬件
    GCP_HwInit();
    
}


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void GCP_MainProc(void)
{
#if GCP_CAN_MASTER_MODE
    //LED显示
    GCP_ShowMainWorkLed();
    GCP_CanMainProc();
#endif
    
    CAN_MainHandler(GCP_CAN_DEF_NODE);
}
