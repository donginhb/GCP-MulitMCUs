/**
  ******************************************************************************
  * @file    SYS_CmuConfig.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.30
  * @brief   System cmu module initialize
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SYS_CmuConfig.h"
#include "SYS_Ctrl.h"
#include "SysConfig.h"
#include "DataType/DataType.h"
      
#if SYS_USING_SIMP_CMU
#include "CMU/Simplify/CMU_Interface.h"
#include "CMU/Simplify/CMU_DataStructDef.h"
#elif SYS_USING_FULL_CMU
#include "CMU/CMU_Interface.h"
#include "CMU/CMU_DataStructDef.h"
#include "CMU/ProtocolLayer/CMU_ExApi.h"
#endif

#if SYS_USING_CNC
#include "CNC/CNCSYS/CNCSYS_Interface.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/ErrorCodeMan/ECM_ErrorCodeMan.h"
#include "CNC/IPO/IPO_Interface.h"
#include "CNC/MemoryAddr/CNC_AddrDef.h"
#include "CNC/MST/MST_Interface.h"
#include "CNC/SPM/SPM_SysCtrlParm.h"
#include "CNC/SPM/SPM_ParmCheck.h"
#endif

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/CoreCtrl/CoreCtrl.h"
#include "SysPeripheral/IRQ/IRQ_Man.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/UART/UART.h"

#include <stdlib.h>


/*****************************************************************************
 * CMU 初始化相关接口
 ****************************************************************************/

#if SYS_USING_FULL_CMU
static uBit8 m_CmuBuff[SYS_CMU_BUFF_SIZE] = {0};    //CMU缓冲区定义


//CMU 通信缓冲区初始化
static uBit32 SYS_InitCmuBuff(void)
{
    CMU_SetRSBufAddr((uBit32)m_CmuBuff, SYS_CMU_BUFF_SIZE);
    
    return 0;
}


//CMU 通信接口初始化
static uBit32 SYS_InitCmuUartInterface(void)
{
    //配置通信接口
    CMU_UART_INTERFACE CmuUartInterface = {0};
    
    CmuUartInterface.pf_UART_Open = UART_Open;
    CmuUartInterface.pf_UART_Close = UART_Close;
    CmuUartInterface.pf_UART_SendBuff = UART_SendBuff;
    CmuUartInterface.pf_UART_RecvBuff = UART_RecvBuff;
    CmuUartInterface.pf_UART_RecvHandler = UART_RecvHandler;
    
    CMU_UART_SetInterface(UART_NODE_0, &CmuUartInterface);
    
    //初始化通信接口
    UART_Init(SYS_CMU_COM_NODE, 115200);
    
    return 0;
}


