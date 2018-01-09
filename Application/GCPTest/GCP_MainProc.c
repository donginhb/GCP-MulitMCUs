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

#include "CMU/CMU_Interface.h"
#include "CMU/CMU_DataStructDef.h"
#include "CMU/ProtocolLayer/CMU_ExApi.h"

#include "SysPeripheral/CAN/Can.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define GCP_CAN_MASTER_MODE                 (0)
#define GCP_CAN_COM_INTERVAL                (1000)  //CAN通信间隔(MS)

void GCP_CanMainProc(void)
{
#if GCP_CAN_MASTER_MODE
    
    static uBit8 uData = 0;
    static SYS_TIME_DATA m_CanCtrlTimer       = {1};    //CAN通信定时器
    
    if (SysTime_CheckExpiredState(&m_CanCtrlTimer))
    {
        SysTime_StartOneShot(&m_CanCtrlTimer, GCP_CAN_COM_INTERVAL); //设置下一次执行的时间
        
        uData++;
        
        CAN_FRAME_T CanSendFrame = {0};
        CanSendFrame.ulID = 0x34 ;
        CanSendFrame.uDLC = 1;
        CanSendFrame.uData[0] = uData;
        CanSendFrame.uType = CAN_TYPE_EXT_FRAME;
        
        CAN_Write(GCP_CAN_DEF_NODE, &CanSendFrame, 1, false);
    }
    
#else 
    
    CAN_FRAME_T CanRecvFrame = {0};
    
    if (CAN_Recv(GCP_CAN_DEF_NODE, &CanRecvFrame, 1) == 1)
    {
        if (((CanRecvFrame.ulID & CAN_EXT_ID_MASK) == 0x34) && (CanRecvFrame.uType & CAN_TYPE_EXT_FRAME))
        {
            GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
        }
    }
    
#endif
    
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
#endif
    
    //温度数据采集处理
    GCP_SamplingTempValue();
    
    GCP_CanMainProc();
    
    CAN_MainHandler(GCP_CAN_DEF_NODE);
}
