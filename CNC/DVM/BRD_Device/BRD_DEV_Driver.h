#ifndef _BRD_DEV_DRIVER_H_
#define _BRD_DEV_DRIVER_H_


#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"
#include "CNC/DataStructDef/AXIS_DataStructDef.h"
#include "CNC/DataStructDef/IO_DataStructDef.h"
//#include "CNC/DataStructDef/VER_DataStructDef.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "Version/Version.h"

//板载设备驱动输出接口--------------------------------------------------------------------------------


uBit32 BRD_DEV_BusInit(uBit32 ulBusChMask);
uBit32 BRD_DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount);
void BRD_DEV_SetAxisParmReadAddr(const AXIS_PARM* pAxisParm);
void BRD_DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pAxisParm);
void BRD_DEV_SetDeviceInfoAddr(DEVICE_INFO* pDeviceInfo, uBit32 ulLen);
void BRD_DEV_SetDeviceStateAddr(DEVICE_STATE* pDeviceState, uBit32 ulLen);
uBit32 BRD_DEV_Scan(DEVICE_INFO* pDeviceInfo, uBit32 ulDevBufLen, uBit32* pRecordDevCount, uBit32* pTotalDevCount);
uBit32 BRD_DEV_SetCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult);
uBit32 BRD_DEV_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState);
uBit32 BRD_DEV_CheckFree(uBit32 ulDevNO);
uBit32 BRD_DEV_IsBusFree(void);                                            //检查总线是否空闲(总线所有有效通道都空闲)
uBit32 BRD_DEV_LockForIPO(uBit32 ulDevNo, uBit32 ulLockFlag, uBit32 ulWaitExeResult);
uBit32 BRD_DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType);
uBit32 BRD_DEV_SetParm(uBit32 ulDevNO, uBit32 ulParmType, uBit8* pParmBuf, uBit32 ulParmLen);
uBit32 BRD_DEV_SynCycleService(void);
uBit32 BRD_DEV_AsynCycleService(void);
uBit32 BRD_DEV_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8* pParmData);
uBit32 BRD_DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion);    //获取设备软件版本信息
uBit32 BRD_DEV_GetDriverVersion(SOFTWARE_VERSION* pVersion);            //获取驱动库软件版本信息
#endif