static uBit32 SYS_InitCmuFunTable(void)
{
    CMU_EXTERNAL_FUN_TEBLE CMUFunTable = {0};
    
    //--------------------------------------------系统管理模块------------------------------------------------
    
    //系统控制接口
    CMUFunTable.pf_SYS_GetSysTickCount = SYS_GetTickCount;
    CMUFunTable.pf_SYS_GetSLCVersion = SYS_GetSLCVersion;
    CMUFunTable.pf_SYS_GetIPOVersion = SYS_GetGCPVersion;
    CMUFunTable.pf_SYS_SetComType = CMU_SetComType;
    CMUFunTable.pf_SYS_Reset = CoreCtrl_ResetSystemNow;
    
    //IO控制接口
    CMUFunTable.pf_GPIO_SetOutputPinState = GPIO_MAN_SetOutputPinState;
    CMUFunTable.pf_GPIO_GetOutputPinState = GPIO_MAN_GetOutputPinState;
    CMUFunTable.pf_GPIO_GetInputPinState = GPIO_MAN_GetInputPinState;
    
#if SYS_USING_CNC
    
#if 0
    //升级相关接口
    CMUFunTable.pf_SYS_UpdateSLC = SYS_UpdateSLC;
    CMUFunTable.pf_SYS_UpdateIPO = SYS_UpdateIPO;
    CMUFunTable.pf_SYS_PreUpdateDevProc = SYS_PreUpdateDevProc;
    CMUFunTable.pf_SYS_PostUpdateDevProc = SYS_PostUpdateDevProc;
    CMUFunTable.pf_SYS_UpdateBootloader = SYS_UpdateBootloader;
#endif
    
    CMUFunTable.pf_SYS_GetStateReadAddr = CNCSYS_GetStateReadAddr;
    
    //--------------------------------------------参数管理模块------------------------------------------------
    CMUFunTable.pf_SPM_SetSysCtrlParm = SPM_SetSysCtrlParm;
    CMUFunTable.pf_SPM_SetCrdSysAxisMapTable = SPM_SetCrdSysAxisMapTable;
    CMUFunTable.pf_SPM_SetCrdParm = SPM_SetCrdParm;
    CMUFunTable.pf_SPM_SetAxisParm = SPM_SetAxisParm;        
    CMUFunTable.pf_SPM_SetAxisPitchCmpParm = SPM_SetAxisPitchCmpParm;    
    CMUFunTable.pf_SPM_SetLookaheadEnable = SPM_SetLookaheadEnable;
    
    CMUFunTable.pf_SPM_GetSysCtrlParmAddr = SPM_GetSysCtrlParmAddr;
    CMUFunTable.pf_SPM_GetCrdSysAxisMapTableAddr = SPM_GetCrdSysAxisMapTableAddr;
    CMUFunTable.pf_SPM_GetCrdSysParmAddr = SPM_GetCrdSysParmAddr;
    CMUFunTable.pf_SPM_GetAxisParmAddr = SPM_GetAxisParmAddr;
    CMUFunTable.pf_SPM_GetAxisPitchCmpParmAddr = SPM_GetAxisPitchCmpParmAddr;
    
    CMUFunTable.pf_SPM_SaveParm = SPM_SaveConfAndParm;
    CMUFunTable.pf_SPM_SaveConfig = SPM_SaveConfAndParm;
    
    //--------------------------------------------设备管理模块------------------------------------------------
    
    CMUFunTable.pf_DEV_BlinkLED = DEV_BlinkLED;
    CMUFunTable.pf_DEV_SetConfig = SPM_SetConfig; 
    CMUFunTable.pf_DEV_GetVersion = DEV_GetVersion;
    CMUFunTable.pf_DEV_GetBusDriverVersion = DEV_GetBusDriverVersion;
    
    CMUFunTable.pf_DEV_GetCount = DEV_GetCount;
    CMUFunTable.pf_DEV_GetInfo = DEV_GetInfo;
    CMUFunTable.pf_DEV_Scan = DEV_Scan;
    CMUFunTable.pf_DEV_GetStateReadAddr = DEV_GetStateReadAddr;
    CMUFunTable.pf_DEV_SendUpdateAppData = DEV_SendUpdateAppData;
    CMUFunTable.pf_DEV_SendUpdateBootData = DEV_SendUpdateBootData;
    CMUFunTable.pf_DEV_CheckLastCmdExeState = DEV_CheckLastCmdExeState;
    
    CMUFunTable.pf_PAX_Home = PAX_Home;                                            
    CMUFunTable.pf_PAX_SetCmdPos = PAX_SetCmdPos;                
    CMUFunTable.pf_PAX_SetQeiPos = PAX_SetQeiPos;
    CMUFunTable.pf_PAX_Enable = PAX_Enable;
    CMUFunTable.pf_PAX_Reset    = PAX_Reset;
    CMUFunTable.pf_PAX_SendSvParm    = PAX_SendSvParm;
    CMUFunTable.pf_PAX_GetRunningStatus = PAX_GetRunningStatus; //获取轴运行状态 duhanfeng 2017.12.16 新增
    CMUFunTable.pf_PAX_GetCmdPos    = PAX_GetCmdPos;            //获取轴指令位置 duhanfeng 2017.12.16 新增
    CMUFunTable.pf_PAX_GetSvParm    = PAX_GetSvParm;
    
    CMUFunTable.pf_IO_SetOutputStatus = IO_SetOutputStatus;    
    CMUFunTable.pf_IO_SetOutPutPWMMode = IO_SetOutPutPWMMode;            
    CMUFunTable.pf_IO_SetOutPutPWMDuty = IO_SetOutPutPWMDuty;                
    CMUFunTable.pf_IO_SetOutPutPWMFreq = IO_SetOutPutPWMFreq;
    
#if 0
    //IO控制相关接口
    CMUFunTable.pf_GPIO_SetHSpdOutputState = GPIO_SetHSpdOutputState;    
    CMUFunTable.pf_GPIO_GetHSpdOutputStatus = GPIO_GetHSpdOutputStatus;    
    CMUFunTable.pf_GPIO_GetHSpdInputStatus = GPIO_GetHSpdInputStatus;    
    CMUFunTable.pf_MDAC_SetHSpdDaData = MDAC_SetHSpdDaData;    
    CMUFunTable.pf_MDAC_GetHSpdDaData = MDAC_GetHSpdDaData;    
    CMUFunTable.pf_QEI_GetHSpdQeiPos =  HQEI_GetHSpdQeiPos;    
    CMUFunTable.pf_QEI_GetHSpdQeiSpeed =  HQEI_GetHSpdQeiSpeed;        
#endif
    
    CMUFunTable.pf_ADDA_SetDAStatus = ADDA_SetDAStatus;
    
    //--------------------------------------------通道管理模块------------------------------------------------
    
    CMUFunTable.pf_LAX_Home = LAX_Home;                        
    CMUFunTable.pf_LAX_SetCmdPos = LAX_SetCmdPos;        
    CMUFunTable.pf_LAX_SetQeiPos = LAX_SetQeiPos;        
    CMUFunTable.pf_LAX_Enable = LAX_Enable;
    CMUFunTable.pf_LAX_CheckLastCmdExeState = LAX_CheckLastCmdExeState;
    
    CMUFunTable.pf_CSM_GetVersion = CSM_GetVersion;
    CMUFunTable.pf_CSM_Reset = CSM_Reset;
    CMUFunTable.pf_CSM_SetCtrlMode = CSM_SetCtrlMode;
    CMUFunTable.pf_CSM_EnableVerify = CSM_EnableVerify;
    CMUFunTable.pf_CSM_EnableSingleSegmentMode = CSM_EnableSingleSegmentMode;
    CMUFunTable.pf_CSM_ProgEStop = CSM_ProgEStop;
    CMUFunTable.pf_CSM_ProgRun = CSM_ProgRun;
    CMUFunTable.pf_CSM_ProgHold = CSM_ProgHold;
    CMUFunTable.pf_CSM_ProgStop = CSM_ProgStop;
    CMUFunTable.pf_CSM_ProgCancel = CSM_ProgCancel;
    CMUFunTable.pf_CSM_ProgSegmentEndStop = CSM_ProgSegmentEndStop;
    
    CMUFunTable.pf_CSN_SetRapidOverride = CSM_SetRapidOverride;
    CMUFunTable.pf_CSM_SetFeedOverride = CSM_SetFeedOverride;
    CMUFunTable.pf_CSM_SetSpindleOverride = CSM_SetSpindleOverride;
    CMUFunTable.pf_CSM_SpindleRun = CSM_SpindleRun;
    CMUFunTable.pf_CSM_SpindleStop = CSM_SpindleStop;
    
    CMUFunTable.pf_CSM_GetCrdSysStateReadAddr = CSM_GetCrdSysStateReadAddr;
    
    CMUFunTable.pf_CSM_SetMotorPosCtrlMotion = CSM_SetMotorPosCtrlMotion;
    CMUFunTable.pf_CSM_SetMotorSpeedCtrlMotion = CSM_SetMotorSpeedCtrlMotion;                    
    CMUFunTable.pf_CSM_SetMotorJogStop = CSM_SetMotorJogStop;    
    CMUFunTable.pf_CSM_MotorJogEStop = CSM_MotorJogEStop;    
    CMUFunTable.pf_CSM_SetAxisPosCtrlMotion = CSM_SetAxisPosCtrlMotion;
    CMUFunTable.pf_CSM_SetAxisSpeedCtrlMotion = CSM_SetAxisSpeedCtrlMotion;        
    CMUFunTable.pf_CSM_SetAxisJogStop = CSM_SetAxisJogStop;        
    CMUFunTable.pf_CSM_AxisJogEStop = CSM_AxisJogEStop;        
    
    CMUFunTable.pf_CSM_SetProgBufMode = CSM_SetProgBufMode;
    CMUFunTable.pf_CSM_ClearProgData = CSM_ClearProgData;
    CMUFunTable.pf_CSM_AddProgDataToBuf = CSM_AddProgDataToBuf;    
    CMUFunTable.pf_CSM_GetProgBufStateReadAddr = CSM_GetProgBufStateReadAddr;
    CMUFunTable.pf_CSM_UploadProgData = CSM_UploadProgData;
    
    CMUFunTable.pf_CSM_SetUserBufMode = CSM_SetUserBufMode;                                        
    CMUFunTable.pf_CSM_ClearUserData = CSM_ClearUserData;                
    CMUFunTable.pf_CSM_AddUserDataToBuf = CSM_AddUserDataToBuf;
    CMUFunTable.pf_CSM_GetUserBufStateReadAddr = CSM_GetUserBufStateReadAddr;
    CMUFunTable.pf_CSM_UploadUserData = CSM_UploadUserData;
    
    CMUFunTable.pf_CSM_ProgDwell = CSM_ProgDwell;
    CMUFunTable.pf_CSM_ProgMCode = CSM_ProgMCode;
    CMUFunTable.pf_CSM_ProgSCode = CSM_ProgSCode;
    CMUFunTable.pf_CSM_ProgTCode = CSM_ProgTCode;
    CMUFunTable.pf_CSM_ProgBCode = CSM_ProgBCode;
    CMUFunTable.pf_CSM_ProgIOCode = CSM_ProgIOCode;
    CMUFunTable.pf_CSM_ProgRapid = CSM_ProgRapid;
    CMUFunTable.pf_CSM_ProgLine = CSM_ProgLine;
    CMUFunTable.pf_CSM_ProgArcC = CSM_ProgArcC;
    CMUFunTable.pf_CSM_ProgArcR = CSM_ProgArcR;
    CMUFunTable.pf_CSM_ProgArcPT = CSM_ProgArcPT;
    
    //--------------------------------------------寄存器模块------------------------------------------------
#if 0
    CMUFunTable.pf_UREG_SetRegData = UREG_SetRegData;
    CMUFunTable.pf_UREG_GetRegData = UREG_GetRegData;
#endif
    //--------------------------------------------错误管理模块------------------------------------------------
    
    CMUFunTable.pf_ECM_GetErrorCode = ECM_GetErrorCode;
    
#endif
    
    CMU_SetExternFun(CMUFunTable);
    
    CMU_InitExApi();
    
    return 0;
}


