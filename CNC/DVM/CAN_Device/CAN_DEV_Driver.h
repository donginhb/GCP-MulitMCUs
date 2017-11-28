#ifndef _CAN_DEV_DRIVER_H_
#define _CAN_DEV_DRIVER_H_

#include "..\\..\\CMU\\UIN_GlobalDataDef.h"
#include "..\\..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\..\\DataStructDef\\DEV_DataStructDef.h"
#include "..\\..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\..\\DataStructDef\\IO_DataStructDef.h"
#include "..\\..\\DataStructDef\\IPO_DataStructDefPub.h"

#include "..\\..\\..\\Version\\VER_DataStructDef.h"


//CAN总线驱动输出接口--------------------------------------------------------------------------------

#ifdef RS_MONITOR_ENALBE
BUS_RS_MS_DATA CAN_DEV_GetBusRsMsData(void);
#endif

uBit32 CAN_DEV_BusInit(uBit32 ulBusChMask);
uBit32 CAN_DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount);
void CAN_DEV_SetAxisParmReadAddr(const AXIS_PARM* pAxisParm);
void CAN_DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pAxisParm);
void CAN_DEV_SetDeviceInfoAddr(DEVICE_INFO* pDeviceInfo, uBit32 ulLen);
void CAN_DEV_SetDeviceStateAddr(DEVICE_STATE* pDeviceState, uBit32 ulLen);
uBit32 CAN_DEV_Scan(DEVICE_INFO* pDeviceInfo, uBit32 ulDevBufLen, uBit32* pRecordDevCount, uBit32* pTotalDevCount);
uBit32 CAN_DEV_SetCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult);
uBit32 CAN_DEV_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState);

uBit32 CAN_DEV_CheckFree(uBit32 ulDevNO);
uBit32 CAN_DEV_IsBusFree(void);                                            //检查总线是否空闲(总线所有有效通道都空闲)
uBit32 CAN_DEV_LockForIPO(uBit32 ulDevNo, uBit32 ulLockFlag, uBit32 ulWaitExeResult);
uBit32 CAN_DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType);
uBit32 CAN_DEV_SetParm(uBit32 ulDevNO, uBit32 ulParmType, uBit8* pParmBuf, uBit32 ulParmLen);
uBit32 CAN_DEV_SynCycleService(void);
uBit32 CAN_DEV_AsynCycleService(void);
//uBit32 CAN_DEV_GetState(uBit32 ulDevNO, DEVICE_STATE* pDeviceState);
uBit32 CAN_DEV_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8* pParmData);
uBit32 CAN_DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion);    //获取设备软件版本信息
uBit32 CAN_DEV_GetDriverVersion(SOFTWARE_VERSION* pVersion);            //获取驱动库软件版本信息
#endif