/**
  * @brief  CMU初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void)
{
    SYS_InitCmuBuff();
    SYS_InitCmuFunTable();
    SYS_InitCmuUartInterface();
    CMU_Init(COM_TYPE_UART);
    
    return 0;
}

#elif SYS_USING_SIMP_CMU



//CMU 通信接口初始化
static uBit32 SYS_InitCmuUartInterface(void)
{
    //配置通信接口
    CMU_UART_INTERFACE CmuUartInterface = {0};
    
    CmuUartInterface.pf_UART_Open = UART_Open;
    CmuUartInterface.pf_UART_Close = UART_Close;
    CmuUartInterface.pf_UART_SendBuff = UART_SendBuff;
    CmuUartInterface.pf_UART_RecvBuff = UART_RecvBuff;
    CmuUartInterface.pf_UART_RecvHandler = UART_RecvHandler;
    
    CMU_UART_SetInterface(UART_NODE_0, &CmuUartInterface);
    
    //初始化通信接口
    UART_Init(SYS_CMU_COM_NODE, 115200);
    
    return 0;
}


static uBit32 SYS_InitCmuFunTable(void)
{
    CMU_EXTERNAL_FUN_TEBLE CMUFunTable = {0};
    
    //--------------------------------------------系统管理模块------------------------------------------------
    
    //系统控制接口
    //CMUFunTable.pf_SYS_UpdateSLC = SYS_UpdateSLC;
    //CMUFunTable.pf_SYS_UpdateIPO = SYS_UpdateIPO;
    CMUFunTable.pf_SYS_GetSLCVersion = SYS_GetSLCVersion;
    CMUFunTable.pf_SYS_Reset = CoreCtrl_ResetSystemNow;
    //CMUFunTable.pf_SYS_WriteSLCProgID = BOOT_WriteSLCProgID;
    
    CMU_SetExternFun(&CMUFunTable);
    
    CMU_InitExApi();
    
    return 0;
}


/**
  * @brief  CMU初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void)
{
    SYS_InitCmuFunTable();
    SYS_InitCmuUartInterface();
    CMU_Init(COM_TYPE_UART);
    
    return 0;
}



#endif //SYS_USING_FULL_CMU
